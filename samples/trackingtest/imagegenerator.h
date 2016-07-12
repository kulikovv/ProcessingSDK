#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <process/processnode.h>
#include <types/data.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace process;

class /*PROCESSINGFRAMEWORK_SHARED_EXPORT*/ ImageGenerator : public ProcessNode
{
//Q_OBJECT
public:
    explicit ImageGenerator(QObject *parent=0);
    virtual void reset();
protected:
    virtual void receiveProcess(esp data);
    cv::Mat _canvas;
    int countObjects;
    int countArguments;
    void matPrint();
    int _width;
    int _height;

};

#endif // IMAGEGENERATOR_H
