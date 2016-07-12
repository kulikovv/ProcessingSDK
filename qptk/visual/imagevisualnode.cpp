#include "imagevisualnode.h"
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <types/rgb.h>
#include <QImage>
#include <QFileDialog>

using namespace visual;

ImageVisualNode::ImageVisualNode(QObject *parent) :
    VisualNode(parent)
{
    _image=0;
}

QIcon ImageVisualNode::getIcon()
{
    return QIcon(":/sdk/visual/ImageVisualNode");
}

ImageVisualNode::~ImageVisualNode()
{
    //qDebug() << "ImageVisualNode destroyed";
    if(0!=_image)
    {
        delete _image;
        _image=0;
    }
}
QImage* ImageVisualNode::toQImage(types::Rgb m)
{
    _image = m.createQImage(_image);
    return _image;
}

QImage* ImageVisualNode::toQImage(QSharedPointer<types::Rgb> m)
{
    _image = m->createQImage(_image);
    return _image;
}

void ImageVisualNode::saveImage(QString fname)
{
    if(_image)
    {
        if(!_image->isNull())
        {
            if(fname.isEmpty())
            {
                fname = QFileDialog::getSaveFileName(0,tr("Save image"),QString(),"*.png");
            }
            if(!fname.isEmpty())
            {
                if(!fname.contains(".png"))
                {
                    fname = fname + ".png";
                }
                _image->save(fname);
            }
        }
    }
}
