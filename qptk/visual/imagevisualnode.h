#ifndef PROCESSINGFRAMEWORK_IMAGEVISUALNODE_H
#define PROCESSINGFRAMEWORK_IMAGEVISUALNODE_H

#include <visual/visualnode.h>

namespace types
{
    class Rgb;
}
class QImage;

namespace visual
{

class PROCESSINGFRAMEWORK_SHARED_EXPORT ImageVisualNode : public VisualNode
{
    Q_OBJECT
public:
    explicit ImageVisualNode(QObject *parent = 0);
    /*! Удаляет _image
      */
    virtual ~ImageVisualNode();
    /*!
      Обновляет данные переменной _image, данными с изображения, переданного
      в переменной m
      */
    QImage* toQImage(types::Rgb m);
    QImage* toQImage(QSharedPointer<types::Rgb> m);
signals:
    void sendImage(QImage* image);
public slots:
    virtual QIcon getIcon();
    void saveImage(QString fname="");
protected:
    QImage* _image;

};
}

#endif // IMAGEVISUALNODE_H
