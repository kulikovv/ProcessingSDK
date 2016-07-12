#ifndef PROPERTYCONFIGURATIONWIDGET_H
#define PROPERTYCONFIGURATIONWIDGET_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QVariant>

typedef QPair<QString,QVariant> savedProperty;
typedef QList< savedProperty > propertyState;

class PropertyConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyConfigurationWidget(QWidget *parent = 0);
    ~PropertyConfigurationWidget();
signals:
    void doUpdate();
public slots:
    /*! Сохранить в память состояние объектов
      \param objects список объектов на сохранение
      */
    void storeObjectState(QList<QObject*> objects);
    //! Востановить состояние объектов из памяти
    void restoreObjectState();
private:
    QMap<QObject*,propertyState* > _objectStatus;
    
};

#endif // PROPERTYCONFIGURATIONWIDGET_H
