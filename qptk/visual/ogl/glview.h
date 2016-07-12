#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>
#include <QGLWidget>
#include <types/rgb.h>

namespace visual{
namespace ogl
{
class GLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLView(QWidget *parent = 0);
    ~GLView();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
signals:

public slots:
    void setCH(int x,int y,bool visible);
    void setImage(types::Rgb* image);
private:
    void drawCs(int x,int y);
    cv::Mat _image;
    GLuint texture;
    GLuint glFormat;
    GLuint glType;
    int _csx;
    int _csy;
    bool _drawcs;
    int _currenttexture;
    static int _textures;
};
}
}
#endif // GLVIEW_H
