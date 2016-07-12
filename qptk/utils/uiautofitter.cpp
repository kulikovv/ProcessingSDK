#include "uiautofitter.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QWidget>
#include <QTabWidget>

#include <visual/imageview.h>
#include <base/graph.h>

using namespace utils;
using namespace visual;
using namespace base;
UIAutoFitter::UIAutoFitter(QWidget *main) :
    QObject(main)
{
    _mainframe = main;
    parseUI(_mainframe);
}
void UIAutoFitter::addGraph(Graph* gr)
{
   // connect(gr,SIGNAL(destroyed(QObject*)),this,SLOT(graphDestroyed(QObject*)));
   // connect(gr,SIGNAL(graphUpdated(Graph*)),this,SLOT(graphUpdate(Graph*)));

    setObjects(gr->nodes("all"));
}

void UIAutoFitter::graphUpdate(Graph * /*g*/)
{
    fitUI();
}

void UIAutoFitter::onDestroyed(QObject* g)
{
    _names.removeOne((VisualNode*)g);
}

void UIAutoFitter::setObjects(QList<base::Node*> nodes)
{
    foreach(Node* node,nodes)
    {
        if(!node)
        {
            continue;
        }
        VisualNode* view = qobject_cast<VisualNode*>(node);
        if(0!=view)
        {
            _names.push_back(view);
            connect(view,SIGNAL(destroyed(QObject*)),this,SLOT(onDestroyed(QObject*)));
        }
    }
    fitUI();
}


void UIAutoFitter::fitUI()
{
     //Get all visual nodes
        foreach(VisualNode* node,_names)
        {
            foreach(QString widgetname,_uimap.keys())
            {
                //If nodename == widget
                QStringList part = node->objectName().split("::");
                if(0==QString::compare(part.first(),widgetname))
                {
                   QWidget* w = _uimap.value(widgetname);

                   QWidget* subw = node->getWidget();

                   subw->setObjectName(node->objectName());
                   //Remplace that widget with node widget

                   if(0!=subw)
                   {

                       QTabWidget* tab = qobject_cast<QTabWidget*>(w);
                       if(!tab){
                           QHBoxLayout* lay = new QHBoxLayout();
                           delete w->layout();
                           w->setLayout(lay);
                           lay->addWidget(subw);
                       }else{
						   
                           tab->setFocusPolicy(Qt::NoFocus);
                           subw->setFocusPolicy(Qt::NoFocus);
                           if(node->tabOrder()>-1){
							   tab->insertTab(node->tabOrder(),subw,node->getIcon(),part.last());
						   }else{
							   tab->addTab(subw,node->getIcon(),part.last());
						   }
                       }
                       emit widgetAdded(part.last(),subw,w);
                   }
                }
            }
        }
}

void UIAutoFitter::parseUI(QWidget* root)
{
    if(0==root)
    {
        return;
    }

    if(!root->objectName().isEmpty())
    {
        _uimap.insert(root->objectName(),root);
    }
    foreach(QObject* obj,root->children())
    {
     //   qDebug() << obj->objectName();
        QWidget* wdj = qobject_cast<QWidget*>(obj);
        if(0!=wdj){
            parseUI(wdj);
        }
    }
}

