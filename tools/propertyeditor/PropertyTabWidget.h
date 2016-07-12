#ifndef PROPERTYTAB_H
#define PROPERTYTAB_H

#include "PropertyEditor_global.h"
#include "PropertyConfigurationWidget.h"

class QTabWidget;


/*! \brief Класс для настройки систем основанные на нашей технологии
  Создает Widget закладок, которой умеет сохранять из загружать состояние объектов
  */
class PROPERTYEDITORSHARED_EXPORT PropertyTabWidget : public PropertyConfigurationWidget
{
    Q_OBJECT
public:
    /*! Создает Tab с закладкой для каждого объекта из списка.
      \param nodes список объектов
      */
    explicit PropertyTabWidget(QList<QObject*> nodes,QMap<QObject*,QIcon> icons,QWidget *parent = 0);
    ~PropertyTabWidget();
private slots:
    void onUpdate();
signals:

private:
    QTabWidget* _tabWidget;
};

#endif // PROPERTYTAB_H
