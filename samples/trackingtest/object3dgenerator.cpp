#include "object3dgenerator.h"
#include <math.h>

#include <QDebug>

Object3dGenerator::Object3dGenerator(QObject *parent) :
    SourceNode(parent)
{
    setFps(30);
}

esp Object3dGenerator::get(const base::timestamp frame) {
    SEND(Object3d, genObject3d(frame))
    return esp();
}

Object3d* Object3dGenerator::genObject3d(base::timestamp frame) {
    Object3d* obj;
    obj->center.x = cos((double)frame / 20) + 2;
    obj->center.y = sin((double)frame / 20) + 2;
    obj->center.z = cos((double)frame / 20) + 2 + 1 * 0.1;
    obj->params.insert( std::pair<std::string, float>
                       ("Arg1", sin((double)frame / 20) + 2 + 1 * 0.1) );
    return obj;
}
