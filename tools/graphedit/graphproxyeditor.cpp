#include "graphproxyeditor.h"
#include "node.h"
#include <visual/visualnode.h>
#include <synch/synchfactory.h>
#include <utils/graphdom.h>
#include "link.h"

using namespace ui;
using namespace visual;

QMap<QString,QColor> colors;

GraphProxyEditor::GraphProxyEditor(QObject *parent) :
    QObject(parent)
{

    //New node graph
    _synch = synch::SynchFactory::createSynch();

    initConnections();

    colors.insert("Visual",QColor(229,83,34));
    colors.insert("Source",QColor(209,149,82));
    colors.insert("Output",QColor(184,191,115));
    colors.insert("Process",QColor(182,219,131));
    colors.insert("Node",QColor(255,249,145));
    colors.insert("Workflow",QColor(255,0,255));
    colors.insert("Synch",QColor(0,128,128));


    _scene.setSceneRect(-500,-500,1000,1000);
    _scene.setBackgroundBrush(QPixmap(":/images/bg3"));
}

QString GraphProxyEditor::saveGraph()
{
    QMap<QString,utils::NodeDecoration> dec = getDecoration();
    return utils::GraphDom::saveGraphMemory(_synch,dec);
}

void GraphProxyEditor::initConnections()
{
    connect(this,SIGNAL(run()),_synch,SLOT(start()));
    connect(this,SIGNAL(stop()),_synch,SLOT(stop()));
    connect(this,SIGNAL(pause()),_synch,SLOT(pause()));
    connect(this,SIGNAL(reset()),_synch,SLOT(reset()));
    connect(&_scene,SIGNAL(selectionChanged()),this,SLOT(onSelection()));
    connect(&_scene,SIGNAL(linked(QString,QString)),_synch,SLOT(link(QString,QString)));
}

bool GraphProxyEditor::loadGraph(QString planexml)
{
    QMap<QString,utils::NodeDecoration> decor;
    synch::Synch *temp  = utils::GraphDom::loadGraphMemory(planexml,decor);
    if(!temp)
    {
        return false;
    }

    _scene.clear();
    if (_synch) {
        delete _synch;
        _synch=0;
    }

    _synch = temp;

    initConnections();
    assignNodes(_synch,decor);
    return true;
}

void GraphProxyEditor::assignNodes(base::Graph *graph,QMap<QString,utils::NodeDecoration> decor)
{
    QMap<Node*,NodeItem*> nodes;
    //Add nodes
    foreach (Node* node,graph->nodes("all")) {
        VisualNode* vis=qobject_cast<VisualNode*>(node);
        if (0!=vis) {
            if (vis->getWidget())
            {
                emit widgetAdded(vis->getWidget());
            }
        }
        Graph* subgraph =qobject_cast<Graph*>(node);
        if(subgraph)
        {
            continue;
        }

        if(!_scene.contains(node->objectName()))
        {
            NodeItem* item= new NodeItem(node->objectName(),node->metaObject()->className());
            connect(item,SIGNAL(nameChanged(QString)),this,SLOT(nodeNameChanged(QString)));

            if(decor.contains(item->getText()))
            {
                item->setPos(decor[item->getText()].place);
                QColor color = colors.value(node->getNodeType());

                if(decor[item->getText()].base)
                {
                    color.setAlpha(128);
                  //  item->setEnabled(false);
                    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIgnoresTransformations| QGraphicsItem::ItemSendsGeometryChanges);
                }
                item->setColor(color);
                item->setIcon(node->getIcon());
            }
            //Accosiate nodes with items
            nodes.insert(node,item);

            _scene.addItem(item);
        }



    }
    //Draw connection
    foreach (Graph::Connection con,graph->getConnections())
    {
        if(0==nodes.value(con.first))
        {
            qDebug() << "Node not found for"<<con.first->objectName();
            continue;
        }

        if(0==nodes.value(con.second))
        {
            qDebug() << "Node not found for"<<con.second->objectName();
            continue;
        }
        _scene.addItem(new Link(nodes.value(con.first),nodes.value(con.second)));
    }

}

/*
void GraphProxyEditor::setGraph(synch::Synch* graph,QMap<QString,utils::NodeDecoration> decor)
{
    scene.clear();
    if (_synch) {
        delete _synch;
        _synch=0;
    }

    QMap<Node*,NodeItem*> nodes;
    //Add nodes
    qDebug()<<"Assign nodes";
    foreach (Node* node,graph->nodes("all")) {
        VisualNode* vis=qobject_cast<VisualNode*>(node);
        if (0!=vis) {
            if (vis->getWidget())
            {
                emit widgetAdded(vis->getWidget());
             //   uix->previewlay->addWidget(vis->getWidget());
            }
        }
        Graph* subgraph =qobject_cast<Graph*>(node);
        if(subgraph)
        {
            continue;
        }

        NodeItem* item= new NodeItem(node->objectName(),node->metaObject()->className());
        connect(item,SIGNAL(nameChanged(QString)),this,SLOT(nodeNameChanged(QString)));

        if(decor.contains(item->getText()))
        {
            item->setPos(decor[item->getText()].place);
            QColor color = colors.value(node->getNodeType());

            if(decor[item->getText()].base)
            {
                color.setAlpha(128);
              //  item->setEnabled(false);
                item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIgnoresTransformations| QGraphicsItem::ItemSendsGeometryChanges);
            }
            item->setColor(color);
        }


        //Accosiate nodes with items
        nodes.insert(node,item);
        scene.addItem(item);
    }
    //Draw connection
    foreach (Graph::Connection con,graph->getConnections())
    {
        if(0==nodes.value(con.first))
        {
            qDebug() << "Node not found for"<<con.first->objectName();
            continue;
        }

        if(0==nodes.value(con.second))
        {
            qDebug() << "Node not found for"<<con.second->objectName();
            continue;
        }
        scene.addItem(new Link(nodes.value(con.first),nodes.value(con.second)));
    }

    _synch = graph;
}*/

GraphProxyEditor::~GraphProxyEditor()
{
    if (0!=_synch) {
        delete _synch;
        _synch=0;
    }
}

void GraphProxyEditor::removeSelectedNode()
{
    QList<QGraphicsItem*> data= _scene.selectedItems();
    foreach (QGraphicsItem* item, data) {
        NodeItem* nd = dynamic_cast<NodeItem*>(item);
        if (0!=nd) {
            if(""==_synch->nodeOwner(_synch->node(nd->getText())))
            {
                _synch->remove(_synch->node(nd->getText()));
            }else{
                return;
            }
        }
        delete item;
    }
}

void GraphProxyEditor::unlinkSelectedNode()
{
    QList<QGraphicsItem*> data= _scene.selectedItems();
    foreach (QGraphicsItem* item, data) {

        NodeItem* nd = dynamic_cast<NodeItem*>(item);
        //Delete all links
        if (0!=nd) {
           nd->unlink();
           _synch->unlink(nd->getText());
        }
    }
}

bool GraphProxyEditor::nodeNameChanged(QString name)
{
    ui::NodeItem *item = qobject_cast<ui::NodeItem*>(this->sender());
    if(0!=item)
    {
        _synch->node(item->getText())->setObjectName(name);
        return true;
    }
    return false;

}

void GraphProxyEditor::step(){
    if(_synch){

        _synch->step();
    }
}

NodeItem* GraphProxyEditor::addItem(QString classname)
{
    Node* instance = Node::classForName(classname);
    if (instance) {
        int id = _synch->add(instance);
        instance->setObjectName(QString("%1_%2").arg(classname).arg(id));
        if (0!=id) {
            VisualNode* vis=qobject_cast<VisualNode*>(instance);
            NodeItem* node= new NodeItem(QString("%1_%2").arg(classname).arg(id),classname);

            connect(node,SIGNAL(nameChanged(QString)),this,SLOT(nodeNameChanged(QString)));
            node->setColor(colors.value(instance->getNodeType()));
            node->setIcon(instance->getIcon());
            if(vis)
            {
                emit widgetAdded(vis->getWidget());
            }
            _scene.addItem(node);
            _scene.setMode(EditorScene::MoveItem);
            return node;
        }
    }
    return 0;
}

void GraphProxyEditor::onSelection()
{
    QList<QGraphicsItem*> data= _scene.selectedItems();
    foreach (QGraphicsItem* item,data)
    {
        NodeItem* nd = dynamic_cast<NodeItem*>(item);
        if (0!=nd)
        {
            emit selectedObject(_synch->node(nd->getText()));
            return;
        }
    }
    emit selectedObject(_synch);
}

void GraphProxyEditor::addBaseGraph(QString name,synch::Synch* graph,QMap<QString,utils::NodeDecoration> decor)
{
    if(graph)
    {
        _synch->addGraph(graph,name);
         assignNodes(_synch,decor);
    }
}

synch::Synch *GraphProxyEditor::getGraph()
{
    return _synch;
}

ui::EditorScene* GraphProxyEditor::getScene(){
    return &_scene;
}

void GraphProxyEditor::clear()
{
    _scene.clear();
    if(_synch)
    {
        delete _synch;
        _synch = 0;
    }
    _synch = synch::SynchFactory::createSynch();
    initConnections();
}

QMap<QString,utils::NodeDecoration> GraphProxyEditor::getDecoration()
{
    QList<QGraphicsItem*> items= _scene.items();
    QMap<QString,utils::NodeDecoration> coords;
    foreach (QGraphicsItem* item, items) {
        NodeItem* nd = dynamic_cast<NodeItem*>(item);
        if (0!=nd)
        {
            coords.insert(nd->getText(),utils::NodeDecoration(nd->pos()));
        }

    }
    return coords;
}
