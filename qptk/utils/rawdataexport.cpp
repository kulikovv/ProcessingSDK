#include "rawdataexport.h"

#include <types/rgbd.h>
#include <types/multimap.h>
#include <opencv2/core/core.hpp>


using namespace utils::debug;

RawDataExport::RawDataExport(QObject *parent) :
    io::OutputNode(parent)
{
    _url = "%1";
}

/*MatlabExport::MatlabExport(QObject *parent) :
    RawDataExport(parent){_extension="m";}
MatlabExport::~MatlabExport(){}*/

RawDataExport::~RawDataExport(){}

void RawDataExport::receiveProcess(esp data)
{
    QString local=_url.arg(data.getFrameId(),4,10,QChar('0'));

    QSharedPointer<types::Rgb> rgb = data.getReadOnly<types::Rgb>();
    QSharedPointer<types::Rgbd> rgbd = data.getReadOnly<types::Rgbd>();
    QSharedPointer<types::MultiMap> map = data.getReadOnly<types::MultiMap>();

    if(!rgb.isNull()){
        saveData(local+"_rgb",rgb->getMat(),QString("RGB%1").arg(data.getFrameId(),4,10,QChar('0')));
    }

    if(!rgbd.isNull()){
        saveData(local+"_rgb",rgbd->rgb().getMat(),QString("RGB%1").arg(data.getFrameId(),4,10,QChar('0')));
        saveData(local+"_depth",rgbd->depth().getMat(),QString("DEPTH%1").arg(data.getFrameId(),4,10,QChar('0')));
    }

    if(!map.isNull()){
        foreach(QString s,map->keys()){
            saveData(local+s,map->value(s,cv::Mat()),QString(s+"%1").arg(data.getFrameId(),4,10,QChar('0')));
        }
    }
}


/*

*/
