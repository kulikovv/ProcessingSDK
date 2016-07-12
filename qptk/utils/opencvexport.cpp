#include "opencvexport.h"
#include <opencv2/core/core.hpp>
using namespace utils::debug;

OpencvExport::OpencvExport(QObject *parent) :
    RawDataExport(parent){_extension="xml";}

void OpencvExport::saveData(QString name,cv::Mat img,QString varname)
{
    cv::FileStorage fs(QString(name+"."+_extension).toStdString(), cv::FileStorage::WRITE);
    fs << varname.toStdString() << img;
}
