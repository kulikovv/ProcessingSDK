#ifndef OGLVIEW_H
#define OGLVIEW_H

#include "visualnode.h"
#include <types/rgb.h>
#include <types/rgbd.h>

class QLayout;
namespace cv
{
    class Mat;
}

namespace visual
{
    namespace ogl{
        class GLCloudView;
        class GLView;
    }

class PROCESSINGFRAMEWORK_SHARED_EXPORT OglView : public VisualNode
{
    Q_OBJECT
    enum DataType{
        Image,
        Cloud
    };

public:
    explicit OglView(QObject *parent = 0);
    
signals:
    void sendCrossHair(int x,int y,bool visible);
    void sendImage(types::Rgb* image);
    void sendCloud(types::Rgbd* image);
public slots:
    virtual QIcon getIcon();
protected:
    virtual void receiveProcess(esp data);
    DataType _type;
    QLayout* _layout;
    ogl::GLCloudView* siv2;
    ogl::GLView *siv;
};
}

#endif // OGLVIEW_H
