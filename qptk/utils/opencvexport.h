#ifndef OPENCVEXPORT_H
#define OPENCVEXPORT_H
#include <utils/rawdataexport.h>
namespace utils{
    namespace debug{
        class OpencvExport : public RawDataExport
        {
            Q_OBJECT
        public:
            explicit OpencvExport(QObject *parent = 0);

        protected:
             virtual void saveData(QString name,cv::Mat img,QString varname);

        };
    }
}
#endif // OPENCVEXPORT_H
