#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
namespace ui
{
class EditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertItem, InsertLine, MoveItem };
    explicit EditorScene(QObject *parent = 0);
    bool contains(QString text);
signals:
    bool linked(int first,int second);
    bool linked(QString first,QString second);
public slots:
    void setMode(Mode mode);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
private:
    QGraphicsLineItem *line;//Линяя
    Mode _mode;
};
}

#endif // EDITORSCENE_H
