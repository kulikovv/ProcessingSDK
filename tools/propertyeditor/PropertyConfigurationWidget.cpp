#include "PropertyConfigurationWidget.h"
#include <QMetaObject>
#include <QMetaProperty>

PropertyConfigurationWidget::PropertyConfigurationWidget(QWidget *parent) :
    QWidget(parent)
{
}

PropertyConfigurationWidget::~PropertyConfigurationWidget()
{
    foreach(QObject* obj,_objectStatus.keys())
    {
        delete _objectStatus[obj];
    }
}

void PropertyConfigurationWidget::storeObjectState(QList<QObject*> objects)
{
    foreach(QObject* obj,objects)
    {
        propertyState* state = new propertyState();
        _objectStatus.insert(obj,state);

        const QMetaObject* metaObject = obj->metaObject();

        int count = metaObject->propertyCount();
        for (int i=0; i<count; ++i)
        {
             QMetaProperty property = metaObject->property(i);
             if(property.isWritable())
             {
                 QVariant v = obj->property(property.name());
                 if(v.isValid())
                 {
                     savedProperty item(QString(property.name()),v);
                     state->push_back(item);
                 }
             }
        }
    }
}

void PropertyConfigurationWidget::restoreObjectState()
{
    foreach(QObject* obj,_objectStatus.keys())
    {
        propertyState* state= _objectStatus[obj];

        foreach(savedProperty sp,*state)
        {
            obj->setProperty(sp.first.toUtf8(),sp.second);
        }
    }
    emit doUpdate();
}
