#ifndef LAYOUTUTILS_H
#define LAYOUTUTILS_H

#include <QMap>
#include <QGraphicsObject>
#include <QPen>

class QGraphicsLineItem;
class QGraphicsTextItem;

namespace visual{
namespace ui{
namespace utils{
/*!
  \brief  That class represent a node that makes transforms on QGraphicsItem objects.
  */
class Anchor: public QGraphicsObject
{
    Q_OBJECT
public:
    /*! Emun of shapes
      */
    enum Shape{
        circle,//!< The anchor will be circle shape
        rect//!<
    };

    //! Enable the use of qgraphicsitem_cast with this item.
    virtual int type() const;

    Anchor(QGraphicsItem *parent=0);
    /*! Create a
      */
    Anchor(QPointF coords,QGraphicsItem *parent=0,double size=20.0,Shape shape=circle,QColor color=Qt::blue);

    //! Overiden functions from QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setColor(QColor color);
signals:
    /*! Signal is emited when anchor is moved.
      \param location the new location of element
      */
    void anchorMoved(QPointF location);
    void stateChanged();
protected:
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    QRectF outlineRect() const;
    /*! Processing of events. Override of QGraphicsItem function.
      \param change  type of event
      \param value of event
      \sa QGraphicsItem
      */
    QVariant itemChange(GraphicsItemChange change,const QVariant &value);
    double _size;//!< The size of anchor
    Shape _shape;//!< The shape of anchor
    QColor _color;//!< Color
    bool _isPositionChanged;//!< True is the position has changed
};
/*! \brief Anchor with fixed movements along one axis
  */
class AnchorFix: public Anchor
{
    Q_OBJECT
public:
    /*! Получить
      */
    virtual int type() const;
    /*!
      */
    AnchorFix(QGraphicsItem *parent=0);
    /*!
      */
    AnchorFix(QPointF coords,QGraphicsItem *parent=0,Qt::Orientation orient=Qt::Horizontal,double size=20.0,Shape shape=circle,QColor color=Qt::blue);
protected:
    /*! Processing of events. Override of QGraphicsItem function.
      */
    QVariant itemChange(GraphicsItemChange change,const QVariant &value);
    Qt::Orientation _orient;
};

/*! \brief Object to trasform and shape changing tools for a QAbstractGraphicsShapeItem
  * he holder is a set of diferent tool that allow to do primitives transformations with
  a QAbstractGraphicsShapeItem.
  */
class Holder: public QGraphicsObject
{
    Q_OBJECT
public:
    virtual int type() const;
    /*! Create a new holder with object to look at
      */
    Holder(QGraphicsItem *parent);
    ~Holder();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
    /*! Implementation of
      \return the bounding rect
      */
    virtual QRectF boundingRect() const;
    void setBinary(bool value);
    /*! Draw the object with white color.
      \param value if value is true object will be drow in white,
      if set to false the object will be drown normal mode
      */
    void setBinary(QGraphicsItem* item,bool value=true);
    /*! Convert all params of Holder into string
      * \return serialized string
      */
    QString toString();
    /*! Restore cofiguration from string
      \param value the serialized string
      \return true if restoration is sucessful, false otherwise
      */
    bool fromString(QString value);
    /*! Get the object that delegates with the holder
      \return object t
      */
    QGraphicsItem* getObject() const;
    /*!
     * \brief additionalInfo доп информация
     * \param data то, что хотим передать
     */
    void setAdditionalInfo(QStringList data);
    QStringList additionalInfo();
    /*! Change the name of holder
      \param name the new name
    */
    void setObjectName(const QString &name);
    /*! set the real size in meters, and returns the calculation of meters per pixel
      */
    float getPX(float realx);
    /*! set the real size in meters, and returns the calculation of meters per pixel
      */
    float getPY(float realy);
    /*! \return the real size in meters
      */
    float getRealX();
    /*!
      * \return the real size in meters
      */
    float getRealY();
    void setObjectBrush(QBrush brush);
    bool isSelected();

    QPointF rotationPos();
    QPointF locationPos();
    QPointF scaleX();
    QPointF scaleY();
    void setRotationPos(QPointF pos);
    void setLocationPos(QPointF pos);
    void setScaleXPos(QPointF pos);
    void setScaleYPos(QPointF pos);
    static QPolygonF parseAdditionalData(QString descr);
signals:
    void paramsChanged();
    void realParamsChanged(float x,float y);
protected:

    QVariant itemChange(GraphicsItemChange change,const QVariant &value);
    void init();

protected slots:
    void rotationChange(QPointF pos);
    void locationChange(QPointF pos);
    void scaleX(QPointF pos);
    void scaleY(QPointF pos);
    void destroy();

private:
    QGraphicsItem * _obj;//!< Object to work with
    Anchor* _rotation;//!< Rotation anchor
    Anchor* _move;//!< Movement anchor
    Anchor* _scalex;//!< Scale X anchor
    Anchor* _scaley;//!< Scale Y anchor
    QGraphicsTextItem* _text;
    double _angle,_sx,_sy;//!< Stored state of transformation
    /*! Apply a transformation on object. Only affine transformation are allowed
      \param angle rotation angle
      \param sx scale by x axis
      \param sy scale by y axis
      */
    void applyTransform(double angle, double sx,double sy);
    QGraphicsLineItem* _line;//!< lie from object center to

    QBrush _objbrush;//!< Cache brush in binary mode
    QPen _objpen;//!< Cache pen in binary mode
    static int _counter;//!< New label generator

    float _realsizex;//!< Real size in meters
    float _realsizey;//!< Real size in meters

    QStringList _adationalInformation;//! Inner data
};
}
}
}
#endif // LAYOUTUTILS_H
