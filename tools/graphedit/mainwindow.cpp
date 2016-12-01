//Local include
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "node.h"
#include "link.h"
#include "crossplatform.h"
//Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QFileDialog>
#include <QMetaProperty>
#include <QSettings>
#include <QDir>
#include <QSignalMapper>
#include <QtWidgets/qmessagebox.h>


#include <synch/synchfactory.h>
#include <visual/visualnode.h>
#include <interfaces.h>
#include <scripteditor.h>
#include <utils/graphdom.h>
#include <utils/logs.h>
#include "properyagregator.h"
#include "PropertyDialog.h"

using namespace ui;
using namespace visual;
using namespace utils;
using namespace base;
using namespace synch;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    uix(new Ui::MainWindow),
    _proxyEditor(this)
{


    //Setting GUI
    uix->setupUi(this);
	pedit = new QPropertyEditorWidget(this);

    QHBoxLayout *lay = new QHBoxLayout();
    this->centralWidget()->setLayout(lay);
    lay->addWidget(&view,2);

    //Tools layout
    QWidget *wid = new QWidget();
    lay->addWidget(wid,1);
    QVBoxLayout *vlay = new QVBoxLayout();
    wid->setLayout(vlay);
    vlay->setMargin(0);
    //Nodes tree
    vlay->addWidget(&tree);
    tree.setHeaderLabel(tr("Libraries"));
    tree.setAlternatingRowColors(true);
    //Propeties
    vlay->addWidget(pedit);
    pedit->setAlternatingRowColors(true);
	

    connect(uix->actionExit,SIGNAL(triggered()),this,SLOT(close()));

    QObject::connect(&tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));

    view.setScene(_proxyEditor.getScene());

    connect(uix->actionRun,SIGNAL(triggered()),&_proxyEditor,SIGNAL(run()));
    connect(uix->actionStop,SIGNAL(triggered()),&_proxyEditor,SIGNAL(stop()));
    connect(uix->actionReset,SIGNAL(triggered()),&_proxyEditor,SIGNAL(reset()));
    connect(uix->actionStep,SIGNAL(triggered()),&_proxyEditor,SLOT(step()));
    connect(&_proxyEditor,SIGNAL(selectedObject(QObject*)),this,SLOT(onObjectSelection(QObject*)));
    connect(&_proxyEditor,SIGNAL(widgetAdded(QWidget*)),this,SLOT(onWidgetPreview(QWidget*)));
    connect(uix->actionRemove_Node,SIGNAL(triggered()),&_proxyEditor,SLOT(removeSelectedNode()));
    connect(uix->actionUnlink,SIGNAL(triggered()),&_proxyEditor,SLOT(unlinkSelectedNode()));
    connect(uix->actionExit,SIGNAL(triggered()),this,SLOT(close()));


    _menu = new QMenu(this);
    QDir dir;
    QStringList filetrs;
    filetrs << CrossPlatform::dynLibName("*");
    //Used nodes (mechanism for not coping names)

    QStringList used;

    view.setContextMenuPolicy(Qt::CustomContextMenu);
    connect(&view, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(onContextMenu(const QPoint&)));

    QMap<QString,QTreeWidgetItem*> roots;
    QMap<QString,QMenu*> menuroots;
    foreach (QString name,dir.entryList(filetrs)) {
        NodeLibrary* lib = GraphXml::loadNodeLibrary(name);
        if (lib) {
            QTreeWidgetItem* item=new QTreeWidgetItem(0);
            QMenu* prev = new QMenu(lib->getLibraryName(),_menu);
            _menu->addMenu(prev);
            item->setText(0,lib->getLibraryName());
            QStringList nodes = Node::getNodesNames();


            for (int i=0;i<nodes.size();i++) {
                if (!used.contains(nodes[i])) {
                    QStringList data = nodes[i].split("::",QString::SkipEmptyParts);
                    QTreeWidgetItem* preitem = item;
                    QMenu* premenu = prev;
                    int j=0;
                    QString path="";
                    for(j=0;j<data.size()-1;j++)
                    {
                        path = path+data[j];
                        if(!roots.contains(path))
                        {
                            QTreeWidgetItem* subitem=new QTreeWidgetItem(0);
                            QMenu* submenu = new QMenu(data[j],premenu);
                            premenu->addMenu(submenu);
                            subitem->setText(0,data[j]);
                            preitem->addChild(subitem);
                            preitem = subitem;
                            premenu = submenu;

                            roots[path]=subitem;
                            menuroots[path]=submenu;
                        }else{
                            preitem = roots[path];
                            premenu = menuroots[path];
                        }
                    }

                    QTreeWidgetItem* subitem=new QTreeWidgetItem(1);
                    QAction* submenu = new QAction(data[j],premenu);
                    submenu->setData(nodes[i]);
                    premenu->addAction(submenu);
                //    connect(submenu,SIGNAL(triggered()),
                    subitem->setText(0,data[j]);
                    subitem->setData(0,Qt::UserRole+1,nodes[i]);
                    preitem->addChild(subitem);

                }
            }
            used << nodes;

            tree.addTopLevelItem(item);
        }
    }

    _filename = "";
    this->setWindowTitle(tr("GraphEdit - [Untitled]"));

    QSettings settings("AISoftPro", QApplication::applicationName());
    _lastdir = settings.value("LastDir").toString();
    settings.beginGroup("Files");
    _fileList = settings.value("FileList").toStringList();
    settings.endGroup();
    connect(&_mapper,SIGNAL(mapped(QString)),this,SLOT(loadFile(QString)));
    QMenu* recent = uix->menuFile->addMenu("Recent");
    foreach(QString path,_fileList)
    {
        QAction* file = new QAction(path,this);
        recent->addAction(file);
        connect(file, SIGNAL(triggered()), &_mapper, SLOT(map()));
        _mapper.setMapping(file, path);
    }
}



void MainWindow::onItemDoubleClicked(QTreeWidgetItem *item, int /*value*/)
{
    if (1==item->type()){
        ui::NodeItem* node = _proxyEditor.addItem(item->data(0,Qt::UserRole+1).toString());

        if(uix->actionInsert_Line->isChecked())
        {
            uix->actionInsert_Line->trigger();
        }
    }
}

void MainWindow::onObjectSelection(QObject* obj)
{
    pedit->setObject(obj);
}

void MainWindow::onWidgetPreview(QWidget* preview)
{
    uix->previewlay->addWidget(preview);
}

MainWindow::~MainWindow()
{
    QSettings settings("AISoftPro", QApplication::applicationName());
    settings.beginGroup("Files");
    settings.setValue("FileList",_fileList);
    settings.endGroup();
    delete uix;


    ScriptEditor::deleteEditor();
//	delete uix;
}

void MainWindow::on_actionInsert_Line_triggered(bool checked)
{
    if (checked){
        _proxyEditor.getScene()->setMode(EditorScene::InsertLine);
    }else{
        _proxyEditor.getScene()->setMode(EditorScene::MoveItem);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dlg;
    QString name = dlg.getOpenFileName(0,tr("Open file"),_lastdir,"*.xml");
    if(!name.isEmpty())
    {
        _lastdir = QFileInfo(name).dir().path();
        QSettings settings("AISoftPro", QApplication::applicationName());
        settings.setValue("LastDir",_lastdir);
    }
    loadFile(name);
}


void MainWindow::saveFile(QString name)
{
    if (!name.isEmpty()) {

        //Building list of used libraries
        if(!name.contains(".xml"))
        {
            name = name + ".xml";
        }
        //Get decor
        _filename = name;
        this->setWindowTitle(QString(tr("GraphEdit - [%1]")).arg(_filename));

        QFile file(name);
        file.open(QFile::WriteOnly);

        QFileInfo finfo(file);
        QDir::setCurrent(finfo.dir().path());

        QTextStream ts(&file);
        ts << _proxyEditor.saveGraph();
    }
}

void MainWindow::loadFile(QString name)
{
    if (!name.isEmpty()&&QFile::exists(name)) {

        //Nodes coordinates
        QFile file(name);
        file.open(QFile::ReadOnly);
        QTextStream ts(&file);

        QFileInfo finfo(file);
        QDir::setCurrent(finfo.dir().path());

        if(!_proxyEditor.loadGraph(ts.readAll()))
        {
            QMessageBox::critical(0,tr("Error"),tr("Some error occured!"));
            return;
        }

        _filename = name;
        this->setWindowTitle(QString(tr("GraphEdit - [%1]")).arg(_filename));


        if(!_fileList.contains(name))
        {
            if(_fileList.count()>5){
                _fileList.removeAt(0);
            }
            _fileList.push_back(name);
        }

    }
}

void MainWindow::on_actionSave_triggered()
{
    if (!_filename.isEmpty()){
        saveFile(_filename);
    }else{
        on_actionSaveAs_triggered();
    }
}

void MainWindow::on_actionShow_Preview_triggered()
{
    uix->preview->show();
}

void MainWindow::on_actionScreen_shot_triggered()
{
    QFileDialog dlg;
    QString name = dlg.getSaveFileName(0,QString(),QString(),"*.png");
    if (!name.isEmpty()&&view.scene()) {

        QImage* image=new QImage(view.scene()->width(), view.scene()->height(),QImage::Format_ARGB32);

        QPainter painter(image);
        view.scene()->render(&painter);
        image->save(name);
    }

}

void MainWindow::on_actionNew_triggered()
{
    pedit->setObject(0);

    _proxyEditor.clear();

    _filename = "";
    this->setWindowTitle(tr("GraphEdit - [Untitled]"));

}



void MainWindow::on_actionSaveAs_triggered()
{
    QFileDialog dlg;
    QString name = dlg.getSaveFileName(0,QString(),_lastdir,"*.xml");
    if(!name.isEmpty())
    {
        _lastdir = QFileInfo(name).dir().path();
        QSettings settings("AISoftPro", QApplication::applicationName());
        settings.setValue("LastDir",_lastdir);
    }
    saveFile(name);
}

void MainWindow::on_actionScientific_pictures_triggered()
{
    _proxyEditor.getGraph()->userCommand("SciencePicture","");
}

void MainWindow::on_actionScript_Editor_triggered()
{
    ScriptEditor *se = ScriptEditor::createEditor(0);
    se->resize(800,600);
    se->show();

    foreach(base::Node* node,_proxyEditor.getGraph()->nodes())
    {
        se->addObject(node,node->objectName());
    }
}

void MainWindow::on_addBaseGraph_triggered()
{
    QFileDialog dlg;
    QString name = dlg.getOpenFileName(0,QString(),_lastdir,"*.xml");
    if(!name.isEmpty())
    {
        _lastdir = QFileInfo(name).dir().path();
        QSettings settings("AISoftPro", QApplication::applicationName());
        settings.setValue("LastDir",_lastdir);
    }

    //Nodes coordinates
    QMap<QString,NodeDecoration> coords;

    synch::Synch* graph = GraphDom::loadGraph(name,coords);

    foreach(QString node,coords.keys())
    {
        coords[node].base = true;
    }

    _proxyEditor.addBaseGraph(name,graph,coords);

}

void MainWindow::onContextMenu(const QPoint& pos)
{
    QAction* action = _menu->exec(view.mapToGlobal(pos));
    if(action)
    {
        ui::NodeItem* node = _proxyEditor.addItem(action->data().toString());
        if(node)
        {

            node->setPos(view.mapToScene(pos));
        }
    }
}



void MainWindow::on_actionRead_Log_triggered()
{
   int w = utils::Logger::getMessages(QtWarningMsg).count();

   int c = utils::Logger::getMessages(QtCriticalMsg).count();
   QMessageBox::information(0,tr("Info"),QString("Warnings: %1 Critical %2").arg(w).arg(c),QMessageBox::Ok,QMessageBox::Cancel);
}

void MainWindow::on_actionAggrigate_Properies_triggered()
{
   // QList<QObject*> o = _proxyEditor.getGraph()->nodes();
    QList<QObject*> configObject;
    configObject << _proxyEditor.getGraph();
    foreach(base::Node* node,_proxyEditor.getGraph()->nodes())
    {
        //0==qobject_cast<base::Graph*>(node)&&

        if(!node->objectName().contains(".xml"))
        {
            configObject << node;
           // icons.insert(node,node->getIcon());
        }
    }
    /*
    ProperyAgregator pa(configObject);
    pa.setObjectName("AAA");
    QList<QObject*> obj;
    obj << &pa;
    PropertyDialog dlg(obj);
    if(QDialog::Accepted==dlg.exec())
    {
        pa.accept();
    }*/
    PropertyDialog dlg(configObject,QMap<QObject*,QIcon>(),PropertyDialog::RESUME);
    dlg.exec();

}
