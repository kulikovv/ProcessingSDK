#include "graphxml.h"
#include <QApplication>
#include <QPluginLoader>
#include <utils/logs.h>
#include <QObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <synch/synchfactory.h>

using namespace utils;
using namespace synch;
using namespace base;

GraphXml::GraphXml()
{
    _writer = 0;

}
/*****************************READING DATA***************************************/
synch::Synch* GraphXml::createGraph(QString xmlname,QMap<QString,NodeDecoration> &decor)
{
    decor.clear();
    typedef QPair<int,int> indexes;
    Node* current=0;
    QList<indexes> connections;
    QMap<int,Node*> associations;



    QFile file(xmlname);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
            CRITICAL("Error: Cannot write file "+xmlname+" : "+file.errorString())
            return 0;
    }

    QXmlStreamReader reader;
    reader.setDevice(&file);

    reader.readNext();
    while (!reader.atEnd()) {
            if (reader.isEndElement()) {
                reader.readNext();
            }

            if (reader.isStartElement()) {
                if (reader.name() == "node") {

                     QString classname = reader.attributes().value("class").toString();
                     current = Node::classForName(classname);
                     if(0==current){
                         CRITICAL("error creating " + classname)
                     }else{
                        //INFO("Reading "+classname);
                        associations.insert(reader.attributes().value("id").toString().toInt(),current);
                     }
                     reader.readNext();
                } else if(reader.name() == "library") {
                     QString libname = reader.attributes().value("name").toString();
                     NodeLibrary* lib = loadNodeLibrary(libname);
                     if(!lib)
                     {
                         CRITICAL("Critical error loading "+libname)
                     }
                     reader.readNext();
                } else if(reader.name() == "property"){
                    if(0!=current){
                        QString name = reader.attributes().value("name").toString();
                        QVariant::Type type = QVariant::nameToType(reader.attributes().value("type").toString().toUtf8());
                        QString value = reader.attributes().value("value").toString();
                        QVariant prop(value);
                        prop.convert(type);
                        //current->property()
                        if(!current->setProperty(name.toUtf8(),prop))
                        {
                            WARNING("Error setting propery"+name+" "+prop.toString())
                        }
                    }
                    reader.readNext();
                }else if(reader.name() == "decor"){
                        QString id = reader.attributes().value("id").toString();
                        float x = reader.attributes().value("x").toString().toFloat();
                        float y = reader.attributes().value("y").toString().toFloat();
                        decor[id]=NodeDecoration(QPointF(x,y));
                        reader.readNext();
                }else if(reader.name() == "connection"){
                        int first = reader.attributes().value("first").toString().toInt();
                        int second = reader.attributes().value("second").toString().toInt();
                        connections.push_back(QPair<int,int>(first,second));
                        reader.readNext();
                }else if(reader.name() == "graph"){
                     reader.readNext();
                }else{
                    WARNING("Found unknown tag "+ reader.errorString())
                      skipUnknownElement(&reader);
                }
            } else {
                reader.readNext();
            }
    }
    Synch *synch = 0;
    if(0!=associations[-1])
    {
        synch = qobject_cast<Synch*>(associations[-1]);
        associations.remove(-1);
    }
    if(0==synch){
        synch = SynchFactory::createSynch();
    }
    foreach(Node* node,associations.values())
    {
        readNode(node);
        synch->add(node);
    }

    foreach(indexes pair,connections)
    {
        Node* first = associations.value(pair.first,0);
        Node* second = associations.value(pair.second,0);
        if(0!=first&&0!=second)
        {
           // INFO("Connect "+first->objectName()+" and "+second->objectName());
            synch->link(first,second);
        }else{
            WARNING("Node not found")
        }

    }

    return synch;

}

void GraphXml::readNode(base::Node* /*node*/)
{
    return;
}

void GraphXml::skipUnknownElement(QXmlStreamReader* reader)
{
    reader->readNext();
    while (!reader->atEnd()) {
        if (reader->isEndElement()) {
            reader->readNext();
            break;
        }

        if (reader->isStartElement()) {
            skipUnknownElement(reader);
        } else {
            reader->readNext();
        }
    }
}

/*****************************STORING DATA***************************************/
void GraphXml::writeNode(int id,Node* nd)
{
    if(0==_writer)
    {
        CRITICAL("Error: no writer")
        return;
    }
    _writer->writeStartElement("node");
    _writer->writeAttribute("id", QString("%1").arg(id));
    const QMetaObject* metaObject = nd->metaObject();

    _writer->writeAttribute("class", metaObject->className());


    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i)
    {
         QMetaProperty property = metaObject->property(i);
         if(property.isWritable())
         {
             QVariant v = nd->property(property.name());
             if(v.isValid())
             {
                _writer->writeStartElement("property");
                _writer->writeAttribute("name", property.name());
                _writer->writeAttribute("type", v.typeName());
                _writer->writeAttribute("value", v.toString());
                _writer->writeEndElement();
             }
         }
    }
    _writer->writeEndElement();
}

void GraphXml::onConnection(int first,int second)
{
    if(0==_writer)
    {
        CRITICAL("Error: no writer")
        return;
    }
    _writer->writeStartElement("connection");
    _writer->writeAttribute("first", QString("%1").arg(first));
    _writer->writeAttribute("second", QString("%1").arg(second));
    _writer->writeEndElement();
}

void GraphXml::storeGraph(QString xmlname,Graph* graph,QList<QPair<QString,QPointF> > decor)
{

    QFile file(xmlname);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        CRITICAL("Error: Cannot write file "+xmlname + " : "+file.errorString())
            return;
    }

    _writer = new QXmlStreamWriter(&file);

    _writer->setAutoFormatting(true);
    _writer->writeStartDocument();
    _writer->writeStartElement("graph");

    QStringList libs;
    foreach(base::Node *node,graph->nodes("all"))
    {
        if(libs.contains(node->library())&&!node->library().isEmpty())
        {
            libs << node->library();
        }
    }

    foreach(QString lib,libs)
    {
        _writer->writeStartElement("library");
        _writer->writeAttribute("name", lib);
        _writer->writeEndElement();
    }

    parseGraph(graph);

    typedef QPair<QString,QPointF> pair;
    foreach(pair p,decor)
    {
        _writer->writeStartElement("decor");
        _writer->writeAttribute("id", QString("%1").arg(p.first));
        _writer->writeAttribute("x", QString("%1").arg(p.second.x()));
        _writer->writeAttribute("y", QString("%1").arg(p.second.y()));
        _writer->writeEndElement();
    }

    _writer->writeEndElement();
    _writer->writeEndDocument();
    file.close();
}

NodeLibrary* GraphXml::loadNodeLibrary(QString name)
{
    if(name.contains("Qt5")){
        return 0;
    }
    QDir dir(QApplication::applicationDirPath());
    QString path = dir.absoluteFilePath(name);

    QPluginLoader loader(path);

    if(!QFile::exists(path))
    {
        CRITICAL("Library not found:"+ name);
        return 0;
    }
    if (NodeLibrary *interface = qobject_cast<NodeLibrary *>(loader.instance()))
    {
        interface->registrate(name);
        return interface;
    }else{

       // CRITICAL("Error fail reading library:"+ name +loader.errorString());

    }

    return 0;
}




