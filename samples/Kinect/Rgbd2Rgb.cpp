#include "Rgbd2Rgb.h"
#include <types/Rgbd.h>
using namespace types;

Rgbd2Rgb::Rgbd2Rgb(QObject* parent):process::ProcessNode(parent)
{
    setEnable(true);
}

QIcon Rgbd2Rgb::getIcon()
{
    return QIcon(":/kinectlibrary/Rgbd2Rgb");
}

void Rgbd2Rgb::receiveProcess(esp data)
{
    QSharedPointer<Rgbd> mat = data.getReadOnly<Rgbd>();
    if(!mat.isNull())
    {
        SEND(Rgb,new Rgb(mat->rgb().getMat().clone()))
    }
}
