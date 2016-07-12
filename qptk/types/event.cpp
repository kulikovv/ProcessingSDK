#include <QtCore/QIODevice>

#include "event.h"

using namespace types;

Event::Event()
{
}

Event::Event(const Event &another)
    : _center(another._center),
      _sizes(another._sizes),
      _eventType(another._eventType),
      _eventSource(another._eventSource),
      _images(another._images)
{
}

Event::Event(const cv::Point3f &center,
      const cv::Rect_<float> &sizes,
      const QString &eventType,
      const QString &eventSource,
      const QList<Rgb> &images)
          : _center(center),
            _sizes(sizes),
            _eventType(eventType),
            _eventSource(eventSource),
            _images(images)
{
}

Event &Event::operator=(const Event &another)
{
    _center = another._center;
    _sizes = another._sizes;
    _eventType = another._eventType;
    _eventSource = another._eventSource;
    _images = another._images;
    return *this;
}

const cv::Point3f &Event::center() const
{
    return _center;
}

const cv::Rect_<float> &Event::sizes() const
{
    return _sizes;
}

const QString &Event::eventType() const
{
    return _eventType;
}

const QString &Event::eventSource() const
{
    return _eventSource;
}

const QList<Rgb> &Event::images() const
{
    return _images;
}

void Event::setCenter(const cv::Point3f &value)
{
    _center = value;
}

void Event::setSizes(const cv::Rect_<float> &value)
{
    _sizes = value;
}

void Event::setEventType(const QString &value)
{
    _eventType = value;
}

void Event::setEventSource(const QString &value)
{
    _eventSource = value;
}

void Event::setImages(const QList<Rgb> &value)
{
    _images = value;
}

QDataStream &operator>>(QDataStream &in, cv::Mat &image)
{
    /*if (image->data) {
        delete [] image->data;
        image->data = 0;
    }*/
    size_t step;
    int rows, cols, type;
    char *data = 0;
    uint sz;
    int sstep;
    in >> rows >> cols >> type >> sstep;
    step = sstep;
    in.readBytes(data, sz);
    cv::Mat(rows, cols, type, data, step).copyTo(image);
    //assert(image.size[0] == (int)sz);
    delete [] data;
    return in;
}

QDataStream &operator<<(QDataStream &out, const cv::Mat &image)
{
    out << image.rows << image.cols << image.type() << (int)(size_t(image.step));
    return out.writeBytes((char *)image.data, image.size[0]);
}

QDataStream &operator>>(QDataStream &in, cv::Point3f &point)
{
    return in >> point.x >> point.y >> point.z;
}

QDataStream &operator<<(QDataStream &out, const cv::Point3f &point)
{
    return out << point.x << point.y << point.z;
}

QDataStream &operator>>(QDataStream &in, cv::Rect_<float> &rect)
{
    return in >> rect.x >> rect.y >> rect.height >> rect.width;
}

QDataStream &operator<<(QDataStream &out, const cv::Rect_<float> &rect)
{
    return out << rect.x << rect.y << rect.height << rect.width;
}

QDataStream &operator>>(QDataStream &in, types::Rgb &rgb)
{
    cv::Mat tmp;
    in >> tmp;
    rgb = tmp;
    return in;
}

QDataStream &operator<<(QDataStream &out, const types::Rgb &rgb)
{
    return out << rgb.getMat();
}

QDataStream &operator>>(QDataStream &in, types::Event &event)
{
    cv::Point3f center;
    cv::Rect_<float> sizes;
    QString eventType, eventSource;
    QList<types::Rgb> images;
    in >> center >> sizes >> eventType >> eventSource >> images;
    event = types::Event(center, sizes, eventType, eventSource, images);
    return in;
}

QDataStream &operator<<(QDataStream &out, const types::Event &event)
{
    return out << event.center() <<
                  event.sizes() <<
                  event.eventType() <<
                  event.eventSource() <<
                  event.images();
}

QByteArray Event::serialize() /*const*/
{
    QByteArray result;
    QDataStream ds(&result, QIODevice::WriteOnly);
    types::Event e = *this;
    ds << e;
    return result;
}

types::Event Event::deserialize(const QByteArray &data)
{
    types::Event result;
    QDataStream ds(data);
    ds >> result;
    return result;
}
