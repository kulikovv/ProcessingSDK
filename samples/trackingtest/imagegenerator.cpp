#include "imagegenerator.h"
#include <types/rgb.h>

#include <QDebug>

using namespace types;
using namespace process;

ImageGenerator::ImageGenerator(QObject *parent) :
    ProcessNode(parent)
{
    //this->_widget = new visual::ui::QTableWidgetCopy(_invisibleParent);
    _width = 640;
    _height = 480;
    _canvas = cv::Mat(_width, _height, CV_8UC1);
    countObjects = 0;
    countArguments = 0;
}

void ImageGenerator::reset() {
    qDebug() << "ImageGenerator: reseting";
    _canvas = cv::Mat(_width, _height, CV_8UC1);
    countObjects = 0;
    countArguments = 0;
}

void ImageGenerator::receiveProcess(esp es) {
    QSharedPointer<types::DataSettings> dataset = es.getReadOnly<types::DataSettings>();
    if(!dataset.isNull()) {
        qDebug() << "ImageGenerator: catching dataset";
        countObjects = dataset->countObjects;
        countArguments = dataset->countArguments;
    }

    /*QSharedPointer<pData> pdata = es.getReadOnly<types::pData>();
    if(!pdata.isNull()) {
        for (int i = 0; i < countObjects; i++) {
            Data data = (*pdata)[i];
            //data[j]
            cv::circle( _canvas,
                        cv::Point(data[0],
                        data[1]),
                        10,
                        cv::Scalar( 0, 255, 0 ));
        }
        SEND(Rgb,new Rgb(_canvas))
    }*/

    QSharedPointer<Data> data = es.getReadOnly<types::Data>();
    if(!data.isNull()) {
        _canvas = cv::Mat(_width, _height, CV_8UC1);
        if (countObjects * countArguments == data->size()) {
            for (int i = 0; i < countObjects; i++) {
                cv::circle( _canvas,
                            cv::Point((*data)[i * countArguments ] * 50,
                            (*data)[i * countArguments + 1] * 50),
                            30, cv::Scalar( 255, 255, 255 ), -1);
            }
        } else {
            qDebug() << "ImageGenerator: (countObjects * countArguments != data->size())";
        }
        SEND(Rgb,new Rgb(_canvas))
    }
}

void ImageGenerator::matPrint() {
    for(int i = 0; i < _canvas.rows; i++) {
        QString s = QString::number(i) + ": ";
        for(int j = 0; j < _canvas.cols; j++) {
            s += QString::number(_canvas.at<float>(i, j)) + " ";
        }
        qDebug() << s;
    }
}
