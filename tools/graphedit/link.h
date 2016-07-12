#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>

namespace ui
{


class NodeItem;
class ILink: public QGraphicsPathItem
{
    public:
    virtual void trackNodeItems()=0;
};
class Link : public ILink
{
public:
    Link(NodeItem *fromNodeItem, NodeItem *toNodeItem);
    ~Link();

    NodeItem * fromNodeItem() const;
    NodeItem *toNodeItem() const;

    void setColor(const QColor &color);
    QColor color() const;

    void trackNodeItems();

private:
    NodeItem *myFromNodeItem;
    NodeItem *myToNodeItem;
    QLineF line1;
    QLineF line2;
    QLineF line3;

};
}

#endif
