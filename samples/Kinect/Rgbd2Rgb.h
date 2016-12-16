#ifndef Rgbd2Rgb_H
#define Rgbd2Rgb_H

#include <process/processnode.h>
#include "kinectlibrary_global.h"

class KINECTLIBRARYSHARED_EXPORT Rgbd2Rgb: public process::ProcessNode
{
	Q_OBJECT
public:
	explicit Rgbd2Rgb(QObject *parent);
public slots:
    QIcon getIcon();
protected:
	void receiveProcess(esp data);
};

#endif
