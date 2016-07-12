#ifndef OBJECT3DGENERATOR_H
#define OBJECT3DGENERATOR_H

#include <QObject>
#include <io/sourcenode.h>
#include <types/general_tracking.h>

using namespace io;
using namespace types;

class Object3dGenerator : public SourceNode
{
    Q_OBJECT
public:
    explicit Object3dGenerator(QObject *parent = 0);

    virtual esp get(const base::timestamp frame);

private:

    Object3d* genObject3d(base::timestamp frame);
};

#endif // OBJECT3DGENERATOR_H
