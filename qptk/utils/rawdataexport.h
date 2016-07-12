#ifndef RAWDATAEXPORT_H
#define RAWDATAEXPORT_H

#include <io/outputnode.h>
namespace cv{
    class Mat;
}
namespace utils{
    namespace debug{


    class RawDataExport : public io::OutputNode
    {
        Q_OBJECT
        Q_PROPERTY(QString url READ url WRITE setUrl USER true)
    public:
        explicit RawDataExport(QObject *parent);
        ~RawDataExport();
    protected:

        /*! Call from other Nodes store your data here
          * \esp data container
          * \sa io::Node , EthoStudioSPointer
          */
        virtual void receiveProcess(esp data);
        virtual void saveData(QString name,cv::Mat img,QString varname)=0;
    };
        /*class MatlabExport : public RawDataExport
        {
                Q_OBJECT
            public:
                explicit MatlabExport(QObject *parent);
                ~MatlabExport();
            protected:
                virtual void saveData(QString name,cv::Mat img,QString varname);
                void writeMat( cv::Mat const& mat, const char* filename, const char* varName="RGB", bool bgr2rgb=true );
        };*/




    }
}

#endif // RAWDATAEXPORT_H
