#include "PropertyStackedWidget.h"
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include "QPropertyEditorWidget.h"

PropertyStackedWidget::PropertyStackedWidget(QList<QObject *> nodes, QMap<QObject *, QIcon> icons, QWidget *parent) :
    PropertyConfigurationWidget(parent)
{
    _contentsWidget = new QListWidget(this);
    _contentsWidget->setViewMode(QListView::ListMode);
    _contentsWidget->setIconSize(QSize(32, 32));
    _contentsWidget->setMovement(QListView::Static);
    _contentsWidget->setMaximumWidth(256);

    _pagesWidget = new QStackedWidget(this);
    storeObjectState(nodes);



    foreach(QObject* obj,nodes)
    {
        QListWidgetItem *configButton = new QListWidgetItem(_contentsWidget);
        configButton->setIcon(icons.value(obj,QIcon()));
        configButton->setText(obj->objectName());
        configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        QPropertyEditorWidget* ew = new QPropertyEditorWidget(this);
        ew->addObject(obj);
        _pagesWidget->addWidget(ew);
    }
    connect(_contentsWidget,
                SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));

    _contentsWidget->setCurrentRow(0);



    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(_contentsWidget);
    horizontalLayout->addWidget(_pagesWidget, 1);

    setLayout(horizontalLayout);
    connect(this,SIGNAL(doUpdate()),this,SLOT(onUpdate()));
}

void PropertyStackedWidget::onUpdate()
{
    _pagesWidget->currentWidget()->update();
}

void PropertyStackedWidget::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
           current = previous;

    _pagesWidget->setCurrentIndex(_contentsWidget->row(current));
}
