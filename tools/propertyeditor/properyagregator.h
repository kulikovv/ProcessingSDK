#ifndef PROPERYAGREGATOR_H
#define PROPERYAGREGATOR_H

#include <QObject>
#include <QMap>
#include "PropertyEditor_global.h"

class PROPERTYEDITORSHARED_EXPORT ProperyAgregator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool test MEMBER _test)
public:
    explicit  ProperyAgregator(QList<QObject*> nodes,QObject *parent = 0);
    void accept();
signals:

public slots:
private:
    bool _test;
    QMap<QString,QObject*> _ppry;
};

#endif // PROPERYAGREGATOR_H
