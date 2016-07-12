#include "graphdom.h"
#include <QDomDocument>
#include <QApplication>
#include <QPluginLoader>
#include <QObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QFile>
#include <QDir>
#include <utils/logs.h>
#include <synch/synchfactory.h>


using namespace utils;

/*!
 * \brief addElement добавляет новый элемент в структуру документа
 * \param doc документ в который добавляется узел
 * \param node узел родитель
 * \param tag название нового узла <NODE/>
 * \param value текст внутри узла <node>TEXT</node>
 * \return
 */
QDomElement addElement( QDomDocument &doc, QDomNode &node,
                        const QString &tag,
                        const QString &value = QString::null )
{
  QDomElement el = doc.createElement( tag );
  node.appendChild( el );
  if ( !value.isNull() ) {
    QDomText txt = doc.createTextNode( value );
    el.appendChild( txt );
  }
  return el;
}

/*!
 * \brief onElement запись параметров узла в виде строк
 * \param doc документ
 * \param node узел в документе, соотвествующий сохраняемому узлу графа
 * \param nd узел графа, настройки которого сохраняются
 */
void onElement(QDomDocument &doc, QDomElement &node,Node* nd)
{
    const QMetaObject* metaObject = nd->metaObject();
    node.setAttribute("class", metaObject->className());

    // Итерации по всем свойствам объекта
    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i)
    {
         QMetaProperty property = metaObject->property(i);
         if(property.isWritable())
         {
             QVariant v = nd->property(property.name());
             if(v.isValid())
             {
                QDomElement ppty = addElement( doc, node, "property");
                ppty.setAttribute("name", property.name());
                ppty.setAttribute("type", v.typeName());
                ppty.setAttribute("value", v.toString());
             }
         }
    }
}
/*!
 * \brief loadNodeLibrary загрузить и зарегистрировать библиотеку плагин с интерфейсом qptk
 * \param name путь к библиотеке
 * \return указатель на библиотеку
 */
NodeLibrary* loadNodeLibrary(QString name)
{
   // static QStringList loadedLibraries;
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

       //CRITICAL("Error fail reading library:"+ name +loader.errorString());
    }

    return 0;
}

/*!
 * \brief readConnections почитать все соединения для узла
 * \param el элемент типа node (родитель для всех соединений)
 * \return список пар соедений в строовом формате для того, чтобы парсить правильно смотреть parseConnectionString
 */
QList<QPair<QString,QString> > readConnections(QDomElement el)
{
    QList<QPair<QString,QString> > connections;
    QDomNodeList connectionList =el.elementsByTagName("connection");
    for(int i=0;i<connectionList.count();i++)
    {
        QDomElement el = connectionList.at(i).toElement();
        connections.push_back(QPair<QString,QString>(el.attribute("from"),el.attribute("to")));
    }
    return connections;
}

bool readLibraries(QDomElement el)
{
    //Read all dependant librararies
    QDomNodeList libraryHeaders = el.elementsByTagName("libs");
    for(int i=0;i<libraryHeaders.count();i++)
    {
      //  INFO("Library header found");
        //read each node
        QDomElement headerElement = libraryHeaders.at(i).toElement();
        QDomNodeList libraries = headerElement.elementsByTagName("library");
        for(int i=0;i<libraries.count();i++)
        {

            QDomElement librarie = libraries.at(i).toElement();
           // INFO("Librarie->"+librarie.text());
            NodeLibrary* lib = loadNodeLibrary(librarie.text());
            if(!lib)
            {
                CRITICAL("Critical error loading library "+librarie.text())
                return false;
            }
        }
    }
    return true;
}

/*!
 * \brief readNode считать настроки
 * \param el
 * \param connections
 * \param dec
 * \return
 */
base::Node* readNode(QDomElement el,QList<QPair<QString,QString> > &connections,NodeDecoration& dec)
{
    QString classname = el.attribute("class");
    //int id = el.attribute("id").toInt();
    readLibraries(el);

    base::Node* node = Node::classForName(classname);
    if(0==node)
    {
        CRITICAL("error creating " + classname)
        return 0;
    }

    QDomNodeList pptyList = el.elementsByTagName("property");
    QDomNodeList connectionList =el.elementsByTagName("connections");
    QDomNodeList decorationList =el.elementsByTagName("decoration");
    for(int i=0;i<pptyList.count();i++)
    {
        //read all node properies
        QDomElement ppty = pptyList.at(i).toElement();

        //Check if current item is child of subitem
        if(el!=ppty.parentNode().toElement())
            continue;

        QVariant::Type type = QVariant::nameToType(ppty.attribute("type").toUtf8());
        QVariant prop(ppty.attribute("value"));
        if(!prop.convert(type))
        {
            WARNING("Error converting "+ppty.attribute("name")+" "+prop.toString()+" to "+ppty.attribute("type"))
            continue;
        }

        if(!node->setProperty(ppty.attribute("name").toUtf8(),prop))
        {
            WARNING("Error setting propery "+ppty.attribute("name")+" "+prop.toString())
        }
    }
    for(int i=0;i<connectionList.count();i++)
    {

        //read all node connections
        QList<QPair<QString,QString> > pairs;
        pairs = readConnections(connectionList.at(i).toElement());
        for(int j=0;j<pairs.count();j++)
        {
            if(!connections.contains(pairs.at(j)))
            {
                connections.push_back(pairs.at(j));
            }
        }
    }
    for(int i=0;i<decorationList.count();i++){
        QDomElement eldec = decorationList.at(i).toElement();
        dec.place = QPoint(eldec.attribute("x").toInt(),eldec.attribute("y").toInt());
        dec.base = false;
    }
    return node;
}
/*!
 * \brief parseConnectionString разбор узла части соединения в новом формате ("названиеузла@названиеграфа")
 * \param connectionstring строка в новом формате  ("названиеузла@названиеграфа")
 * \param subGraphs список всех существующих подграфов
 * \return указатель на узел, если такой есть
 */
base::Node* parseConnectionString(QString connectionstring, QMap<QString,base::Graph*> &subGraphs)
{
    base::Node* node = 0;
    QStringList details = connectionstring.split("@");
    QString key = "";//Пустой граф - текущий загружаемый граф
    if(details.count()>1)
    {
        key = details[1];
    }


    base::Graph* subgraph = subGraphs[key];

    if(subgraph)
    {

          node = subgraph->node(details[0]);

    }
    return node;
}

synch::Synch* GraphDom::loadGraph(QString xmlname)
{
    QMap<QString, NodeDecoration> drop;
    return GraphDom::loadGraph(xmlname,drop);
}

/*!
 * \brief loadGraphFormat1 загрузка графа в старом формате
 * \param docElem
 * \param decor массив для декора
 * \return
 */
synch::Synch* loadGraphFormat1(QDomElement docElem,QMap<QString, NodeDecoration> &decor)
{
    typedef QPair<int,int> indexes;//пары для соединения
    QList<indexes> connections;//соединения, найденные в файле
    QMap<int,Node*> associations;//соотношения номера к узлу

    QDomNodeList libraryList = docElem.elementsByTagName("library");
    for(int i=0;i<libraryList.count();i++)
    {
        //read each node
        QDomElement el = libraryList.at(i).toElement();
        NodeLibrary* lib = loadNodeLibrary(el.attribute("name"));
        if(!lib)
        {
            CRITICAL("Critical error loading library "+el.attribute("name"))
            return 0;
        }
    }

    //Для совместимости!!!
    NodeDecoration dec;
    QList<QPair<QString,QString> > connectionBase;


    QDomNodeList nodesList = docElem.elementsByTagName("node");
    for(int i=0;i<nodesList.count();i++)
    {
        //read each node
        QDomElement el = nodesList.at(i).toElement();
        int id = el.attribute("id").toInt();
        base::Node* node = readNode(el,connectionBase,dec);
        if(node)
        {
          //  graph->add(node);
            if(node)
            {
                associations.insert(id,node);
            }
        }
    }
    QDomNodeList decorList = docElem.elementsByTagName("decor");
    for(int i=0;i<decorList.count();i++)
    {
        //read each decor
        QDomElement el = decorList.at(i).toElement();
        float x = el.attribute("x").toFloat();
        float y = el.attribute("y").toFloat();
        decor[el.attribute("id")]=NodeDecoration(QPointF(x,y));
    }

    //Считываем соединения
    QDomNodeList connectionList = docElem.elementsByTagName("connection");
    for(int i=0;i<connectionList.count();i++)
    {
        QDomElement el = connectionList.at(i).toElement();
        int first = el.attribute("first").toInt();
        int second = el.attribute("second").toInt();
        connections.push_back(QPair<int,int>(first,second));
    }

    //Механизм создания синхронизатора
    synch::Synch *synch = 0;
    if(0!=associations[-1])
    {
        //Если в стаором формате синхронизатор имел индекс -1, если он был - то все ок
        synch = qobject_cast<synch::Synch*>(associations[-1]);
        associations.remove(-1);
    }

    if(0==synch){
        synch = synch::SynchFactory::createSynch();
    }
    //Добавляем граф все загруженные узлы
    foreach(Node* node,associations.values())
    {
        if(node)
        {
            synch->add(node);
        }
    }

    // Соединяем соответсвующие узлы
    foreach(indexes pair,connections)
    {
        Node* first = associations.value(pair.first,0);
        Node* second = associations.value(pair.second,0);
        if(0!=first&&0!=second)
        {
            //INFO("Connect "+first->objectName()+" and "+second->objectName());
            synch->link(first,second);
        }else{
            WARNING("Node not found")
        }

    }
    return synch;

}
/*!
 * \brief loadGraphFormat2 загрузить в новом формате (поддрержка подграфов и тд)
 * \param docElem (гланый узел, с которога начинается парсинг документа)
 * \param decor настройки декорации
 * \return
 */
synch::Synch* loadGraphFormat2(QDomElement docElem,QMap<QString, NodeDecoration> &decor)
{
    QList<QPair<QString,QString> > connectionBase;
    //read the graph settings and test if it is a graph
    NodeDecoration dec;
    synch::Synch* graph = qobject_cast<synch::Synch*>(readNode(docElem,connectionBase,dec));
    if(!graph)
    {
        CRITICAL("Root node is not graph convertable")
        return 0;
    }


    // get the node's interested in, this time only caring about person's
    QMap<QString,base::Graph*> subGraphs;
    subGraphs.insert("",graph);

    //Reading subgraph settings
    QDomNodeList subGraphList = docElem.elementsByTagName("subgraph");
    for(int i=0;i<subGraphList.count();i++)
    {
        //read subgraph
        QDomElement el = subGraphList.at(i).toElement();
        base::Graph* subgraph = GraphDom::loadGraph(el.attribute("path"),decor);
        if(subgraph)
        {
            //Load subgraph
            graph->addGraph(subgraph,el.attribute("path"));
            //
            foreach(base::Node *subnode,subgraph->nodes())
            {
                decor[subnode->objectName()].base = true;
            }

            QDomNodeList connectionList =el.elementsByTagName("connections");
            for(int i=0;i<connectionList.count();i++)
            {
                //read all node connections
                QList<QPair<QString,QString> > pairs;
                pairs = readConnections(connectionList.at(i).toElement());
                for(int j=0;j<pairs.count();j++)
                {
                    if(!connectionBase.contains(pairs.at(j)))
                    {
                        connectionBase.push_back(pairs.at(j));
                    }
                }
            }
            subGraphs.insert(el.attribute("path"),subgraph);
        }
    }

    QDomNodeList nodesList = docElem.elementsByTagName("node");
    for(int i=0;i<nodesList.count();i++)
    {
        //read each node
        QDomElement el = nodesList.at(i).toElement();
        base::Node* node = readNode(el,connectionBase,dec);
        if(node)
        {
            graph->add(node);
            decor.insert(node->objectName(),NodeDecoration(dec));
        }
    }

    for(int i=0;i<connectionBase.count();i++)
    {
        base::Node* first   = parseConnectionString(connectionBase.at(i).first,subGraphs);
        base::Node* second  = parseConnectionString(connectionBase.at(i).second,subGraphs);

        if(!graph->link(first,second))
        {
            CRITICAL("Fail connecting "+connectionBase.at(i).first+" and "+connectionBase.at(i).second)
        }

    }
    return graph;
}

/*!
 * \brief preLoadGraph предварительная проверка Dom структуры
 * \param docElem коренной элемент Dom структуры
 * \param decor декорации
 * \return указатель на граф
 */
synch::Synch* preLoadGraph(QDomElement docElem,QMap<QString, NodeDecoration> &decor)
{

    // you could check the root tag name here if it matters
    if("graph"!=docElem.tagName())
    {
        CRITICAL("Root is not a graph")
        return 0;
    }

    switch(docElem.attribute("version","0").toInt())
    {
        case 0:return loadGraphFormat1(docElem,decor);
        case 2:return loadGraphFormat2(docElem,decor);
        default:return loadGraphFormat1(docElem,decor);
    }
    return 0;
}

synch::Synch* GraphDom::loadGraphMemory(QString xml)
{
    QMap<QString, NodeDecoration> drop;
    return GraphDom::loadGraphMemory(xml,drop);
}

synch::Synch* GraphDom::loadGraphMemory(QString xml,QMap<QString,NodeDecoration> &decor)
{
    QDomDocument doc("mydocument");
    if (!doc.setContent(xml))
    {
        CRITICAL("Not valid content")
        return 0;
    }

    QDomElement docElem = doc.documentElement();

    return preLoadGraph(docElem,decor);
}

synch::Synch* GraphDom::loadGraph(QString xmlname, QMap<QString, NodeDecoration> &decor)
{
    QFile file(xmlname);
    /* If we can't open it, let's show an error message. */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        CRITICAL("File not found "+xmlname+" in "+QDir::current().path())
        return 0;
    }

    /* QDomDocument takes any QIODevice. as well as QString buffer*/
    QDomDocument doc("mydocument");
    if (!doc.setContent(&file))
    {
        CRITICAL("Not valid content")
        return 0;
    }

    //Switch to current diractory
    QFileInfo finfo(file);
    QDir::setCurrent(finfo.dir().path());
 //   INFO("Switch to"+finfo.dir().path()+" got "+QDir::current().path())


    QDomElement docElem = doc.documentElement();
    // you could check the root tag name here if it matters
    synch::Synch* synch = preLoadGraph(docElem,decor);
    if(!synch){
        return 0;
    }
    synch->setObjectName(finfo.baseName());
    return synch;
}

QString GraphDom::saveGraphMemoryExpanded(synch::Synch* graph,QMap<QString,NodeDecoration> &decor)
{
    QDomDocument doc;
    QDomProcessingInstruction instr = doc.createProcessingInstruction(
                        "xml", "version='1.0' encoding='UTF-8'");
    doc.appendChild(instr);

    QDomElement root = addElement( doc, doc, "graph" );
    root.setAttribute("version",2);
    onElement(doc,root,graph);


    QStringList libs;
    QDomElement libsnode = addElement( doc, root, "libs" );
    foreach(base::Node *node,graph->nodes("all"))
    {
        if(!libs.contains(node->library())&&!node->library().isEmpty())
        {
            libs << node->library();
            addElement( doc, libsnode, "library", node->library() );
        }
    }

    if(!libs.contains(graph->library())&&!graph->library().isEmpty())
    {
        libs << graph->library();
        addElement( doc, libsnode, "library", graph->library() );
    }

    QList<Graph::Connection> conn = graph->getConnections();
    QList<base::Node*> used_nodes;
    foreach(Graph::Connection c,conn){

        base::Node* nd = 0;
        for(int i=0;i<2;i++)
        {
            switch(i){
                case 0: nd=c.first; break;
                case 1: nd=c.second; break;
                default:nd=c.first; break;
            }


            if(!used_nodes.contains(nd)){
                QDomElement node = addElement( doc, root, "node");
                node.setAttribute("id",QString("%1").arg(graph->nodeID(nd)));
                onElement(doc,node,nd);

                QDomElement decoration = addElement( doc, node, "decoration");
                decoration.setAttribute("x",decor[nd->objectName()].place.x());
                decoration.setAttribute("y",decor[nd->objectName()].place.y());

                QDomElement connode = addElement( doc, node, "connections");
                foreach(Graph::Connection con,conn){
                    if(nd==con.first)
                    {
                        QDomElement lc = addElement( doc, connode, "connection");
                        lc.setAttribute("to",con.second->objectName());
                        lc.setAttribute("from",con.first->objectName());
                    }
                }
                used_nodes.push_back(nd);
            }
        }
    }

    return doc.toString();
}

QString GraphDom::saveGraphMemory(synch::Synch* graph,QMap<QString,NodeDecoration> &decor)
{
    QDomDocument doc;
    QDomProcessingInstruction instr = doc.createProcessingInstruction(
                        "xml", "version='1.0' encoding='UTF-8'");
    doc.appendChild(instr);

    QDomElement root = addElement( doc, doc, "graph" );
    root.setAttribute("version",2);
    onElement(doc,root,graph);


    QStringList libs;
    QDomElement libsnode = addElement( doc, root, "libs" );
    foreach(base::Node *node,graph->nodes("all"))
    {
        if(!libs.contains(node->library())&&!node->library().isEmpty())
        {
            libs << node->library();
            addElement( doc, libsnode, "library", node->library() );
        }
    }

    if(!libs.contains(graph->library())&&!graph->library().isEmpty())
    {
        libs << graph->library();
        addElement( doc, libsnode, "library", graph->library() );
    }
    //qDebug() << "Current path"<<QDir::current().path();



    QList<Graph::Connection> conn = graph->getConnections();
    qDebug() << conn;
    foreach(base::Node* nd,graph->nodes())
    {
        if(0!=qobject_cast<base::Graph*>(nd))
        {
            QDomElement node = addElement( doc, root, "subgraph");
            node.setAttribute("path",QDir::current().relativeFilePath(nd->objectName()));
            QDomElement connode = addElement( doc, node, "connections");
            foreach(Graph::Connection con,conn)
            {       
                if(nd->objectName()==graph->nodeOwner(con.first)&&nd->objectName()!=graph->nodeOwner(con.second))
                {
                            QDomElement lc = addElement( doc, connode, "connection");
                            lc.setAttribute("to",con.second->objectName()+"@"+QDir::current().relativeFilePath(graph->nodeOwner(con.second)));
                            lc.setAttribute("from",con.first->objectName()+"@"+QDir::current().relativeFilePath(graph->nodeOwner(con.first)));
                }
            }

            continue;
        }
        QDomElement node = addElement( doc, root, "node");
        node.setAttribute("id",QString("%1").arg(graph->nodeID(nd)));
        onElement(doc,node,nd);

        QDomElement decoration = addElement( doc, node, "decoration");
        decoration.setAttribute("x",decor[nd->objectName()].place.x());
        decoration.setAttribute("y",decor[nd->objectName()].place.y());

        QDomElement connode = addElement( doc, node, "connections");
        foreach(Graph::Connection con,conn){
            if(nd==con.first)
            {
                QDomElement lc = addElement( doc, connode, "connection");
                lc.setAttribute("to",con.second->objectName()+"@"+QDir::current().relativeFilePath(graph->nodeOwner(con.second)));
                lc.setAttribute("from",con.first->objectName()+"@"+QDir::current().relativeFilePath(graph->nodeOwner(con.first)));
            }
        }
    }
    return doc.toString();
}

void GraphDom::saveGraph(QString xmlname, synch::Synch* graph,QMap<QString,NodeDecoration> &decor,bool expand)
{
    QFile file(xmlname);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        CRITICAL("Error: Cannot write file "+xmlname + " : "+file.errorString())
        return;
    }

    QFileInfo finfo(file);
    QDir::setCurrent(finfo.dir().path());

    QTextStream stream(&file);
    if(!expand){
        stream << saveGraphMemory(graph,decor);
    }else{
        stream << saveGraphMemoryExpanded(graph,decor);
    }
    stream.flush();
}

