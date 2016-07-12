#include "PropertyTabWidget.h"
#include "QPropertyEditorWidget.h"
#include <QDebug>
#include <QHBoxLayout>

PropertyTabWidget::PropertyTabWidget(QList<QObject*> nodes, QMap<QObject *, QIcon> icons, QWidget *parent) :
    PropertyConfigurationWidget(parent)
{
    int index=0;
    _tabWidget = new QTabWidget;
    storeObjectState(nodes);
    foreach(QObject* obj,nodes)
    {

        QPropertyEditorWidget* ew = new QPropertyEditorWidget(this);
        ew->addObject(obj);
        _tabWidget->insertTab(index,ew,icons.value(obj,QIcon()),obj->objectName());
        index++;
    }
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(_tabWidget);
    setLayout(horizontalLayout);
    connect(this,SIGNAL(doUpdate()),this,SLOT(onUpdate()));
}
void PropertyTabWidget::onUpdate()
{
    _tabWidget->currentWidget()->update();
}

PropertyTabWidget::~PropertyTabWidget()
{
    delete _tabWidget;
}


