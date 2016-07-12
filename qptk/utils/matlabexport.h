#ifndef MATLABEXPORT_H
#define MATLABEXPORT_H

#include <utils/rawdataexport.h>
namespace utils{
namespace debug{


class MatlabExport : public RawDataExport
{
    Q_OBJECT
public:
    explicit MatlabExport(QObject *parent);
    ~MatlabExport();
    static void writeMat( cv::Mat const& mat, const char* filename, const char* varName="RGB", bool bgr2rgb=true );
protected:
    virtual void saveData(QString name,cv::Mat img,QString varname);


    
};

}
}

#endif // MATLABEXPORT_H
