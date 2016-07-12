#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include "%ProjectName:c%_global.h"

#include <QObject>
#include <interfaces.h>

class %ProjectName:u%SHARED_EXPORT %ProjectName:c% : public QObject,NodeLibrary
{
    Q_OBJECT
    Q_INTERFACES(NodeLibrary)
public:
    %ProjectName:c%();
    virtual QString getLibraryName();
    virtual void registrate();
};

#endif // %ProjectName:u%_H
