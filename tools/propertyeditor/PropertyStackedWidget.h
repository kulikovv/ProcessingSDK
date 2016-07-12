#ifndef PROPERTYSTACKEDWIDGET_H
#define PROPERTYSTACKEDWIDGET_H

#include "PropertyConfigurationWidget.h"
#include "PropertyEditor_global.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class PROPERTYEDITORSHARED_EXPORT PropertyStackedWidget : public PropertyConfigurationWidget
{
    Q_OBJECT
public:
    explicit PropertyStackedWidget(QList<QObject*> nodes,QMap<QObject*,QIcon> icons,QWidget *parent = 0);
    
signals:
    
public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private slots:
    void onUpdate();
private:
    QListWidget *_contentsWidget;
    QStackedWidget *_pagesWidget;
};

#endif // PROPERTYSTACKEDWIDGET_H
