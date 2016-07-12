#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include "PropertyEditor_global.h"

#include <QDialog>
#include <QMap>
#include <QIcon>

class PROPERTYEDITORSHARED_EXPORT PropertyDialog : public QDialog
{
    Q_OBJECT
public:
    enum DIALOG_TYPE
    {
        TABVIEW,
        STACKED,
        RESUME
    };

    explicit PropertyDialog(QList<QObject*> nodes,QMap<QObject*,QIcon> icons=QMap<QObject*,QIcon>(),DIALOG_TYPE dlg=TABVIEW, QWidget *parent = 0);

signals:

public slots:

};

#endif // PROPERTYDIALOG_H
