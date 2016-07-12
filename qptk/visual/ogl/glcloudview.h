#ifndef GLCLOUDVIEW_H
#define GLCLOUDVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <opencv2/core/core.hpp>
#include <types/rgbd.h>

namespace visual{
namespace ogl
{
class GLCloudView : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLCloudView(QWidget *parent = 0);
    
signals:
    
public slots:
    void reciveCloud(types::Rgbd* cloud);
protected:
    void drawCloud(types::Rgbd* pts);
    void drawAxis();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mousePressEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);

    GLfloat _xRot;
    GLfloat _yRot;
    GLfloat _zRot;
    GLfloat _zTra;
    GLfloat _nSca;
    QPoint _ptrMousePosition;
    types::Rgbd* _cloud;
    cv::Mat _volumeCloud;
    cv::Mat _rgb;
};
}
}
#endif // GLCLOUDVIEW_H
