#ifndef PROCESSINGFRAMEWORK_DATA_H
#define PROCESSINGFRAMEWORK_DATA_H

#include <vector>
#include <QString>
#include <QStringList>
#include <sdk_global.h>
#include <stdexcept>
#include <math.h>
#include <cmath>

class QTextStream;

template<typename T>
bool is_nan( const T &value )
{
    // True if NAN
    return value != value;
}

namespace cv{
    class Mat;
}
/*! \brief Различные типы данных используемые в qptk
    Все различные данные используемые и обрабатываемые qptk следует обявлять здесь
  */
namespace types{

class PROCESSINGFRAMEWORK_SHARED_EXPORT unexpected_header: public std::runtime_error
{
public:
    unexpected_header():std::runtime_error("Unexpected header"){}
};

class PROCESSINGFRAMEWORK_SHARED_EXPORT unknown_index: public std::runtime_error
{
    public:
    unknown_index():std::runtime_error("Try to access unknown index"){}
};

class PROCESSINGFRAMEWORK_SHARED_EXPORT data_inconsistent: public std::runtime_error
{
public:
     data_inconsistent():std::runtime_error("The header and data are inconsistent"){}
};

class PROCESSINGFRAMEWORK_SHARED_EXPORT data_asynchronous: public std::runtime_error
{
public:
     data_asynchronous():std::runtime_error("The data is asynchronous"){}
};

class PROCESSINGFRAMEWORK_SHARED_EXPORT data_ambiguous: public std::runtime_error
{
public:
     data_ambiguous():std::runtime_error("Some merged data are ambiguous"){}
};

/*!
  \brief тип данных для научных исследований
  Вектор состоящий из чисел с плавающей точкой
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT LabeledData{
public:
    LabeledData(QStringList header, std::vector<float> data, const int ts=0);
    LabeledData(QStringList header,const int ts=0);
    LabeledData(const int ts=0);

    float &at(std::size_t idx);
    float &at(QString& name);
    float &at(const char* name);

    std::size_t size();
    float& operator[](std::size_t idx);
    const float& operator[](std::size_t idx) const;

    float& operator()(char* name){return at(name);}
    float& operator()(QString name){return at(name);}
  //  const float& operator[](const char* name) const {return at(name);}

    static LabeledData merge(LabeledData a,LabeledData b);
    static LabeledData merge(QStringList header,LabeledData a,LabeledData b);

    inline QStringList header();
    inline bool check();
    bool isempty();
    inline void insert(QString key,float value);

    typedef QStringList::iterator iterator;
    typedef QStringList::const_iterator const_iterator;
    iterator begin() { return _header.begin(); }
    iterator end() { return _header.end(); }

    const int frameId(){return _frameid;}
private:
    int _frameid;
    QStringList _header;
    std::vector<float> _data;
    bool _empty;
};

typedef LabeledData Data;

/*!
  \brief тип настроек данных
  Количество объектов и аргументов
  */
struct DataSettings {
    int countArguments;
    int countObjects;
    DataSettings() {
        this->countArguments = 0;
        this->countObjects = 0;
    }
    DataSettings(int countObjects, int countArguments) {
        this->countArguments = countArguments;
        this->countObjects = countObjects;
    }
};


/*!
  \brief массив из данных для научных исследований
  Вектор состоящий из векторов с плавающей точкой
  */

typedef std::map<int,types::Data> inner_container;
class PROCESSINGFRAMEWORK_SHARED_EXPORT DataContainer
{

public:
    inline void push_back(types::Data &t);
    typedef inner_container::iterator iterator;
    typedef inner_container::const_iterator const_iterator;
    iterator begin() { return _data.begin(); }
    iterator end() { return _data.end(); }
    std::size_t size() { return _data.size();}
    types::Data& operator[](std::size_t idx);
 //   const types::Data& operator[](std::size_t idx) const {return _data[idx];}

    void clear();
    QStringList header();
private:
    inner_container _data;
    QStringList _header;
};

typedef DataContainer pData;

/*!
  \brief перевод списка в строку для распечатки и просмотра
  Функция отладки
  \param data список всех значений
  \param legend список навзаний все значений
  \return строку со всеми значениями
  */
QString PROCESSINGFRAMEWORK_SHARED_EXPORT dataToString(Data &data,QStringList &legend);
QString PROCESSINGFRAMEWORK_SHARED_EXPORT matToString(cv::Mat mat);
cv::Mat PROCESSINGFRAMEWORK_SHARED_EXPORT stringToMat(QString str);
template<class T>
QString PROCESSINGFRAMEWORK_SHARED_EXPORT templateMatToString(cv::Mat mat);

QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::Data &data);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator<<(QTextStream &out,types::Data &data);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator>>(QTextStream &in, types::pData &data);
QTextStream PROCESSINGFRAMEWORK_SHARED_EXPORT &operator<<(QTextStream &out, types::pData &data);

}
#endif // PROCESSINGFRAMEWORK_DATA_H
