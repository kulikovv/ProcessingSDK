#ifndef PROCESSINGFRAMEWORK_ESP_H
#define PROCESSINGFRAMEWORK_ESP_H

#include <QSharedPointer>
#include <typeinfo>
#include <sdk_global.h>

#define SEND(T,Ptr) presend(esp::init<T>(Ptr,frame,""));//emit send(esp::init<T>(Ptr,frame,""));
#define SEND_TAG(T,Ptr,Tag) presend(esp::init<T>(Ptr,frame,Tag)); //emit send(esp::init<T>(Ptr,frame,Tag));

namespace base
{
typedef unsigned long timestamp;//!< Тип данных для номера кадра
class ConvertFactory;
class PROCESSINGFRAMEWORK_SHARED_EXPORT Esp
{
private:
    class PROCESSINGFRAMEWORK_SHARED_EXPORT Container
    {
    public:
        Container(void* ptr,void (*releaseFunc)(void*));
        ~Container();

        void (*_releaseFunc)(void*);
        void* _ptr;

    };

    Esp(QString type,Container* data,timestamp frame=0,QString tag="");

    template<typename T> static void autoDelete(void* data)
    {
        if(0!=data)
        {
            T* item = (T*)data;
            delete item;
        }
    }
    template<typename T> static void donotDelete(T* data) {Q_UNUSED(data);}

 public:
    Esp();

    template<typename T> static Esp init(T* data,timestamp frame=0,QString tag="")
    {
        return Esp(typeid(T).name(),new Container((void*)data,Esp::autoDelete<T>),frame,tag);
    }
    template<typename T> static Esp init(T* data,void (*releaseFunc)(void*),timestamp frame=0,QString tag="")
    {
        return Esp(typeid(T).name(),new Container((void*)data,releaseFunc),frame,tag);
    }
    /*! \warning данные, возвращенные умным указателем живут столько сколько живет esp т.е. их контейнер.
     *  \brief Функция позволяет привести к нужному типу указатель, если это возможно.
     */
    template<typename T> QSharedPointer<T> getReadOnly()
    {      
        if(0!=QString::compare(_type,typeid(T).name()))
        {
          /*  void* temp = convert(typeid(T).name());
            if(0!=temp)
            {
                return QSharedPointer<T>((T*)temp,Esp::autoDelete<T>);
            }*/
            return QSharedPointer<T>(0);
        }
        return QSharedPointer<T>((T*)_data->_ptr,Esp::donotDelete<T>);
    }
    /*!
     * \brief setTag установить метку на данные. Эта функция очень полезна, когда узлам могуть приходить данные одного типа, но которые должны обрабатываться по разному
     * \param tag
     */
    void setTag(QString tag);
    /*!
     * \brief getFrameId получить серийный номер кадра
     * \return  возвращает номер текущего кадра
     */
    timestamp getFrameId() const;
    /*!
     * \brief operator == провека соответсвия метки с заданной строкой
     * \param rhs
     * \return
     */
    bool operator==(const QString &rhs) const;
    bool operator!=(const QString &other) const;
    static void registerConvertor(ConvertFactory* cv);
private:
    void* convert(std::string type);

    QSharedPointer<Container> _data;//!< безтипный контэйнер данных
    timestamp _timestamp;//!< номер кадра
    QString _type;//!< строковое оприсание типа данных
    QString _tag;//!< собственно
};
}
typedef class base::Esp esp;
#endif // ESP_H
