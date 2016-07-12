#ifndef GENERAL_TRACKING_H
#define GENERAL_TRACKING_H

#include <sdk_global.h>
#include <opencv2/core/core.hpp>
#include <QTextStream>



namespace types
{
    typedef std::map<int,cv::Point3d> TrackedObjects;//Серийный номер объекта и его координаты
    typedef std::vector<cv::Point3d> Objects;//Список координат

    struct Object3d{//Полное описание 3д объекта
        cv::Point3d center;//Координаты центра объекта
        std::vector<cv::Point2i> contour;//Точки контура объекта
        std::map<std::string,float> params;//Дополнительные параметры
    };

    typedef std::map<int,Object3d> TrackedObjects3d;//Серийный номер объекта и его координаты
}

QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::TrackedObjects &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator<<(QTextStream &out, const types::TrackedObjects &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::Objects &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator<<(QTextStream &out, const types::Objects &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::TrackedObjects3d &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator<<(QTextStream &out, const types::TrackedObjects3d &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::Object3d &objects);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator<<(QTextStream &out, const types::Object3d &objects);
#endif // GENERAL_TRACKING_H
