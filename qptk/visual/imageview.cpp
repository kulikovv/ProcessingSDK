#include "imageview.h"

#include <types/rgb.h>
#include <visual/forms/simpleimageview.h>

#include <QDebug>

using namespace visual;

ImageView::ImageView(QObject *parent) :
    ImageVisualNode(parent)
{
     ui::SimpleImageView* siv = new ui::SimpleImageView(_invisibleParent);
     connect(siv,SIGNAL(imageClicked(MouseControl)),this,SLOT(onImageClicked(MouseControl)));
     connect(siv,SIGNAL(saveScreen()),this,SLOT(saveImage()));
     connect(this,SIGNAL(sendImage(QImage*)),siv,SLOT(setData(QImage*)));
     _widget=siv;
     _prefered = "";


}
QString ImageView::prefered() const
{
    return _prefered;
}
void ImageView::setPrefered(QString value)
{
    _prefered = value;
}

//This function is called if this node is enable
void ImageView::receiveProcess(esp data)
{
    if(!_prefered.isEmpty())
    {

        if(data!=_prefered)
        {
            return;
        }
    }

    QSharedPointer<types::Rgb> var = data.getReadOnly<types::Rgb>();
    if(0!=var)
    {
        _innerdata = data;
        cv::Mat img = var->getMat();
        preDraw(img);
        emit sendImage(toQImage(var));
    }
}
void ImageView::preDraw(cv::Mat& img)
{
    Q_UNUSED(img)
    //Nothing to do by default
    Q_UNUSED(img)
}

void ImageView::onImageClicked(const MouseControl point)
{
    emit send(_innerdata);
 //   SEND(MouseControl,new MouseControl(point))
    emit send(esp::init<MouseControl>(new MouseControl(point),frame,""));
}

void ImageView::userReaction()
{
  //  emit send(esp::create<PMat>(&data));
}

