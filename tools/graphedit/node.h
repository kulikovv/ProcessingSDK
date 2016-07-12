#ifndef NodeItem_H
#define NodeItem_H

#include <QtGui>
#include <QGraphicsItem>
#include <QSet>
#include <QAbstractItemModel>

namespace ui
{


class ILink;
/*!
 * \brief The NodeItem class
 */
class NodeItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit NodeItem(QString text,QString type);
    ~NodeItem();


    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);


    void addLink(ILink *link);
    void removeLink(ILink *link);
    QString getText();
    void unlink();
    QString nodetype();

    //Style section
    void setIcon(QIcon icon);
    void setColor(QColor br);
public slots:
    void setText(QString text);
signals:
    bool nameChanged(QString name);
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private:
    QRectF outlineRect() const;

    QString _text;
    QString _type;
    QIcon _icon;
    QColor _textColor;
    QColor myBackgroundColor;
    QColor myOutlineColor;
    //Links associated with the node
    QSet<ILink *> _links;
};
}

#endif // NodeItem_H
