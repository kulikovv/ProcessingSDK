#include "calibrationnode.h"

#include <camera/calibration.h>
#include <visual/forms/calibrationdialog.h>
#include <QMessageBox>
#include <QDebug>
#include <types/rgb.h>
#include <types/data.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>

using namespace camera;


int CalibrationNode::gridx()
{
   return _calib.getGridSize().width;
}

int CalibrationNode::gridy()
{
    return _calib.getGridSize().height;
}

double CalibrationNode::meanReprojectionError() const
{
    return _calib.getMeanReprojectionError();
}

double CalibrationNode::squereSize()
{
    return _calib.getSquereSize();
}

CalibrationNode::pattern_type CalibrationNode::pattern() const
{
    return (CalibrationNode::pattern_type)_calib.getPatternType();
}
void CalibrationNode::setPattern(pattern_type pt)
{
    _calib.setPatternType(pt);
}

void CalibrationNode::setSquereSize(double value)
{
    if(value>0)
    {
        _calib.setSquereSize(value);
    }
}

bool CalibrationNode::useNormalization()
{
    return _normalization;
}

void CalibrationNode::setUseNormalization(bool value)
{
    _normalization = value;
    _calib.setNormalization(_normalization);
}

void CalibrationNode::setGridx(int value)
{
    if(value>0&&value<20)
    {
           int h = _calib.getGridSize().height;
           _calib.setGridSize(cv::Size(value,h));
    }
}

void CalibrationNode::setGridy(int value)
{
    if(value>0&&value<20)
    {
           int w = _calib.getGridSize().width;
           _calib.setGridSize(cv::Size(w,value));
    }
}

CalibrationNode::CalibrationNode(QObject *parent) :
    ProcessNode(parent)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

    _cameraReady = false;
    _dlg = 0;
    _doUndistortion = false;
    _disAdded = false;
    _camAdded = false;
    _normalization = true;
    setEnable(true);
}
CalibrationNode::~CalibrationNode()
{
    //TODO: Destruction	
    if(_dlg)
    {
        delete _dlg;
        _dlg=0;
    }
}
bool CalibrationNode::undistorsion()
{
    return _doUndistortion;
}
void CalibrationNode::makeFeed(cv::Mat data)
{
    if(0==_dlg)
        return;

    if(!_dlg->isVisible())
        return;

    cv::Mat feedImg;
    cv::resize(data,feedImg,cv::Size(320,240));
    emit sendFeed(feedImg);
}

void CalibrationNode::receiveProcess(esp data)
{
    QSharedPointer<types::Rgb> var = data.getReadOnly<types::Rgb>();
    if(!var.isNull())
    {
        //Backup data for getting
        _data = var->getMat();
        _cameraReady = true;
        //If undistorion is on - do so
       // if()
        makeFeed(_data);
        if(_doUndistortion)
        {
       //     qDebug() << "Do undistortion";
            cv::Mat clean = _calib.undistort(_data.clone());
            SEND(types::Rgb,new types::Rgb(clean))
        }
    }
}
void CalibrationNode::badResult()
{
    _calib.removeLast();
}
void CalibrationNode::calibrate()
{

        if(_calib.callibrate()){

            QMessageBox::information(0,tr("Info"),tr("Calibration successful!"));
            _doUndistortion = true;
            emit onCalibrated();
        }else{
             QMessageBox::warning(0,tr("Error"),tr("Calibration failed!"));
            _calib.clear();
        }
        if(_dlg)
        {
            _dlg->hide();
            delete _dlg;
            _dlg = 0;
        }


}

bool CalibrationNode::fromMemory()
{
    return false;
}

void CalibrationNode::setFromMemory(bool value)
{
    if(value)
    {
        QStringList pics = QFileDialog::getOpenFileNames(0,tr("Pictures with chessboard"),QString(),"Images (*.png *.xpm *.jpg *.bmp)");
        if(pics.size()>9)
        {
            _calib.clear();
            int goodcounter = 0;
            foreach(QString pic,pics)
            {
                _data = cv::imread(pic.toStdString());
                if(_calib.addImage(_data))
                {
                    goodcounter++;
                }
            }
            if(goodcounter>7)
            {
                calibrate();
            }else{
                QMessageBox::warning(0,tr("Error"),QString(tr("Chessboard has been found on %1 we need at least 7")).arg(goodcounter));
            }
        }else{
            QMessageBox::warning(0,tr("Error"),tr("Must have at least 9 images!"));
        }
    }
}



bool CalibrationNode::calibration() const
{
    return false;
}
QString CalibrationNode::distCoef()
{
    return types::matToString(_calib.getDistCoef());
}

void CalibrationNode::setDistCoef(QString dist)
{
    cv::Mat distCoef = types::stringToMat(dist);

    _calib.setDistCoef(distCoef);
    if(!distCoef.empty())
    {
        _disAdded = true;
        if(_camAdded)
        {
            _doUndistortion = true;
        }else{
             _doUndistortion = false;
        }
    }
}
bool CalibrationNode::calibrated() const
{
    return _doUndistortion;
}
QString CalibrationNode::intristics()
{

   // cv::Mat A=cv::Mat::eye(3,3,CV_32FC1);

    return types::matToString(_calib.getCamMatrix());
}


bool CalibrationNode::useResolutionReduction() const
{
    return _calib.getReduceResolution();
}
void CalibrationNode::setUseResolutionReduction(bool value)
{
    _calib.setReduceResolution(value);
}

float CalibrationNode::fovX() const
{
    if(_data.empty())
    {
        return -1;
    }
    if(_calib.getCamMatrix().empty())
    {
        return -1;
    }

    return 57.3*2*atan((_data.cols/2)/_calib.getCamMatrix().at<double>(0,0));
}
float CalibrationNode::fovY() const
{
    if(_data.empty())
    {
        return -1;
    }
    if(_calib.getCamMatrix().empty())
    {
        return -1;
    }

    return 57.3*2*atan((_data.rows/2)/_calib.getCamMatrix().at<double>(1,1));
}
void CalibrationNode::setCalibration(bool value)
{
    if(value)
    {
        try{
            if(!_cameraReady)
            {
                QMessageBox::warning(0,tr("Error"),tr("Connect to camera before calibration!"));
                return;
            }
            if(_dlg)
            {
                delete _dlg;
                _dlg = 0;
            }
            _dlg = new visual::ui::CalibrationDialog();

            connect(_dlg,SIGNAL(request()),this,SLOT(frameRequest()));
            connect(_dlg,SIGNAL(badgrid()),this,SLOT(badResult()));
            connect(_dlg,SIGNAL(callibrate()),this,SLOT(calibrate()));
            connect(this,SIGNAL(sendFeed(cv::Mat)),_dlg,SLOT(reciveSmallFeed(cv::Mat)));
            connect(this,SIGNAL(sendFrame(cv::Mat)),_dlg,SLOT(reciveFrame(cv::Mat)));
            _dlg->show();
        }catch(std::exception &ex){
            qDebug() << ex.what();
        }
    }
}
void CalibrationNode::setIntristics(QString intistics)
{
    cv::Mat cam = types::stringToMat(intistics);
    _calib.setCamMatrix(cam);
    if(!cam.empty())
    {
        _camAdded  = true;
        if(_disAdded)
        {
            _doUndistortion = true;
        }else{
            _doUndistortion = false;
        }
    }
}

void CalibrationNode::frameRequest()
{

    cv::Mat res;
    if(_cameraReady)
    {

        if(_data.empty())
        {
            qDebug() << "No data";
            return;
        }

        if(_calib.addImage(_data,res))
        {
            emit sendFrame(res);
        }

    }
}
