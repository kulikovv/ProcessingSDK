#include "layoututils.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextStream>
#include <QDebug>
#include <utils/logs.h>
#include <QGraphicsTextItem>

using namespace visual::ui::utils;

int Anchor::type() const
{
    return UserType + 1;
}
void Anchor::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if(_isPositionChanged)
    {
        _isPositionChanged = false;
        emit stateChanged();
    }
    QGraphicsObject::mouseReleaseEvent(event);
}
int AnchorFix::type() const
{
    return UserType + 3;
}

AnchorFix::AnchorFix(QGraphicsItem *parent):Anchor(parent)
{
    _orient = Qt::Horizontal;
}

AnchorFix::AnchorFix(QPointF coords,QGraphicsItem *parent,Qt::Orientation orient,double size,Shape shape,QColor color):
    Anchor(coords,parent,size,shape,color)
{
    _orient = orient;
}

QVariant AnchorFix::itemChange(GraphicsItemChange change,const QVariant &value)
{
    if (QGraphicsItem::ItemPositionHasChanged == change )
    {
        switch(_orient)
        {
            case Qt::Horizontal:
                if(pos().x()>0)
                {
                    setPos(pos().x(),0);
                    emit anchorMoved(pos());
                }else{
                    setPos(0,0);
                } break;
            case Qt::Vertical:
                if(pos().y()>0) {
                    setPos(0,pos().y());
                    emit anchorMoved(pos());
                }else{
                    setPos(0,0);
                } break;
            default: setPos(pos().x(),0); break;
        }
        _isPositionChanged = true;
    }

    return QGraphicsItem::itemChange(change, value);
}

Anchor::Anchor(QGraphicsItem *parent):QGraphicsObject(parent)
{
    setFlags(ItemIsMovable | ItemIsSelectable |  ItemSendsGeometryChanges);
    _shape = circle;
    _size = 20;
    _color = Qt::blue;
    _isPositionChanged = false;
}
Anchor::Anchor(QPointF coords,QGraphicsItem *parent,double size,Shape shape,QColor color):QGraphicsObject(parent)
{
    _color = color;
    setFlags(ItemIsMovable | ItemIsSelectable |  ItemSendsGeometryChanges);
    setPos(coords);
    _size = size;
    _shape = shape;
    _isPositionChanged = false;
}

//! Overiden functions from QGraphicsItem
QRectF Anchor::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

void Anchor::setColor(QColor color)
{
    _color = color;
}

void Anchor::paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget * /*widget*/)
{

    QPen pen(_color);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);

    }
    painter->setBrush(QColor(_color.red(),_color.green(),_color.blue(),128));
    painter->setPen(pen);

    switch(_shape)
    {
        case circle: painter->drawEllipse(outlineRect()); break;
        case rect: painter->drawRect(outlineRect());break;
        default: painter->drawEllipse(outlineRect()); break;
    }
}
QRectF Anchor::outlineRect() const
{
    return QRectF(-_size/2,-_size/2,_size,_size);
}

QVariant Anchor::itemChange(GraphicsItemChange change,const QVariant &value)
{
    if ( QGraphicsItem::ItemPositionHasChanged == change )
    {
        _isPositionChanged = true;
        emit anchorMoved(pos());
    }
    return QGraphicsItem::itemChange(change, value);
}


int Holder::type() const
{
    return UserType + 2;
}
int Holder::_counter = 1;
Holder::Holder(QGraphicsItem  *parent):QGraphicsObject(0)
{
    setFlags(ItemSendsGeometryChanges);
    _obj = parent;
    _angle = 0;
    _sx = _sy = 1;

    _rotation = _move = _scalex = _scaley = 0;
    _text = 0;
    _line = 0;

    _realsizex=0;
    _realsizey=0;

    init();
}

Holder::~Holder()
{

}
void Holder::destroy()
{
    if(_obj){
      //  scene()->removeItem(_obj);
        scene()->removeItem(_move);	
    }
}
void Holder::setObjectBrush(QBrush brush)
{
    QAbstractGraphicsShapeItem* obj = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(_obj);

    if(!obj)
        return;

    obj->setBrush(brush);
}
void Holder::setBinary(bool value)
{
   // bool value = true;
    _rotation->setVisible(!value);
    _move->setVisible(!value);
    _scalex->setVisible(!value);
    _scaley->setVisible(!value);
    _text->setVisible(!value);

    setBinary(_obj,value);

   /* if(tag.isEmpty()||(tag == this->objectName()))
    {
        setBinary(_obj,value);
    }else{

    }

    foreach(QGraphicsItem* item,_obj->childItems())
    {
            if(!tag.isEmpty())
            {

            }
            setBinary(item,value);
    }*/

}

void Holder::setBinary(QGraphicsItem* item,bool value)
{
    QAbstractGraphicsShapeItem* obj = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(item);

    if(!obj)
        return;

    if(value)
    {
        obj->setData(2,obj->brush().color());
        Qt::GlobalColor color = Qt::white;
        if(obj->brush().color().red() > obj->brush().color().green())
        {
            color = Qt::black;
        }
        obj->setBrush(color);
        obj->setPen(QPen(color, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }else{
        obj->setBrush(QBrush(obj->data(2).value<QColor>()));
        obj->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
}

QString Holder::toString()
{
    QString str;
    QAbstractGraphicsShapeItem* obj = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(_obj);

    QBrush _brush;
    if(obj){
        _brush =obj->brush();
    }
    QTextStream stream(&str);
    stream << _scalex->pos().x()<<";"<<_scalex->pos().y() <<";"<<_scaley->pos().x()<<";"<<_scaley->pos().y() <<";"<< pos().x()<<";"<< pos().y()<<";"<<_rotation->pos().x() << ";" <<_rotation->pos().y() <<";"<< _brush.color().red() << ";"<<_brush.color().green() << ";"<< _brush.color().blue()<<";"<< _brush.color().alpha()<<";"<<objectName();
    if(!_adationalInformation.isEmpty())
    {
        foreach(QString str,_adationalInformation)
        {
            stream <<";" << str;
        }
    }
    return str;
}
QGraphicsItem* Holder::getObject() const
{
    return this->_obj;
}
void Holder::setObjectName(const QString &name)
{
    this->_text->setHtml(name);
    _obj->setData(0,name);
    QObject::setObjectName(name);
}

float Holder::getPX(float realx)
{
    if(-1!=realx)
    {
        _realsizex = realx;
    }
   // qDebug() << "Size X" << _obj->boundingRect().width()*_sx;
    return _realsizex/(_obj->boundingRect().width()*_sx);
}
float Holder::getRealX()
{
    return _realsizex;
}

float Holder::getRealY()
{
    return _realsizey;
}

float Holder::getPY(float realy)
{
    if(-1!=realy)
    {
        _realsizey = realy;
    }
 //   qDebug() << "Size Y" << _obj->boundingRect().height()*_sy;
    return _realsizey/(_obj->boundingRect().height()*_sy);
}

QPolygonF Holder::parseAdditionalData(QString descr)
{
    QPolygonF poly;
    QStringList data = descr.split(";");
    if(data.count()>13)
    {
        for(int i=13;i<data.count()-1;i=i+2){
            poly.push_back(QPointF(data[i].toFloat(),data[i+1].toFloat()));
        }
    }
    return poly;
}

bool Holder::fromString(QString value)
{
    QStringList data = value.split(";");
    if(8>data.count())
    {
        return false;
    }

    QAbstractGraphicsShapeItem* obj = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(_obj);

    if(!obj)
        return false;


    _scalex->setPos(data[0].toFloat(),data[1].toFloat());
    _scaley->setPos(data[2].toFloat(),data[3].toFloat());
    _move->setPos(data[4].toFloat(),data[5].toFloat());
    _rotation->setPos(data[6].toFloat(),data[7].toFloat());
    if(data.count()>11)
    {
        QColor color;
        color.setRgb(data[8].toInt(),data[9].toInt(),data[10].toInt(),data[11].toInt());
        if(obj){
             obj->setBrush(color);
        }
    }
    if(data.count()>12)
    {
        _adationalInformation.clear();
        this->setObjectName(data[12]);
        for(int i=13;i<data.count();i++){
            _adationalInformation.push_back(data[i]);
        }
    }

    return true;
}

void Holder::setAdditionalInfo(QStringList data)
{
    _adationalInformation = data;
}

QStringList Holder::additionalInfo()
{
    return _adationalInformation;
}

bool Holder::isSelected()
{
    return _move->isSelected();
}

void Holder::init()
{
   //Check input object
   if(0==_obj)
   {
        CRITICAL("Holder#No object")
        return;
   }
   if(0==_obj->scene())
   {
        CRITICAL("Holder#Object has no scene")
        return;
   }

   QPointF center = _obj->boundingRect().center();
   QPointF objpos = _obj->pos();

   foreach(QGraphicsItem* item,_obj->childItems())
   {
       item->setPos(item->pos().x()-center.x(),item->pos().y()-center.y());
   }
   if(_obj->childItems().isEmpty())
   {
    //    _obj->setPos(0,0);
   }

   center = QPointF(0,0);

 //  qDebug() << _obj->boundingRect().center()<<_obj->pos();
   _move = new Anchor(center,0,20,Anchor::rect);
   _move->setToolTip(tr("Move object"));


   if(_obj->childItems().isEmpty())
   {
       _text = new QGraphicsTextItem("",_move);
       QString name = _obj->data(0).toString();
       if(name.isEmpty())
       {
           name = QString("Zone%1").arg(_counter);

       }
       this->setObjectName(name);

       _counter++;
       _text->setPos(20,-10);
    }else{
     /*  foreach(QGraphicsItem* item,_obj->childItems())
       {
           QGraphicsTextItem* text = new QGraphicsTextItem(item->data(0).toString(),item);
           text->setTransform(item->transform());
       }*/
   }

   _rotation  = new Anchor(center-QPointF(_obj->boundingRect().width(),0),this,20,Anchor::circle,Qt::red);
   _rotation->setToolTip(tr("Rotate object"));

   _scalex = new AnchorFix(QPointF(0,_obj->boundingRect().height()/2),_move,Qt::Vertical ,10,Anchor::rect,Qt::green);
   _scalex->setToolTip(tr("Scale Y"));
   _scaley = new AnchorFix(QPointF(_obj->boundingRect().width()/2,0),_move,Qt::Horizontal,10,Anchor::rect,Qt::yellow);
   _scaley->setToolTip(tr("Scale X"));

   _line = new QGraphicsLineItem(QLineF(QPointF(0,0),QPointF(-_obj->boundingRect().width(),0)),_move);

   _obj->scene()->addItem(this);
//   setPos(_obj->pos());
   scene()->addItem(_move);

   connect(_move,SIGNAL(anchorMoved(QPointF)),this,SLOT(locationChange(QPointF)));
   connect(_scalex,SIGNAL(anchorMoved(QPointF)),this,SLOT(scaleX(QPointF)));
   connect(_scaley,SIGNAL(anchorMoved(QPointF)),this,SLOT(scaleY(QPointF)));
   connect(_rotation,SIGNAL(anchorMoved(QPointF)),this,SLOT(rotationChange(QPointF)));

   connect(_move,SIGNAL(stateChanged()),this,SIGNAL(paramsChanged()));
   connect(_scalex,SIGNAL(stateChanged()),this,SIGNAL(paramsChanged()));
   connect(_scaley,SIGNAL(stateChanged()),this,SIGNAL(paramsChanged()));
   connect(_rotation,SIGNAL(stateChanged()),this,SIGNAL(paramsChanged()));

   _move->setPos(_obj->pos());
}

QPointF Holder::rotationPos()
{
    return _rotation->pos();
}

QPointF Holder::locationPos()
{
    return _move->pos();
}

QPointF Holder::scaleX()
{
    return _scalex->pos();
}

QPointF Holder::scaleY()
{
    return _scaley->pos();
}

void Holder::setRotationPos(QPointF pos)
{
    _rotation->setPos(pos);
}

void Holder::setLocationPos(QPointF pos)
{
    _move->setPos(pos);
}

void Holder::setScaleXPos(QPointF pos)
{
    _scalex->setPos(pos);
}

void Holder::setScaleYPos(QPointF pos)
{
    _scaley->setPos(pos);
}

void Holder::rotationChange(QPointF pos)
{
     QLineF line(pos, QPointF(0,0));
     _angle = line.angle(QLineF(0, 0, 1, 0));
     if (line.dy() < 0){
              _angle = 360 - _angle;
     }

     _line->setLine(QLineF(QPointF(0,0),QPointF(-line.length(),0)));
     applyTransform(_angle,_sx,_sy);
}

void Holder::scaleX(QPointF pos)
{
     _sy = (pos.y()*2)/_obj->boundingRect().height();
     applyTransform(_angle,_sx,_sy);
}

void Holder::scaleY(QPointF pos)
{
    _sx = (pos.x()*2)/_obj->boundingRect().width();
    applyTransform(_angle,_sx,_sy);
}

void Holder::locationChange(QPointF pos)
{
    //_obj->setPos(pos-QPointF(_obj->boundingRect().width()/2,_obj->boundingRect().height()/2)-_obj->boundingRect().topLeft());
    _obj->setPos(pos);

    setPos(_obj->pos());
}

void Holder::applyTransform(double angle, double sx,double sy)
{
    QTransform trans;
    trans.rotate(angle);
    _move->setTransform(trans);
    trans.scale(sx,sy);
    _obj->setTransform(trans);
    emit realParamsChanged(getPX(-1),getPY(-1));
}

QRectF Holder::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void Holder::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
  //  painter->setTransform(_obj->transform());
 //   painter->drawRect(_obj->boundingRect());
}

QVariant Holder::itemChange(GraphicsItemChange change,const QVariant &value)
{
    if ( QGraphicsItem::ItemSceneChange == change && 0!=scene())
    {
        destroy();
    }
    return QGraphicsItem::itemChange(change, value);
}
