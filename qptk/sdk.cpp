#include "sdk.h"
#include <QtPlugin>


#include <io/media/imagenode.h>
#include <io/media/videooutput.h>
#include <io/media/videosource.h>
#include <io/kinect/kinectplaynode.h>
#include <io/kinect/kinectoutnode.h>
#include <io/capture/opencvcameranode.h>
#include <io/capture/ipcamera.h>
#include <io/text/datasource.h>
#include <io/text/dataoutput.h>
#include <io/text/dataionode.h>
//#include <io/text/multiobjectdata.h>
#include <io/text/streamoutput.h>
#include <visual/imageview.h>
#include <visual/kinect/kinectrgb.h>
#include <visual/kinect/kinectdepth.h>
#include <visual/text/tableview.h>
//#include <visual/text/multiobjecttableview.h>
#include <visual/zoomimageview.h>
#include <visual/layoutnode.h>
//#include <visual/oglview.h>
#include <process/binarynode.h>
#include <process/resizenode.h>
#include <process/negative.h>
#include <process/fpslistener.h>
#include <workflow/adapter.h>
#include <workflow/parallel.h>
#include <synch/leadersynch.h>
#include <camera/calibrationnode.h>
#include <network/tcpabstractsender.h>
#include <utils/opencvexport.h>
#include <utils/matlabexport.h>

Sdk::Sdk(QObject *parent) : QObject(parent)
{

}

QString Sdk::getLibraryName()
{
    return "SDK Nodes";
}

void Sdk::registrate(QString filename)
{

    qRegisterMetaType<unsigned long>("base::timestamp");

    REGISTER(io::media::VideoOutput)
    REGISTER(io::media::ImageNode)
    REGISTER(io::media::VideoSource)
    REGISTER(io::kinect::KinectPlayNode)
    REGISTER(io::kinect::KinectOutNode)
    REGISTER(io::capture::OpenCVCameraNode)
    REGISTER(io::text::DataSource)
    REGISTER(io::text::DataOutput)
    REGISTER(io::text::DataIONode)
  //  REGISTER(io::text::MultiObjectData)
    REGISTER(io::text::StreamOutput)
    REGISTER(io::capture::IpCamera)
    REGISTER(visual::ImageView)
    REGISTER(visual::ZoomImageView)
    REGISTER(visual::kinect::KinectDepth)
    REGISTER(visual::kinect::KinectRGB)
    REGISTER(visual::text::TableView)
   // REGISTER(visual::text::MultiObjectTableView)
    REGISTER(visual::LayoutNode)
   // REGISTER(visual::OglView)
    REGISTER(process::BinaryNode)
    REGISTER(process::ResizeNode)
    REGISTER(process::Negative)
    REGISTER(process::FpsListener)
    REGISTER(workflow::Parallel)
    REGISTER(workflow::Adapter)
    REGISTER(synch::LeaderSynch)
    REGISTER(camera::CalibrationNode)
    REGISTER(network::TCPAbstractSender)
    REGISTER(utils::debug::MatlabExport)
    REGISTER(utils::debug::OpencvExport)

}
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(Sdk, Sdk)
#endif

