#include "visualnode.h"
#include <QDebug>
#include <QWidget>
using namespace visual;

QWidget* VisualNode::_invisibleParent;


int VisualNode::tabOrder()
{
	return _taborder;
}
void VisualNode::setTabOrder(int order)
{
	_taborder = order;
}
VisualNode::VisualNode(QObject *parent):Node(parent)
{
	_taborder = -1;
    setNodeType("Visual");
    _widget=0;

    setEnable(true);
    setBehaviour(DoNothing);
    if(0==_invisibleParent)
    {
        _invisibleParent = new QWidget();
    }
    _invisibleParent->setVisible(false);
}
VisualNode::~VisualNode()
{
    //qDebug() << "VisualNode destroyed";
    QWidget * w = this->getWidget();
    if(0!=w)
    {
        delete w;
    }
}
QWidget* VisualNode::getWidget()
{
   return _widget;
}
int VisualNode::threadId() const
{
    return 0;
}
bool VisualNode::visible()
{
    QWidget * w = getWidget();
    if(0!=w)
    {
        return w->isVisible();
    }
    return false;
}
void VisualNode::reset()
{
    //By default do nothing
    return;
}

void VisualNode::setVisible(bool vis)
{
     QWidget * w = getWidget();
     if(0!=w)
     {
         w->setVisible(vis);
     }
}
