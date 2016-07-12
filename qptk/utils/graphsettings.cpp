#include "graphsettings.h"
#include <QSettings>
#include <base/node.h>
#include <QMetaProperty>
#include <utils/logs.h>
#include <io/outputnode.h>
#include <QApplication>
#include <base/graph.h>
#include <QFile>

using namespace utils;
/*
GraphSettings::GraphSettings(QString graphname,QString setting,QString application)
{
    _settings = new QSettings(application,graphname);
    _settingName = setting;
	_ignoreUrls = true;
}

void GraphSettings::setIgnoreUrl(bool value)
{
	this->_ignoreUrls = value;
}



GraphSettings::~GraphSettings()
{
    if(0!=_settings)
    {
        delete _settings;
        _settings = 0;
    }
}
void GraphSettings::storeGraph(QString ,base::Graph* graph,QStringList ,QList<QPair<int,QPointF> > )
{
    if(!_settings)
    {
        return;
    }
    _settings->beginGroup(_settingName);
    parseGraph(graph);
    _settings->endGroup();
}

void GraphSettings::readNode(base::Node* node)
{
    if(0==node){
        INFO("Error reading node")
        return;
    }

    _settings->beginGroup(_settingName);
    _settings->beginGroup(node->objectName());

    const QMetaObject* metaObject = node->metaObject();
    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i)
    {
         QMetaProperty property = metaObject->property(i);
         if(property.isWritable())
         {
             QString pname = QString(property.name());
             if("url"==pname&&_ignoreUrls){
                 if(qobject_cast<io::OutputNode*>(node)){
                     continue;
                 }
             }

             QVariant v = node->property(property.name());
             if(v.isValid())
             {


                 _settings->beginGroup(property.name());
                 QVariant type = _settings->value("type");
                 QVariant value = _settings->value("value");
                 _settings->endGroup();

                 if(type.isValid()&&value.isValid())
                 {
                     QVariant::Type typex = QVariant::nameToType(type.toString().toAscii());

                     QVariant prop(value.toString());
                     prop.convert(typex);

                     if(!node->setProperty(property.name(),prop))
                     {
                         INFO("Error setting property "+QString(property.name())+" "+prop.toString())
                     }
                 }
             }
         }
    }

    _settings->endGroup();
    _settings->endGroup();

    return;
}
void GraphSettings::writeNode(int ,base::Node* node)
{
    if(!_settings)
    {
        return;
    }

    if(0==node){
        return;
    }

    const QMetaObject* metaObject = node->metaObject();


    _settings->beginGroup(node->objectName());
   // _settings->setValue("class", metaObject->className());

    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i)
    {
         QMetaProperty property = metaObject->property(i);
         if(property.isWritable())
         {
             QVariant v = node->property(property.name());
             if(v.isValid())
             {
                 _settings->beginGroup(property.name());
                 _settings->setValue("type",v.typeName());
                 _settings->setValue("value", v.toString());
                 _settings->endGroup();
             }
         }
    }
    _settings->endGroup();
}

void GraphSettings::onConnection(int ,int )
{
    return;
}*/

void readNode(QSettings *settings,base::Node* node)
{
    if(0==node){
        WARNING("Error reading node")
        return;
    }


    settings->beginGroup(node->objectName());

    const QMetaObject* metaObject = node->metaObject();
    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i)
    {
         QMetaProperty property = metaObject->property(i);
         if(property.isWritable())
         {
            /* QString pname = QString(property.name());
             if("url"==pname&&_ignoreUrls){
                 if(qobject_cast<io::OutputNode*>(node)){
                     continue;
                 }
             }*/

             QVariant v = node->property(property.name());
             if(v.isValid())
             {


                 settings->beginGroup(property.name());
                 QVariant type = settings->value("type");
                 QVariant value = settings->value("value");
                 settings->endGroup();

                 if(type.isValid()&&value.isValid())
                 {
                     QVariant::Type typex = QVariant::nameToType(type.toString().toUtf8());

                     QVariant prop(value.toString());
                     prop.convert(typex);

                     if(!node->setProperty(property.name(),prop))
                     {
                         WARNING("Error setting property "+QString(property.name())+" "+prop.toString())
                     }
                 }
             }
         }
    }

    settings->endGroup();
    return;
}

void writeNode(QSettings *settings,base::Node* node)
{
    if(0==node){
        return;
    }
    settings->beginGroup(node->objectName());

    const QMetaObject* metaObject = node->metaObject();
    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i)
    {
         QMetaProperty property = metaObject->property(i);
         if(property.isWritable())
         {
             QVariant v = node->property(property.name());
             if(v.isValid())
             {
                 settings->beginGroup(property.name());
                 settings->setValue("type",v.typeName());
                 settings->setValue("value", v.toString());
                 settings->endGroup();
             }
         }
    }
    settings->endGroup();
}

QStringList GraphSettings::getStoredConfigs(base::Graph *graph)
{
    if(!graph)
    {
        return QStringList();
    }
    QSettings* settings=new QSettings(QApplication::organizationName(),QApplication::applicationName());
    settings->beginGroup(graph->objectName());
    QStringList keys = settings->allKeys();
    delete settings;
    return keys;
}

void GraphSettings::saveSettingsFile(base::Graph* graph,QString filename)
{
    QSettings* settings = new QSettings(filename,QSettings::IniFormat);
    //Ключами лучше пользоваться название организации + название приложения, чтобы не разбрасывать файлы
    settings->beginGroup(graph->objectName());
    saveGraph(graph,settings);
    settings->endGroup();
    delete settings;
}

void GraphSettings::loadSettingsFile(base::Graph* graph,QString filename)
{
    QSettings* settings = new QSettings(filename,QSettings::IniFormat);

    settings->beginGroup(graph->objectName());
    loadGraph(graph,settings);
    settings->endGroup();
    delete settings;
}

void GraphSettings::saveSettings(base::Graph* graph,QString setting)
{
    if(!graph)
    {
        return;
    }
    QSettings* settings=new QSettings(QApplication::organizationName(),QApplication::applicationName());

    //Ключами лучше пользоваться название организации + название приложения, чтобы не разбрасывать файлы
    settings->beginGroup(graph->objectName());
    settings->beginGroup(setting);

    saveGraph(graph,settings);

    settings->endGroup();
    settings->endGroup();
    delete settings;
}

void GraphSettings::loadSettings(base::Graph* graph,QString setting)
{
    if(!graph)
    {
        return;
    }

    QSettings* settings  = new QSettings(QApplication::organizationName(),QApplication::applicationName());


    settings->beginGroup(graph->objectName());
    settings->beginGroup(setting);

    loadGraph(graph,settings);

    settings->endGroup();
    settings->endGroup();
    delete settings;
}

void GraphSettings::saveGraph(base::Graph* graph, QSettings* settings)
{
    writeNode(settings,graph);//Сохраняем настройки главного графа
    foreach(base::Node* nd,graph->nodes("all"))//Сохраняем настройки всех его узлов, включая подграфиы
    {
        writeNode(settings,nd);//Сохраняем
    }
}

void GraphSettings::loadGraph(base::Graph* graph, QSettings* settings)
{
    readNode(settings,graph);
    foreach(base::Node* nd,graph->nodes("all"))//Загружаем настройки всех его узлов, включая подграфиы
    {
        readNode(settings,nd);//Загружаем
    }
}


