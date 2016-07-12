#include "layoutwidget.h"
#include "layoututils.h"
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QCursor>
#include <QSignalMapper>
#include <utils/logs.h>
#include <QMouseEvent>
#include <QInputDialog>
#include <QRgb>
#include <QApplication>
#include "holderdialog.h"

using namespace visual::ui;
using namespace visual::ui::utils;



LayoutWidget::LayoutWidget(QWidget *parent) :
    ZoomWidget(parent)
{
    _editMode = false;
   // this->grabMouse();
    _hasChanges = true;
    _polyitem = 0;

    actionMapper = new QSignalMapper();
    feelMapper = new QSignalMapper();

    connect(createAction(tr("Add Rectangle"),actionMapper,LayoutWidget::rect,QIcon(":/sdk/visual/forms/Rectangle")), SIGNAL(triggered()), actionMapper, SLOT(map()));
    connect(createAction(tr("Add Ellipse"),actionMapper,LayoutWidget::ellipse,QIcon(":/sdk/visual/forms/Ellipse")), SIGNAL(triggered()), actionMapper, SLOT(map()));
    connect(createAction(tr("Add Sector"),actionMapper,LayoutWidget::sector,QIcon(":/sdk/visual/forms/Sector")), SIGNAL(triggered()), actionMapper, SLOT(map()));
    connect(createAction(tr("Add Half Ellipse"),actionMapper,LayoutWidget::halfellipse,QIcon(":/sdk/visual/forms/HalfEllipse")), SIGNAL(triggered()), actionMapper, SLOT(map()));
    connect(createAction(tr("Add Polygon"),actionMapper,LayoutWidget::polygon,QIcon(":/sdk/visual/forms/Polygon")), SIGNAL(triggered()), actionMapper, SLOT(map()));
    connect(actionMapper, SIGNAL(mapped(const int &)),this, SLOT(actionTrigger(int)));

    connect(createAction(tr("Mark as passage"),feelMapper,LayoutWidget::passage,QIcon(":/sdk/visual/forms/Passage")), SIGNAL(triggered()), feelMapper, SLOT(map()));
    connect(createAction(tr("Mark as obstacle"),feelMapper,LayoutWidget::obstacle,QIcon(":/sdk/visual/forms/Obstacle")), SIGNAL(triggered()), feelMapper, SLOT(map()));
    connect(feelMapper, SIGNAL(mapped(const int &)),this, SLOT(changeColor(int)));

    connect(createAction(tr("Remove"),QIcon(":/sdk/visual/forms/Remove")),SIGNAL(triggered()),this,SLOT(remove()));
    connect(createAction(tr("Edit"),QIcon(":/sdk/visual/forms/Edit")),SIGNAL(triggered()),this,SLOT(edit()));
    QAction *hide;
    connect(hide=createAction(tr("Hide Editors"),QIcon(":/sdk/visual/forms/Hide")),SIGNAL(toggled(bool)),this,SLOT(hideHelpers(bool)));
    hide->setCheckable(true);
  //   connect(createAction(tr("Group")),SIGNAL(triggered()),this,SLOT(group()));
 //   connect(createAction(tr("Ungroup")),SIGNAL(triggered()),this,SLOT(ungroup()));

    setMouseTracking(true);
    qApp->installEventFilter(this);

    _lastName = "";
    QPixmap pix(":/images/background");
    if(pix.isNull())
    {
        _scene->setBackgroundBrush(Qt::black);
    }else{
        _scene->setBackgroundBrush(pix);
    }
}

void LayoutWidget::setEditMode(bool x)
{
    bool change = false;
    if(x&&!_editMode){
        _poly.clear();
        if(_polyitem){
            delete _polyitem;
            _polyitem=0;
        }
        QPainterPath path;
        path.addPolygon(_poly);
        _polyitem=_scene->addPath(path,QPen(Qt::red,2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
        change = true;
    }

    if(!x&&_editMode){
        if(_poly.first()==_poly.last()&&_poly.size()>2){
            actionTrigger(polygon,_poly);

            if(_polyitem){
                _scene->removeItem(_polyitem);
                delete _polyitem;
                _polyitem=0;
            }
        }
        change = true;
    }

    _editMode = x;
    if(change){
        foreach(QAction *a,this->actions()){
            a->setDisabled(_editMode);
        }
    }
}

void LayoutWidget::hideHelpers(bool value)
{
    if(_rendersurface){
        if(value){
            _rendersurface->setZValue(15);
        }else{
            _rendersurface->setZValue(-15);
        }
    }
}

LayoutWidget::~LayoutWidget()
{
    _view->close();
    _scene->clear();
    _holders.clear();
}

void LayoutWidget::group()
{
    QList<Holder*> distinct;
    foreach(Holder* item,_holders.keys())
    {

        if(item->isSelected())
        {
                distinct.push_back(item);
        }

    }
    if(distinct.count()<2)
        return;

    QGraphicsItemGroup* group = new QGraphicsItemGroup();
    foreach(Holder* holder,distinct)
    {
        group->addToGroup(holder->getObject());
        _holders.remove(holder);
        _scene->removeItem(holder);
    }
    _scene->addItem(group);
    Holder* holder = new Holder(group);
    _holders.insert(holder,-1);
    connect(holder,SIGNAL(paramsChanged()),this,SLOT(stateChanged()));
}

void LayoutWidget::ungroup()
{
    QList<Holder*> distinct;
    foreach(Holder* item,_holders.keys())
    {

        if(item->isSelected())
        {
                distinct.push_back(item);
        }

    }
    QList<QGraphicsItem*> items;
    foreach(Holder* holder,distinct)
    {
        QList<QGraphicsItem*> childs = holder->getObject()->childItems();
        if(childs.count()>0)
        {
            items << childs;
            _holders.remove(holder);
            _scene->removeItem(holder);
        }
    }
    foreach(QGraphicsItem* item,items)
    {
        Holder* holder = new Holder(item);
        _holders.insert(holder,-1);
        connect(holder,SIGNAL(paramsChanged()),this,SLOT(stateChanged()));
    }
}

QString  LayoutWidget::toString()
{
    QString str;
    QTextStream stream(&str);
    foreach(Holder *hl,_holders.keys())
    {
        stream<<_holders.value(hl)<<"("<<hl->toString()<<")";
    }

    return str;
}
bool LayoutWidget::fromString(QString data)
{
    QRegExp rx("\\(|\\)+");

    foreach(Holder* hold,_holders.keys())
    {
        _scene->removeItem(hold->getObject());
        _scene->removeItem(hold);
    }
    _holders.clear();

    QStringList ls = data.split(rx,QString::SkipEmptyParts);
    if(0!=ls.count()%2)
    {
        INFO(tr("Length illegal must be odd"))
        return false;
    }
    for(int i=0;i<ls.count();i=i+2)
    {
        if(1!=ls[i].length()){
            INFO(tr("Unknown primitives"))
            return false;
        }
    }
    for(int i=0;i<ls.count();i=i+2)
    {
        Holder* hold = actionTrigger(ls[i].toInt(),Holder::parseAdditionalData(ls[i+1]));
        if(hold){
             hold->fromString(ls[i+1]);
        }else{
            CRITICAL(tr("Error creating holder"))
        }
        connect(hold,SIGNAL(realParamsChanged(float,float)),this,SIGNAL(realParamsChanged(float,float)));
    }

    return true;
}

void LayoutWidget::stateChanged ()
{
    _hasChanges = true;
}

bool LayoutWidget::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
      if(_editMode)
      {
          QPointF cur = _view->mapToScene(mapFromGlobal(QCursor::pos()));
          if(_poly.size()>0){
              QPolygonF pl= _poly;
              pl<<cur;
              QPainterPath path;
              path.addPolygon(pl);

              _polyitem->setPath(path);
          }
      }
  }
  return ZoomWidget::eventFilter(obj,event);
}

void LayoutWidget::mousePressEvent(QMouseEvent *event)
{
    if(Qt::RightButton==event->button())
    {
        _click = _view->mapToScene(mapFromGlobal(QCursor::pos()));
    }
    if(_editMode)
    {
        QPointF cur = _view->mapToScene(mapFromGlobal(QCursor::pos()));

        if(Qt::LeftButton==event->button())
        {
            _poly.push_back(cur);
            if(QLineF(_poly.first(),_poly.last()).length()<5&&_poly.size()>2){
                _poly.last() = _poly.first();
                setEditMode(false);
            }
        }

    }
    ZoomWidget::mousePressEvent(event);
}
void LayoutWidget::changeColor(int what)
{
    foreach(Holder* x,_holders.keys())
    {
 //Remove only Holder items
        if(x->isSelected())
        {
            QColor color;
            switch(what)
            {
                case passage: color = QColor(0,192,0,96); break;
                case obstacle: color = QColor(192,0,0,96); break;
                default: color = QColor(0,192,0,96);
            }
            x->setObjectBrush(QBrush(color));
           _hasChanges = true;
        }
    }
}
void LayoutWidget::edit()
{
    QMap<QString,Holder*> map;
    foreach(Holder* x,_holders.keys())
    {
        map.insert(x->objectName(),x);
    }

    //Get item at coordinates
    foreach(Holder* x,_holders.keys())
    {

        if(x->isSelected())
        {

            bool ok=true;
            QStringList list = map.keys();
            list.removeOne(x->objectName());

            //
            HolderDialog holder(list);
            holder.setName(x->objectName());
            holder.setSizeX(x->getRealX());
            holder.setSizeY(x->getRealY());

            if(QDialog::Accepted!=holder.exec())
            {
                ok =false;
            }

            QString text = holder.getName();
            if(ok&&!text.isEmpty())
            {
                x->setObjectName(text);
                if(!holder.getSourceObject().isEmpty())
                {
                    double p = (double)holder.getPercent()/100.0f;
                    Holder * h = map.value(holder.getSourceObject(),0);
                    if(h)
                    {
                        x->setScaleXPos(h->scaleX()*p);
                        x->setScaleYPos(h->scaleY()*p);
                        x->setRotationPos(h->rotationPos());
                    }
                }

                emit realParamsChanged(x->getPX(holder.getSizeX()),x->getPY(holder.getSizeY()));
            }
        }
    }
}

void LayoutWidget::remove()
{
    //Get item at coordinates
    foreach(Holder* x,_holders.keys())
    {
        if(x->isSelected())
        {
           _holders.remove(x);
           _scene->removeItem(x->getObject());
           _scene->removeItem(x);
           _hasChanges = true;

        }
    }
}
void LayoutWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        setEditMode(false);
        break;
    default:
       ZoomWidget::keyPressEvent(event);
    }
    update();

}

visual::ui::utils::Holder* LayoutWidget::actionTrigger(int what, QPolygonF mypolygon)
{

    QPointF p = _click;//_view->mapToScene(mapFromGlobal(QCursor::pos()));
    QAbstractGraphicsShapeItem* obj=0;
    QPainterPath piePath;
    QPolygonF pl;
    Holder* holder = 0;
    QStringList additional;
    switch(what)
    {
        case rect:   obj = new QGraphicsRectItem(QRectF(-100,-40,200,80));    break;
        case ellipse:obj = new QGraphicsEllipseItem(QRectF(-100,-40,200,80));   break;
        case sector:
        piePath.moveTo(0.0, 0.0);
        piePath.arcTo(-50.0, -50.0, 100.0,100.0, 90.0, 90.0);
        piePath.closeSubpath();      
        pl << QPointF(50,50);
        piePath.addPolygon(pl);
        obj = new QGraphicsPathItem(piePath);   break;
        case halfellipse:
        piePath.moveTo(0.0, 0.0);
        piePath.arcTo(-50.0, -50.0, 100.0,100.0, 0.0, 180.0);
        piePath.closeSubpath();
        pl << QPointF(50,50);
        piePath.addPolygon(pl);
        obj = new QGraphicsPathItem(piePath);   break;      
        case polygon:
        if(mypolygon.empty()){
            setEditMode(true);
            return 0;
        }else{
            QPointF center;
            foreach(QPointF pt,mypolygon)
            {
                center+=pt;
            }

            center/=mypolygon.size();
            p=center;
            QPolygonF centerd;
            foreach(QPointF pt,mypolygon)
            {
                centerd.push_back( pt-center);
                additional.push_back(QString("%1").arg(centerd.last().x()));
                additional.push_back(QString("%1").arg(centerd.last().y()));
            }

            piePath.addPolygon(centerd);
            obj = new QGraphicsPathItem(piePath);

            break;
        }


    }
    if(0!=obj)
    {
        obj->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        obj->setBrush(QColor(0,192,0,96));
        obj->setPos(p);
        _scene->addItem(obj);

		holder = new Holder(obj);
        if(!additional.isEmpty())
            holder->setAdditionalInfo(additional);
       // holder->setPos(p);
		//_scene->addItem(holder);
        connect(holder,SIGNAL(paramsChanged()),this,SLOT(stateChanged()));
        _hasChanges = true;
    }
    if(0!=holder)
    {
        _holders.insert(holder,what);
        connect(holder,SIGNAL(realParamsChanged(float,float)),this,SIGNAL(realParamsChanged(float,float)));
    }
    return holder;
}
bool LayoutWidget::hasChanges()
{
    return _hasChanges;
}
bool LayoutWidget::isInside(const QImage &img, double x, double y)
{
    QRgb pix = img.pixel(QPoint(x,y));
    if(0!=qRed(pix))
    {
        return true;
    }
    return false;
}
QStringList LayoutWidget::getHoldersNames()
{
    QStringList list;
    foreach(Holder* item,_holders.keys())
    {
        list.push_back(item->objectName());
    }
    return list;
}

bool LayoutWidget::isInside(QString name,double x,double y)
{
    foreach(Holder* item,_holders.keys())
    {

           if(name==item->objectName())
           {
               QImage img = renderScene(name);
               return isInside(img,x,y);
           //    return item->getObject()->contains(QPointF(x,y));
           }
    }
    return false;
}

QPointF LayoutWidget::getPosOfLayer(QString name)
{

    foreach(Holder* item,_holders.keys())
    {   
           if(name==item->objectName())
           {
                return item->pos();
           }
    }
    return QPointF();
}
QGraphicsItem* LayoutWidget::addMark(QPointF p,QString label,QColor color)
{
    QGraphicsItemGroup *ig = new QGraphicsItemGroup();
    ig->addToGroup(_scene->addEllipse(p.x()-5,p.y()-5,10,10,QPen(color),QBrush(color)));
    ig->addToGroup(_scene->addText(label));
    _scene->addItem(ig);
    ig->setVisible(true);
    //TODO: adding to array
    return ig;
}
QGraphicsItem* LayoutWidget::addVector(QPointF p,QPointF vec,QString label,QColor color)
{
    QGraphicsItemGroup *ig = (QGraphicsItemGroup*)addMark(p,label,color);
    ig->addToGroup(_scene->addLine(QLineF(p,p+vec),QPen(color)));
    return ig;
}

void LayoutWidget::removeMark(QGraphicsItem* mark)
{
    if(mark)
    {
        _scene->removeItem(mark);
        mark = 0;
    }else{
        foreach(QGraphicsItem* mark_id,_marks)
        {
            removeMark(mark_id);
            _marks.clear();
        }
    }
    //TODO: removing
}

QImage LayoutWidget::renderScene(QString layer)
{
    if(layer==_lastName&&!hasChanges()&&!_lastRender.isNull())
    {
        return _lastRender;
    }
    //Set back
    QBrush br;
    QBrush bkbr;
    if(!_rendersurface)
        return QImage();

    qreal zval = -15;
    if(_rendersurface)
    {
        zval = _rendersurface->zValue();
        _rendersurface->setZValue(-15);
        br = _rendersurface->brush();
        _rendersurface->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        _rendersurface->setBrush(Qt::black);
    }
    bkbr = _scene->backgroundBrush();

    this->_scene->setBackgroundBrush(Qt::black);


    foreach(QGraphicsItem* item,_scene->items())
    {
        Holder *x = dynamic_cast<Holder*>(item);
        if(0!=x)
        {

            x->setBinary(true);
            if(!layer.isEmpty()&&layer!=x->objectName())
            {
               x->getObject()->setVisible(false);
            }
        }
    }


    QImage image(_rendersurface->boundingRect().width(), _rendersurface->boundingRect().height(),QImage::Format_ARGB32);
    QPainter painter(&image);
    _scene->render(&painter,QRect(_rendersurface->pos().x(),_rendersurface->pos().y(),_rendersurface->boundingRect().width(), _rendersurface->boundingRect().height()));


    foreach(QGraphicsItem* item,_scene->items())
    {
        Holder *x = dynamic_cast<Holder*>(item);
        if(0!=x)
        {
            x->setBinary(false);
            if(!layer.isEmpty()&&layer!=x->objectName())
            {
                x->getObject()->setVisible(true);
            }
        }
    }

    if(_rendersurface)
    {    
        _rendersurface->setBrush(br);
        _rendersurface->setZValue(zval);
    }
    _scene->setBackgroundBrush(bkbr);

    _hasChanges = false;

    _lastName = layer;
    _lastRender = image;

    return image;
}

