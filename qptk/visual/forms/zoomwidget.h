#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QtWidgets/QWidget>
#include <sdk_global.h>
#include <QIcon>
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;
class QAction;
class QSignalMapper;

namespace visual
{
namespace ui{
/*! Виджет для увеличения размеров изображения с использованием QtGraphics Framework
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT ZoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomWidget(QWidget *parent = 0);

signals:
    void saveScreen();
public slots:
    void zoomIn();
    void zoomOut();
    void toOriginal();
    void fit();
    void setImage(QImage* image);
    void setZoomCenter(QPointF point);
protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
protected:
    QAction* createAction(QString text,QSignalMapper* mapper,int value,QIcon icon=QIcon());
    QAction* createAction(QString text,QIcon icon=QIcon());
    void setSceneRect( qreal x, qreal y, qreal w, qreal h);
    QGraphicsView*  _view;
    QGraphicsScene* _scene;
    QGraphicsRectItem* _rendersurface;
    float _zoomstep;

};

}
}
#endif // ZOOMWIDGET_H
