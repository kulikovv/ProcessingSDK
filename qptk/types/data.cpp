#include "data.h"
#include <QVector>
#include <QStringList>
#include <opencv2/core/core.hpp>
#include <types/general_tracking.h>
#include <QDebug>
#include <limits>

QString types::dataToString(Data &data,QStringList &legend)
{
    QString string;
    for(unsigned int i=0;i<data.size();i++)
    {
        if(legend.size()>i )
        {
            string = string + QString("%1:").arg(legend[i]);
        }
        string = string + QString(" %1").arg(data[i]);
    }
    return "["+string+"]";
}

QString types::matToString(cv::Mat mat)
{
    if(mat.empty())
    {
        return "";
    }
    switch (mat.type()) {
    case CV_32F :
        return templateMatToString<float>(mat);
        break;
    case CV_64F :
        return templateMatToString<double>(mat);
        break;
    case CV_8U :
        return templateMatToString<unsigned char>(mat);
        break;
    default:
        qDebug() << "Error: unknowing type\n";
        return "";
    }
}

template<class T>
QString types::templateMatToString(cv::Mat mat) {
    QString str;
    str+= QString("%1,%2;").arg(mat.rows).arg(mat.cols);
    for(int i=0;i<mat.rows;i++)
    {
        for(int j=0;j<mat.cols;j++)
        {
            str+= QString("%1").arg(mat.at<T>(i,j));
            if(j+1<mat.cols)
            {
               str+= QString(",");
            }
        }
        str+= QString(";");
    }
    return str;
}


cv::Mat types::stringToMat(QString str)
{
    if(str.isEmpty())
    {
        return cv::Mat();
    }
    str.remove(" ");
    QStringList data = str.split(";",QString::SkipEmptyParts);
    QString sizes = data.first();
    bool isok;
    QStringList sizes_list = sizes.split(",",QString::SkipEmptyParts);
    if(2!=sizes_list.size())
    {
        return cv::Mat();
    }


    cv::Mat A(sizes_list[0].toInt(&isok),sizes_list[1].toInt(&isok),CV_64FC1,cv::Scalar::all(0));

    data.removeFirst();
    for(int i=0;i<data.size();i++)
    {
        QStringList row_list = data[i].split(",",QString::SkipEmptyParts);
        for(int j=0;j<row_list.size();j++)
        {

           double x = row_list[j].toDouble(&isok);
           if(!isok)
           {
               return cv::Mat();
           }
           A.at<double>(i,j) = x;
        }
    }
    return A;
}


//QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::TrackedObjects &objects);
QTextStream &operator<<(QTextStream &out, const types::TrackedObjects &objects)
{
    for (types::TrackedObjects::const_iterator iter = objects.begin(); iter != objects.end(); ++iter) {
        out <<" "<< iter->first <<" "<< iter->second.x  <<" "<< iter->second.y <<" "<< iter->second.z << ";";
    }
    return out;
}

//QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::Objects &objects);
QTextStream  &operator<<(QTextStream &out, const types::Objects &objects){
    for (types::Objects::const_iterator iter = objects.begin(); iter != objects.end(); ++iter) {
        out <<" "<< iter->x  <<" "<< iter->y <<" "<< iter->z << ";";
    }
    return out;
}

//QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::TrackedObjects3d &objects);
QTextStream  &operator<<(QTextStream &out, const types::TrackedObjects3d &objects)
{

    for (types::TrackedObjects3d::const_iterator iter = objects.begin(); iter != objects.end(); ++iter) {
        out << iter->first  <<" "<< iter->second << ";";
    }
    return out;
}

//QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::Object3d &objects);
QTextStream  &operator<<(QTextStream &out, const types::Object3d &objects)
{

    out <<" "<< objects.center.x  <<" "<< objects.center.y <<" "<< objects.center.z;

    return out;
}



QTextStream &types::operator>>(QTextStream &in, types::Data &data)
{
    QString line = in.readLine();
    if(line.isEmpty())
        throw -1;


    QStringList part = line.split("\t");

    if(part.size()!=data.header().size()+1)
        throw types::data_inconsistent();

    types::Data newData(data.header(),part[0].toInt());

    for(int i=1;i<part.size();i++){
        newData[i-1]=part[i].toFloat();
    }
    data = newData;
    return in;
}
QTextStream &types::operator << (QTextStream &out, types::Data  &data){
    if(data.isempty())
        return out;
    out << data.frameId();
    for(int i=0;i<data.size();i++){
       out <<"\t" << data[i] ;
    }
    out << "\n";
    return out;
}

QTextStream &types::operator>>(QTextStream &in, types::pData &data)
{
    QString line = in.readLine();//Read header

    QStringList header = line.split("\t", QString::SkipEmptyParts);
    int number = header.indexOf("#");
    header.removeOne("#");

    types::pData newData = types::pData();
    try{
        int t=0;
    while(true){

        types::Data d(header);
        in >> d;
        newData.push_back(d);
    }
    }catch(types::data_inconsistent inc){
        throw inc;
    }catch(...){

    }
    data = newData;
    return in;
}

QTextStream &types::operator<<(QTextStream &out, types::pData &data)
{
    out << "#"<< "\t" << data.header().join("\t")<<"\n";
    for (std::map<int,types::Data>::iterator it=data.begin(); it!=data.end(); ++it)
    {
        types::Data chunk = it->second;
        if(data.header()!=chunk.header())
        {
            chunk = types::Data::merge(data.header(),types::Data(),chunk);
        }
        out << chunk;

    }


    return out;

}
types::Data& types::DataContainer::operator[](std::size_t idx)
{
    return _data[idx];
}

QStringList types::LabeledData::header()
{
    return _header;
}

std::size_t types::LabeledData::size(){
        return _data.size();
}
types::LabeledData::LabeledData(QStringList header, std::vector<float> data, const int ts)
{
    if(header.size()!=data.size())
        throw types::data_inconsistent();

    _header = header;
    _data = data;
    _frameid = ts;
    _empty = false;

}
types::LabeledData::LabeledData(const int ts){
    _frameid = ts;
    _empty = true;
}
bool types::LabeledData::check(){
   /* if(_empty){
        throw types::data_inconsistent();
    }*/
    if(_header.size()!=_data.size()){
        throw types::data_inconsistent();
    }
}

bool types::LabeledData::isempty()
{
    return _empty;
}
void types::LabeledData::insert(QString key,float value)
{
    int idx = _header.indexOf(key);
    if(-1==idx){
        _header.push_back(key);
        _data.push_back(value);
    }else{
        _data[idx]=value;
    }
    _empty = false;
}

types::LabeledData::LabeledData(QStringList header,const int ts)
{
    _frameid=ts;
    _header = header;
    _data.resize(_header.size(),std::numeric_limits<float>::quiet_NaN());
     _empty = false;

}

float &types::LabeledData::at(std::size_t idx){
    return _data[idx];
}
float &types::LabeledData::at(const char* name)
{
    return at(QString(name));
}

float &types::LabeledData::at(QString& name){

    int idx = _header.indexOf(name);
    if(-1==idx)
        throw types::unknown_index();
    return _data[idx];
}

float& types::LabeledData::operator[](std::size_t idx)
{ return _data[idx]; }
const float& types::LabeledData::operator[](std::size_t idx) const
{ return _data[idx]; }
types::LabeledData types::LabeledData::merge(LabeledData a,LabeledData b)
{
    return types::LabeledData::merge(QStringList(),a,b);
}

types::LabeledData types::LabeledData::merge(QStringList header,types::LabeledData a,types::LabeledData b){

        if(!a.isempty()&&!b.isempty())
        {
            if(a.frameId()!=b.frameId())
            {
                throw types::data_asynchronous();
            }
        }

        a.check();
        b.check();

        int ts = a.frameId();
        if(a.isempty()){
            ts = b.frameId();
        }

        types::LabeledData ab(header,ts);

        types::LabeledData(a.frameId());

        foreach(QString h,a.header()){
            ab.insert(h,a(h));
        }

        foreach(QString h,b.header()){
            ab.insert(h,b(h));
        }

        return ab;
    }
void types::DataContainer::push_back(types::Data &t)
{
    if(t.isempty()){
        return;
    }

    if(_header.isEmpty()){
        _header = t.header();
    }


  //  types::Data merged = t;
   // if(!_data[t.frameId()].isempty())
 //   {
        types::Data merged = types::Data::merge(_header,_data[t.frameId()],t);
        if(_header!=merged.header()){
                _header=merged.header();
        }
   // }



    _data[t.frameId()]=merged;
}
QStringList types::DataContainer::header(){
    return _header;
}

void types::DataContainer::clear(){
    _header.clear();
    _data.clear();
}
