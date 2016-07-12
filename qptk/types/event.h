#ifndef PROCESSINGFRAMEWORK_EVENT_H
#define PROCESSINGFRAMEWORK_EVENT_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QDataStream>

#include <opencv2/core/core.hpp>
#include <types/rgb.h>
#include <types/serializable.h>

QDataStream &operator>>(QDataStream &in, cv::Mat &image);
QDataStream &operator<<(QDataStream &out, const cv::Mat &image);
QDataStream &operator>>(QDataStream &in, cv::Point3f &point);
QDataStream &operator<<(QDataStream &out, const cv::Point3f &point);
QDataStream &operator>>(QDataStream &in, cv::Rect_<float> &rect);
QDataStream &operator<<(QDataStream &out, const cv::Rect_<float> &rect);
QDataStream &operator>>(QDataStream &in, types::Rgb &rgb);
QDataStream &operator<<(QDataStream &out, const types::Rgb &rgb);

namespace types
{
    /*! Класс описывающ
      */
    class PROCESSINGFRAMEWORK_SHARED_EXPORT Event: public Serializable
    {
    public:
        Event();
        Event(const Event &another);
        Event(const cv::Point3f &center,
              const cv::Rect_<float> &sizes,
              const QString &eventType,
              const QString &eventSource,
              const QList<Rgb> &images = QList<Rgb>());
        Event &operator=(const Event &another);

        //! getters
        const cv::Point3f &center() const;
        const cv::Rect_<float> &sizes() const;
        const QString &eventType() const;
        const QString &eventSource() const;
        const QList<Rgb> &images() const;
        //! setters
        void setCenter(const cv::Point3f &);
        void setSizes(const cv::Rect_<float> &);
        void setEventType(const QString &);
        void setEventSource(const QString &);
        void setImages(const QList<Rgb> &);

        QByteArray serialize() /*const*/;
        static Event deserialize(const QByteArray &data);

    private:
        cv::Point3f _center;//!< Центр масс объекта
        cv::Rect_<float> _sizes;//!< Размеры объекта в реальных единицах
        QString _eventType;//!< Тип события
        QString _eventSource;//!< Источник события
        QList<Rgb> _images;//!< Изображения
    };
    typedef QList<Event> EventList; //! По факту надо будет передавать список событий
}

QDataStream &operator>>(QDataStream &in, types::Event &event);
QDataStream &operator<<(QDataStream &out, const types::Event &event);

#endif // PROCESSINGFRAMEWORK_EVENT_H
