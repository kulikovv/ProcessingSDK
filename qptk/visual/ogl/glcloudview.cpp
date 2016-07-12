#include "glcloudview.h"
#include <QDebug>

using namespace visual::ogl;

GLCloudView::GLCloudView(QWidget *parent) :
    QGLWidget(parent)
{
    _xRot=0; _yRot=0; _zRot=0; _zTra=50; _nSca=0.3;
    _cloud =0;
}

void GLCloudView::initializeGL()
{
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
}

void GLCloudView::resizeGL(int width, int height)
 {

     glMatrixMode(GL_PROJECTION);

     glLoadIdentity();

     GLfloat ratio=(GLfloat)height/(GLfloat)width; // отношение высоты окна виджета к его ширине

     // мировое окно
     if (width>=height)
     {
           glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -100.0, 100.0); // параметры видимости ортогональной проекции
     }else{
           glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -100.0, 100.0); // параметры видимости ортогональной проекции
     }
     glViewport(0, 0, (GLint)width, (GLint)height);

     glMatrixMode(GL_MODELVIEW);

 }

void GLCloudView::drawAxis()
{
   glLineWidth(3.0f); // устанавливаю ширину линии приближённо в пикселях
   // до вызова здесь команды ширина была равна 1 пикселю по умолчанию

   glColor4f(1.00f, 0.00f, 0.00f, 1.0f); // устанавливается цвет последующих примитивов
   // ось x красного цвета
   glBegin(GL_LINES); // построение линии
      glVertex3f( 10.0f,  0.0f,  0.0f); // первая точка
      glVertex3f( 0.0f,  0.0f,  0.0f); // вторая точка
   glEnd();

   QColor halfGreen(0, 128, 0, 255);
   qglColor(halfGreen);
   glBegin(GL_LINES);
      // ось y зеленого цвета
      glVertex3f( 0.0f,  10.0f,  0.0f);
      glVertex3f( 0.0f, 0.0f,  0.0f);

      glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
      // ось z синего цвета
      glVertex3f( 0.0f,  0.0f,  10.0f);
      glVertex3f( 0.0f,  0.0f, 0.0f);
   glEnd();
}

void GLCloudView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); // чистим буфер

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(_nSca, _nSca, _nSca);

    glRotatef(_xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(_zRot, 0.0f, 1.0f, 0.0f);

    drawAxis();
   /* std::vector<cv::Point3f> pts;
    pts.push_back(cv::Point3f(0,1,0));
    pts.push_back(cv::Point3f(-1,-1,0));
    pts.push_back(cv::Point3f(1,-1,0));
    pts.push_back(cv::Point3f(1,1,1));
    pts.push_back(cv::Point3f(-1,-1,-1));
    pts.push_back(cv::Point3f(1,0,0));
    cv::Mat image(pts);*/


    if(!_volumeCloud.empty())
    {
        drawCloud(_cloud);
    }

    glFlush ();


}
void GLCloudView::reciveCloud(types::Rgbd* cloud)
{
    _cloud = cloud;
    cv::Mat vc;
    _cloud->getPointCloud(vc);
    if(vc.empty()){
        vc = _cloud->depth().getMat();
    }
    _volumeCloud = vc;
    _rgb=cloud->rgb().getMat().clone();
    repaint();
}

void GLCloudView::drawCloud(types::Rgbd* pts)
{

    if(!_volumeCloud.empty())
    {

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glColorPointer(3,GL_UNSIGNED_BYTE,0,_rgb.data);
        glVertexPointer(3, GL_FLOAT, 0, _volumeCloud.data);
        glDrawArrays(GL_POINTS, 0, _rgb.cols*_rgb.rows);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

void GLCloudView::mousePressEvent(QMouseEvent* pe)
{
    _ptrMousePosition = pe->pos();
}

void GLCloudView::mouseReleaseEvent(QMouseEvent* pe)
{

}

void GLCloudView::mouseMoveEvent(QMouseEvent* pe)
{
    _xRot += 180/_nSca*(GLfloat)(pe->y()-_ptrMousePosition.y())/height();
    _zRot += 180/_nSca*(GLfloat)(pe->x()-_ptrMousePosition.x())/width();

    _ptrMousePosition = pe->pos();

    updateGL();
}

void GLCloudView::wheelEvent(QWheelEvent* pe)
{
    if ((pe->delta())>0) _nSca=_nSca*1.1; else if ((pe->delta())<0) _nSca=_nSca/1.1;

    updateGL(); // обновление изображения
}
