#include "multiobjectdata.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <utils/logs.h>
#include <exception>

using namespace io;
using namespace io::text;
using namespace types;

MultiObjectData::MultiObjectData(QObject *parent) :
    InoutNode(parent)
{
    _extension = "txt";
    _separator = "\t";
    setEnable(true);

    constMatSize = 50;
    matSize = constMatSize;
    matStepSize = 10;
    emptyValue = -1;

    countArguments = 3;
    countObjects = 3;    
}

MultiObjectData::~MultiObjectData()
{
    clear();
}
void MultiObjectData::play(esp data)
{
    get(data.getFrameId());
}

void MultiObjectData::reset()
{
    qDebug() << "MultiObjectData: Call reset";
    _extension = "txt";
    _separator = "\t";
    setEnable(true);
    clear();
    InoutNode::reset();
}

esp MultiObjectData::get(const base::timestamp frame)
{
    //INFO("Get")
    esp pointer;
    try {
        if ((frame >= 0) && (frame < _mat.rows)) {
            pointer = esp::init(new Data(rowMatToData(_mat, frame)), frame);
            emit send(pointer);
        } else {
            qDebug() << "MultiObjectData: Exception: incorrect value of frameID: " << frame;
        }
    } catch(...) {
        qDebug() << "MultiObjectData: Exception";
    }
    return pointer;
}
void MultiObjectData::record(esp data)
{    
    if (!recording()) {
        qDebug() << "MultiObjectData: !recording()";
        return;
    }
    //INFO("Record")
    if (_mat.empty()) {
        qDebug() << "MultiObjectData: _mat.empty()";
        _mat = cv::Mat(matSize, countArguments * countObjects, CV_32F);
        for (int i = 0; i < matSize; i++) {
            _mat.row(i).setTo(cv::Scalar(emptyValue));
        }
    }

    if (data.getFrameId() < 0) {
        qDebug() << "MultiObjectData: Exeption: incorrect value of data.getFrameId(): " << data.getFrameId();
        return;
    }

    QSharedPointer<Data> res = data.getReadOnly<Data>();
    try {
        if (!res.isNull()) {
            dataToRowMat(*res, &_mat, data.getFrameId());
            SEND(types::Data, new types::Data( *res))
        }
    }
    catch(std::exception ex) {
        CRITICAL(QString("MultiObjectData: Exeption recived %1").arg(ex.what()))
    }

    QSharedPointer<pData> pres = data.getReadOnly<pData>();
    try {
        if (!pres.isNull()) {
            pdataToRowMat(*pres, &_mat, data.getFrameId());
            SEND(types::pData, new types::pData( *pres))
        }
    }
    catch(std::exception ex) {
        CRITICAL(QString("MultiObjectData: Exeption recived %1").arg(ex.what()))
    }

    QSharedPointer<Object3d> obj3d = data.getReadOnly<Object3d>();
    try {
        if (!obj3d.isNull()) {
            //
        }
    }
    catch(std::exception ex) {
        CRITICAL(QString("MultiObjectData: Exeption recived %1").arg(ex.what()))
    }
}

bool MultiObjectData::save(QString path)
{
    //INFO(QString("Saving to %1 %2 rows").arg(path).arg(_data.size()))
    //Open file and store data
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return false;
    QTextStream stream(&file);    
    for(int i = 0; i < _mat.rows; i++) {
        for(int j = 0; j < _mat.cols; j++) {
            stream << _mat.at<float>(i, j) << _separator;
        }
        stream << "\n";
    }
    file.close();

    /*cv::FileStorage f;
    f.open("C:/Coding/test7.txt", cv::FileStorage::WRITE);
    if (!f.isOpened()) {
        qDebug() << "Error: failed to open file";
        //return false;
    }
    f << "M" << _mat2;
    f.release();*/

    return true;
}


bool MultiObjectData::load(QString path)
{
   // INFO(QString("Loading... %1").arg(path))
    //Open file and copy data
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    clear();
    QTextStream stream(&file);
    QStringList cols;
    QString line;
    int k = 0;
    _length = 0;
    if (_mat.empty()) {
        qDebug() << "MultiObjectData: _mat.empty()";
        _mat = cv::Mat(matSize, countArguments * countObjects, CV_32F);
        for (int i = 0; i < matSize; i++) {
            _mat.row(i).setTo(cv::Scalar(emptyValue));
        }
    }
    line = stream.readLine();
    while(!line.isEmpty()) {
        if (k >= matSize) {
            matSize += matStepSize;            
            _mat.resize(matSize);
            for (int i = k; i < matSize; i++) {
                _mat.row(i).setTo(cv::Scalar(emptyValue));
            }
        }
        cols = line.split(_separator, QString::SkipEmptyParts);
        if (countArguments * countObjects != cols.size()) {
            qDebug() << "MultiObjectData: Error: incorrect number of arguments of line from file";
            return false;
        }
        for (int i = 0; i < countObjects; i++) {
            for (int j = 0; j < countArguments; j++) {
                _mat.at<float>(k, i * countArguments + j) = cols[i * countArguments + j].toFloat();
            }
        }
        k++;
        line = stream.readLine();
    }
    file.close();

    /*cv::FileStorage f;
    f.open("C:/Coding/test7.txt", cv::FileStorage::READ);
    if (!f.isOpened()) {
        qDebug() << "Error: failed to open file";
        //return false;
    }
    f["M"] >> _mat2;
    qDebug() << "!!!!" << matToString(_mat2);*/

    //INFO("Send full data")
    onConnected();
    //INFO(QString("Reading end %1").arg(_length))
    return true;
}

void MultiObjectData::onConnected()
{    
    if(_mat.rows > 0) {
        pData* copy = new pData();
        for (int i = 0; i < _mat.rows; i++) {
            copy->push_back(rowMatToData(_mat, i));
        }
        INFO("Send full data")
        SEND(pData, copy)
    }

    qDebug() << "MultiObjectData: sending dataset";
    SEND(types::DataSettings, new types::DataSettings(countObjects, countArguments))
}

void MultiObjectData::clear()
{
    INFO("Clean started")
    matSize = constMatSize;
    _mat = cv::Mat(matSize, countArguments * countObjects, CV_32F);
    for (int i = 0; i < matSize; i++) {
        _mat.row(i).setTo(cv::Scalar(emptyValue));
    }
    SEND(Data, new Data())
    INFO("Clean finished")
}

types::Data MultiObjectData::rowMatToData(cv::Mat mat, int k) {
    Data data;
    for (int i = 0; i < mat.cols; i++) {
        data.insert("",mat.at<float>(k, i));
    }
    return data;
}

void MultiObjectData::dataToRowMat(Data data, cv::Mat *mat, int k) {
    if (k >= matSize) {
        int x = matSize;
        matSize += matStepSize;
        mat->resize(matSize);
        for (int i = x; i < matSize; i++) {
            _mat.row(i).setTo(cv::Scalar(emptyValue));
        }
    }
    if (data.size() != mat->cols) {
        //incorrect number of arguments in data
        qDebug() << "MultiObjectData: Exception: (data.size() != mat->cols)";
    }
    for (int i = 0; i < mat->cols; i++) {
        if (i < data.size()) {
            mat->at<float>(k, i) = data.values()[i];
        } else {
            mat->at<float>(k, i) = emptyValue;
        }
    }
}

void MultiObjectData::pdataToRowMat(pData pdata, cv::Mat *mat, int k) {
    if (k >= matSize) {
        int x = matSize;
        matSize += matStepSize;
        mat->resize(matSize);
        for (int i = x; i < matSize; i++) {
            _mat.row(i).setTo(cv::Scalar(emptyValue));
        }
    }
    if (pdata.size() != countObjects) {
        //incorrect number of arguments in pdata
        qDebug() << "MultiObjectData: Exception: (pdata.size() != countObjects)";
    }
    for (int i = 0; i < countObjects; i++) {
        if (i < pdata.size()) {
            if (!pdata[i].isEmpty()) {
                if (pdata[i].size() != countArguments) {
                    qDebug() << "MultiObjectData: Exception: (pdata[" << i << "].size() != countArguments)";
                }
                for (int j = 0; j < countArguments; j++) {
                    if (j < pdata[i].size()) {
                        mat->at<float>(k, i * countArguments + j) = pdata[i].values()[j];
                    } else {
                        mat->at<float>(k, i * countArguments + j) = emptyValue;
                    }
                }
            } else {
                for (int j = 0; j < countArguments; j++) {
                    mat->at<float>(k, i * countArguments + j) = emptyValue;
                }
            }
        } else {
            for (int j = 0; j < countArguments; j++) {
                mat->at<float>(k, i * countArguments + j) = emptyValue;
            }
        }
    }
}

void MultiObjectData::matPrint() {
    for(int i = 0; i < _mat.rows; i++) {
        QString s = QString::number(i) + ": ";
        for(int j = 0; j < _mat.cols; j++) {
            s += QString::number(_mat.at<float>(i, j)) + " ";
        }
        qDebug() << s;
    }
}

