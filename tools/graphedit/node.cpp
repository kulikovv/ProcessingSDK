#include "node.h"
#include <QtWidgets/QtWidgets>
#include <QFontMetricsF>
#include "link.h"

using namespace ui;
/*
NodeItem::NodeItem(int id)
{
    _id=id;
    _text = "NodeItem";
    _type = "No type";
    _textColor = Qt::black;
    myOutlineColor = Qt::black;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsMovable | ItemIsSelectable | ItemIgnoresTransformations| ItemSendsGeometryChanges);
}*/
NodeItem::NodeItem(QString text, QString type)
{
  //  _id=id;
    _text = text;
    _type = type;
    _textColor = Qt::black;
    myOutlineColor = Qt::black;
    myBackgroundColor = Qt::white;

    setFlags(ItemIsMovable | ItemIsSelectable | ItemIgnoresTransformations| ItemSendsGeometryChanges);
}

NodeItem::~NodeItem()
{
    unlink();
}

void NodeItem::setIcon(QIcon icon)
{
    _icon = icon;
}

void NodeItem::setColor(QColor br)
{
    myBackgroundColor = br;
}

void NodeItem::setText(QString text)
{
    _text = text;
}

QString NodeItem::getText()
{
    return _text;
}

QString NodeItem::nodetype()
{
    return _type;
}

void NodeItem::unlink()
{
    foreach (ILink *link, _links)
    {
        delete link;
    }
}

void NodeItem::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(myOutlineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawPath(shape());

    QFontMetricsF metrics(qApp->font());
    QRectF textrect= metrics.boundingRect(_text);
    QPointF textp(-textrect.center().x(),rect.center().y());
    textrect.translate(textp);
    painter->setPen(_textColor);

    QRectF rect1;
    rect1.setCoords(rect.x(),rect.y(),rect.x()+rect.width(),rect.y()+rect.height()/2);
    QRectF rect2;
    rect2.setCoords(rect.x(),rect.y()+rect.height()/2,rect.x()+rect.width(),rect.y()+rect.height());

    if(!_icon.isNull())
    {
        rect1.setCoords(rect.x()+32,rect.y(),rect.x()+rect.width(),rect.y()+rect.height()/2);
        rect2.setCoords(rect.x()+32,rect.y()+rect.height()/2,rect.x()+rect.width(),rect.y()+rect.height());
        painter->drawPixmap(rect.x()+2,-16,32,32,_icon.pixmap(32,32));
    }

    painter->drawText(rect1, Qt::AlignCenter, _text);
    painter->drawText(rect2, Qt::AlignCenter, _type);
}




void NodeItem::addLink(ILink *link)
{
    _links.insert(link);
}
QRectF NodeItem::boundingRect() const
{
    const int Margin = 0;
    QRectF r = outlineRect();
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}
QPainterPath NodeItem::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundedRect(rect,5,5);

    return path;
}

void NodeItem::removeLink(ILink *link)
{
    _links.remove(link);
}



QRectF NodeItem::outlineRect() const
{
    const int Padding = 8;
    QFontMetricsF metrics(qApp->font());
        QRectF rect = metrics.boundingRect(_text);
        QRectF rect2 = metrics.boundingRect(_type);
        rect=rect.united(rect2);
        rect.adjust(0,0,0,rect2.height());
        rect.adjust(-Padding, -Padding, +Padding, +Padding);
        if(!_icon.isNull())
        {
            rect.adjust(-32, 0,0,0);
        }
        rect.translate(-rect.center());
        return rect;
}


QVariant NodeItem::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (ILink *link, _links)
        {
            link->trackNodeItems();
        }

        QPointF newPos = value.toPointF();

        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                                         QObject::tr("Edit Text"), QObject::tr("Enter new text:"),
                           QLineEdit::Normal, _text);
    if (!text.isEmpty())
    {
        if(emit nameChanged(text))
        {
            setText(text);
        }
    }
}
