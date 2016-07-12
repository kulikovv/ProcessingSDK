#include "editorscene.h"
#include "node.h"
#include "link.h"
#include <QMimeData>
using namespace ui;
EditorScene::EditorScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setMode(MoveItem);
} 
void EditorScene::setMode(Mode mode)
{
    _mode = mode;
}
bool EditorScene::contains(QString text)
{
    foreach(QGraphicsItem* item,items())
    {
        NodeItem* nd = dynamic_cast<NodeItem*>(item);
        if (0!=nd) {
            if(text==nd->getText())
                return true;
        }
    }
    return false;
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (mouseEvent->button() != Qt::LeftButton)
         return;
     switch (_mode) {
         case InsertItem:
             break;
         case InsertLine:
             line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                         mouseEvent->scenePos()));
             line->setPen(QPen(Qt::black, 2));
             addItem(line);
             break;
     default:
         ;
     }
     QGraphicsScene::mousePressEvent(mouseEvent);
 }

 void EditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (_mode == InsertLine && line != 0) {
         QLineF newLine(line->line().p1(), mouseEvent->scenePos());
         line->setLine(newLine);
     } else if (_mode == MoveItem) {
         QGraphicsScene::mouseMoveEvent(mouseEvent);
     }
 }

 void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (line != 0 && _mode == InsertLine) {
         QList<QGraphicsItem *> startItems = items(line->line().p1());
         if (startItems.count() && startItems.first() == line)
             startItems.removeFirst();
         QList<QGraphicsItem *> endItems = items(line->line().p2());
         if (endItems.count() && endItems.first() == line)
             endItems.removeFirst();

         removeItem(line);
         delete line;

         if (startItems.count() > 0 && endItems.count() > 0 &&
             startItems.first() != endItems.first()) {
             NodeItem *startItem =
                 qgraphicsitem_cast<NodeItem *>(startItems.first());
             NodeItem *endItem =
                 qgraphicsitem_cast<NodeItem *>(endItems.first());
            /* if(emit linked(startItem->getID(),endItem->getID()))
             {
                this->addItem(new Link(startItem,endItem));
             }*/
             if(emit linked(startItem->getText(),endItem->getText()))
             {
                 this->addItem(new Link(startItem,endItem));
             }
         }
     }
     line = 0;
     QGraphicsScene::mouseReleaseEvent(mouseEvent);
 }

