#include <QtGui>

#include "link.h"
#include "node.h"
#define Pi 3.14
using namespace ui;
Link::Link(NodeItem *fromNodeItem, NodeItem *toNodeItem)
{
    myFromNodeItem = fromNodeItem;
    myToNodeItem = toNodeItem;

    myFromNodeItem->addLink(this);
    myToNodeItem->addLink(this);

    setZValue(-1);


    setColor(Qt::black);
    trackNodeItems();
}

Link::~Link()
{
    myFromNodeItem->removeLink(this);
    myToNodeItem->removeLink(this);
}

NodeItem *Link::fromNodeItem() const
{
    return myFromNodeItem;
}

NodeItem *Link::toNodeItem() const
{
    return myToNodeItem;
}

void Link::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
    setBrush(QBrush(color,Qt::SolidPattern));
}

QColor Link::color() const
{
    return pen().color();
}

void Link::trackNodeItems()
{
 //   setLine(QLineF(myFromNodeItem->pos(), myToNodeItem->pos()));
    QPainterPath path;
    if (myFromNodeItem->collidesWithItem(myToNodeItem))
    {
        setPath(path);
             return;
    }

    QPointF end   = myToNodeItem->pos();
    QPointF start  = myFromNodeItem->pos();


    QLineF line(end,start);
/*    qreal min = (start.y()>end.y())? start.y():end.y();
    QPointF t1(start.x(),min-(start.y()-end.y())/2.0);
    QPointF t2(end.x(),min-(start.y()-end.y())/2.0);*/
  /*  path.moveTo(start);
    path.lineTo(t1);
    path.lineTo(t2);
    path.lineTo(end);*/


    QPointF intersectPoint(0,0);

    QPolygonF endPolygon(myToNodeItem->boundingRect());

    QPointF p1 = endPolygon.first() + end;
    QPointF p2;
    for (int i = 1; i < endPolygon.count(); ++i) {
        p2 = endPolygon.at(i) + end;
        if (QLineF(p1, p2).intersect(line, &intersectPoint) == QLineF::BoundedIntersection)
        {
             break;
        }
        p1 = p2;
    }
    qreal arrowSize = 10;
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
          angle = (Pi * 2) - angle;
    QPointF arrowP1 = intersectPoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                             cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = intersectPoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                             cos(angle + Pi - Pi / 3) * arrowSize);
    QPolygonF arrowHead;

    arrowHead << intersectPoint << arrowP1 << arrowP2<<intersectPoint;
    path.moveTo(start);
    path.lineTo(end);
    path.addPolygon(arrowHead);
    setPath(path);
}
/*
QRectF Link::boundingRect() const
{
    return QRectF(myFromNodeItem->pos(),myToNodeItem->pos());
}
QPainterPath Link::shape() const
{
    QPainterPath path;
    QPointF start = myFromNodeItem->pos();
    QPointF end = myToNodeItem->pos();
    path.addRect(start.x(),start.y(),end.x()-start.x(),end.y()-start.y());
    return path;
}
void Link::paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black,1.0));
    QPointF start = myFromNodeItem->pos();
    QPointF end = myToNodeItem->pos();


    QLineF line1(start,end);
 //   QLineF line2(t1,t2);
 //   QLineF line3(t2,end);
  //  painter->drawLine(line1);
 //   painter->drawLine(line2);
    painter->drawLine(line1);
}
*/
