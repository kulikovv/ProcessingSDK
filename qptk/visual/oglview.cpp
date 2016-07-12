#include "oglview.h"
#include <types/rgb.h>
#include <visual/ogl/glview.h>
#include <visual/ogl/glcloudview.h>
#include <QHBoxLayout>

using namespace visual;
OglView::OglView(QObject *parent) :
    VisualNode(parent)
{


    _widget = new QWidget(_invisibleParent);
    _layout = new QHBoxLayout();
    _widget->setLayout(_layout);

    siv2 = new ogl::GLCloudView(_widget);
    siv = new ogl::GLView(_widget);
    _type = Image;
    siv2->setVisible(false);
    _layout->addWidget(siv);
    _layout->addWidget(siv2);

    connect(this,SIGNAL(sendImage(types::Rgb*)),siv,SLOT(setImage(types::Rgb*)));
    connect(this,SIGNAL(sendCloud(types::Rgbd*)),siv2,SLOT(reciveCloud(types::Rgbd*)));

}

QIcon OglView::getIcon(){
    return QIcon(":/sdk/visual/OglView");
}

void OglView::receiveProcess(esp data)
{
    QSharedPointer<types::Rgb> var = data.getReadOnly<types::Rgb>();
    if(0!=var)
    {
        if(Cloud==_type){
            siv->setVisible(true);
            siv2->setVisible(false);
            _type = Image;
        }
        emit sendImage(var.data());
    }



    QSharedPointer<types::Rgbd> readonly = data.getReadOnly<types::Rgbd>();
    if(0!=readonly)
    {
     /*   if(data!="3D")
        {
            qDebug("Not 3D");
            return;
        }*/
        if(Image==_type){
            siv->setVisible(false);
            siv2->setVisible(true);
            _type = Cloud;
        }
        emit sendCloud(readonly.data());
    }
}
