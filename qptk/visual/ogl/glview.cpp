#include "glview.h"
#include <QDebug>
using namespace visual::ogl;
int GLView::_textures=0;

GLView::GLView(QWidget *parent) :
    QGLWidget(parent)
{
    _drawcs = false;
    glFormat = GL_RGB;

    //QGL::setPreferredPaintEngine(QPaintEngine::OpenGL2);
    _textures++;
    _currenttexture = _textures;
}

GLView::~GLView()
{
    glDeleteTextures(_currenttexture,&texture);
}
void GLView::setImage(types::Rgb* ximage)
{
    cv::Mat image = ximage->getMat();
    if(image.cols!=_image.cols||image.rows!=_image.rows)
    {
        if(!_image.empty())
        {
            glDeleteTextures(_currenttexture,&texture);
        }
        _image = image;
        if(1==_image.channels())
        {
            glFormat = GL_LUMINANCE;
        }else{
            glFormat = GL_RGB;
        }
        initializeGL();


    }
    _image = image;
    this->repaint();
   // paintGL();

}

void GLView::initializeGL()
{

    glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
    glGenTextures(_currenttexture,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // ( NEW )
    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, _image.cols, _image.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL );

}

void GLView::resizeGL(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,w,0,h,-1,1);
    glMatrixMode (GL_MODELVIEW);
}


void GLView::drawCs(int x,int y)
{
    int sz=5;
    glColor3f(1.f,0.f,0.f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x-sz, y-sz);
    glVertex2f(x-sz, y+sz);
    glVertex2f(x+sz, y+sz);
    glVertex2f(x+sz, y-sz);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, y-3*sz);
    glVertex2f(x, y+3*sz);
    glVertex2f(x-3*sz, y);
    glVertex2f(x+3*sz, y);
    glEnd();
}

void GLView::setCH(int x,int y,bool visible)
{
    _csx = x;
    _csy = y;
    _drawcs = visible;
}

void GLView::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT);
    if(_image.empty())
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , _image.cols, _image.rows,  glFormat, GL_UNSIGNED_BYTE, _image.data );

    int hshift = (this->height()-_image.rows)/2;
    int wshift = (this->width()-_image.cols)/2;
    glColor3f(1.f,1.f,1.f);
    glBegin(GL_QUADS);   // in theory triangles are better
    glTexCoord2i(0,0); glVertex2i(wshift,hshift+_image.rows);
    glTexCoord2i(0,1); glVertex2i(wshift,hshift);
    glTexCoord2i(1,1); glVertex2i(wshift+_image.cols,hshift);
    glTexCoord2i(1,0); glVertex2i(wshift+_image.cols,hshift+_image.rows);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    if(_drawcs)
    {
        drawCs(wshift+_csx,hshift+_image.rows-_csy);
    }


    glFlush();
}


