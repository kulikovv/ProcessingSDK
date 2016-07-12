#ifndef LAYOUTWIDGET_H
#define LAYOUTWIDGET_H

#include "zoomwidget.h"
#include <sdk_global.h>
#include <QMap>


class QSignalMapper;
class QImage;
class QGraphicsItem;
class QGraphicsPathItem;

namespace visual
{
namespace ui{
namespace utils{
    class Holder;
}
/*! Окно используемое для разметки кадра на области интереса
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT LayoutWidget : public ZoomWidget
{
    Q_OBJECT
public:
    enum Mode { none, rect, ellipse, sector, halfellipse, polygon };
    enum Feel { passage, obstacle };
    explicit LayoutWidget(QWidget *parent = 0);
    ~LayoutWidget();
    QString toString();
    /*! Restore the layout
      */
    bool fromString(QString data);
    bool hasChanges();
    QPointF getPosOfLayer(QString name);
    bool isInside(QString name,double x,double y);
    bool isInside(const QImage &img,double x,double y);
    QStringList getHoldersNames();
signals:
    void changes(QString str);
    void mapGenerated(QImage img);
    void realParamsChanged(float x,float y);
protected:
    void keyPressEvent(QKeyEvent *event);
public slots:
    void group();
    void ungroup();
    /*!
      renderScene into a bitmap all primitives with layer id = layer
      */
    QImage renderScene(QString layer);
    /*!
        Adds a mark on the scene for showing something
        \param p the coordinates of the mark
        \param label the name or some cation used to describe the mark
        \param color the color of the mark
        \return the indificator of the mark
      */
    QGraphicsItem* addMark(QPointF p,QString label,QColor color);
    QGraphicsItem* addVector(QPointF p,QPointF vec,QString label,QColor color);
    /*! Remove mark from plot
      \param id the indificator of the mark returned by addMarkFunction
      */
    void removeMark(QGraphicsItem* mark=0);

protected slots:
    /*! Slot accurs when user click menu item
      \param what
      */
    utils::Holder* actionTrigger(int what,QPolygonF mypolygon=QPolygonF());
    void stateChanged();
    void remove();
    void changeColor(int what);
    void edit();
    void hideHelpers(bool value);
protected:
    QPointF _click;//!< Координаты последнего клика мышки
    virtual void mousePressEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    QSignalMapper *actionMapper;
    QSignalMapper *feelMapper;
    bool _hasChanges;
    QMap<utils::Holder*,int> _holders;
    QList<QGraphicsItem*> _marks;
    void setEditMode(bool x);
private:
    bool _editMode;
    QString _lastName;
    QImage _lastRender;
    QPolygonF _poly;
    QGraphicsPathItem* _polyitem;
};
}
}
#endif // LAYOUTWIDGET_H
