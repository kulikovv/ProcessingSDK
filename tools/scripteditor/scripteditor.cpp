#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <QLabel>
#include <QCompleter>
#include <QStringListModel>
#include <QMetaMethod>
#include <QInputDialog>
#include <QPushButton>
#include <QFileDialog>
#include <QScrollBar>
#include <QList>
#include <QtScript>
#include <QToolBar>
#include <QApplication>


#include "scriptworker.h"
#include "scripteditor.h"
#include "scripthighlight.h"
#include "scripttexteditor.h"
#include "linecounter.h"

Q_DECLARE_METATYPE(QList<double> )

#define SAFE_DELETE(x) if(x){delete x;x=0;}

QWidget* ScriptEditor::getControlPanel()
{
    if(over)
    {
        QWidget* w = new QWidget(this);
        w->setLayout(over);
        return w;
    }
    return NULL;
}

ScriptEditor* ScriptEditor::cloneEditor(QWidget* parent)
{
    ScriptEditor* se = new ScriptEditor(parent);

    copy(ScriptEditor::createEditor(),se);
    se->hideOutput(true);
    return se;
}




void ScriptEditor::copy(ScriptEditor* from,ScriptEditor* to)
{

    to->clear();
    QStringList keys = from->getScripts();
    foreach(QString name,keys)
    {
        to->setScript(from->getScript(name),name);
    }
}
void ScriptEditor::clear()
{
    this->editor->setPlainText("");
    this->scripts.clear();
}

ScriptEditor::ScriptEditor(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose,false);
    over = 0;
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new ScriptTextEditor;
    editor->setLineWrapMode(QTextEdit::NoWrap);
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    output= new QTextEdit;
    output->setReadOnly(true);
    editor->setFont(font);
    editor->setEnabled(false);

    connect(editor,SIGNAL(textChanged()),this,SLOT(onTextChanged()));

    highlight = new ScriptHighlight(editor->document());
    listview = new QListWidget;
    connect(listview,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    QHBoxLayout *lay = new QHBoxLayout;
    QVBoxLayout *lay2 = new QVBoxLayout;
   // lay2->addWidget(new QLabel(tr("Write script body here:")));


    numberPage = new LineCounter(editor);
    numberPage->setFont(font);

    QHBoxLayout *sublay = new QHBoxLayout;
    sublay->setSpacing(0);
    sublay->addWidget(numberPage);
    sublay->addWidget(editor);



    outpanel = new QWidget(this);
    QVBoxLayout* outlay = new QVBoxLayout(outpanel);
    outlay->addWidget(new QLabel(tr("Text output:")));
    outlay->addWidget(output);
    outpanel->setLayout(outlay);


    hideOutput(true);

    QScrollBar *numberScrollBar = numberPage->verticalScrollBar();
    QScrollBar *textScrollBar   = editor->verticalScrollBar();

    connect(textScrollBar, SIGNAL(valueChanged(int)), numberScrollBar, SLOT(setValue(int)));
    QToolBar *control = new QToolBar;

    QAction* runscript = new QAction(QIcon(":/scripts/run"),tr("Run"),this);
    connect(runscript,SIGNAL(triggered()),this,SLOT(run()));
    QAction* savescript = new QAction(QIcon(":/scripts/save"),tr("Save"),this);
    connect(savescript,SIGNAL(triggered()),this,SLOT(save()));
    QAction* newscript = new QAction(QIcon(":/scripts/new"),tr("New"),this);
    connect(newscript,SIGNAL(triggered()),this,SLOT(newScript()));
    QAction* removescript = new QAction(QIcon(":/scripts/remove"),tr("Remove"),this);
    connect(removescript,SIGNAL(triggered()),this,SLOT(removeScript()));

    QAction* hideout = new QAction(QIcon(":/scripts/output"),tr("Output"),this);
    hideout->setCheckable(true);
    connect(hideout,SIGNAL(toggled(bool)),this,SLOT(nhideOutput(bool)));

    QAction* abort = new QAction(QIcon(":/scripts/abort"),tr("Abort"),this);
    connect(abort,SIGNAL(triggered()),this,SLOT(stopExecuting()));

    QAction* clear = new QAction(QIcon(":/scripts/clear"),tr("Clear output"),this);
    connect(clear,SIGNAL(triggered()),output,SLOT(clear()));
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    addAction(runscript);
    addAction(savescript);
    addAction(newscript);
    addAction(removescript);
    addAction(hideout);
    addAction(abort);
    addAction(clear);

    control->addActions(this->actions());


  //  control->addWidget(newscript);
  //  control->addWidget(removescript);

    over = new QVBoxLayout;

    over->addWidget(listview);

    lay2->addWidget(control);
    lay2->addLayout(sublay,80);
    lay2->addWidget(outpanel,20);

    lay->addLayout(over,25);
    lay->addLayout(lay2,75);
    this->setLayout(lay);

    setScriptDir(QApplication::applicationDirPath());




    interpreter = new ScriptWorker();
    addObject(interpreter,"ScriptWorker","");
    connect(interpreter,SIGNAL(printSignal(QString)),this,SLOT(printText(QString)));
}

void ScriptEditor::setScriptDir(QString dir)
{
    listview->clear();
    scripts.clear();
    scriptdir = dir;

    QStringList scripts = ScriptWorker::getScripts(QApplication::applicationDirPath());
    foreach(QString script,scripts)
    {
        openScriptFile(scriptdir+"/"+script);
    }

}
void ScriptEditor::save()
{
    foreach(QString script,scripts.keys())
    {
        saveScriptFile(script);
    }
}

ScriptEditor::~ScriptEditor()
{

    SAFE_DELETE(interpreter);
    SAFE_DELETE(editor);
    SAFE_DELETE(output);
    SAFE_DELETE(listview);
}
void ScriptEditor::stopExecuting()
{
    interpreter->abortExecution();
}

void ScriptEditor::addObject(QObject* obj,QString name,QString decription)
{
    interpreter->addObject(obj,name);
    highlight->addKeyWord(interpreter->getKeywords());
    editor->setCompleter(new QCompleter(interpreter->getCompliterModel()));
}

void ScriptEditor::run()
{
    runScriptText(getScript());

}
void ScriptEditor::runScript(QString name)
{
    QString scriptText = scripts.value(name);
    if(!scriptText.isEmpty())
    {
        runScriptText(scriptText);
    }

}


void ScriptEditor::runScriptText(QString script)
{
    interpreter->runScript(script);
}
void ScriptEditor::removeScript()
{
   QListWidgetItem* item = listview->currentItem();
   if(item)
   {
       if(QMessageBox::Yes==QMessageBox::warning(0,tr("Delete script"),tr("Do you really want to delete script?"),QMessageBox::Yes,QMessageBox::No))
       {
            scripts.remove(item->text());
            QFile::remove(scriptdir+"/"+item->text()+".js");
            editor->clear();
            editor->setEnabled(false);
            delete item;
        }
   }
}


void ScriptEditor::newScript()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New Script"),
                                              tr("Script name:"), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
    {
        newScript(text);
    }
}

void  ScriptEditor::newScript(QString name)
{
    if(scripts.value(name).isEmpty())
    {
        QListWidgetItem* item = new QListWidgetItem(name,listview);
        scripts.insert(name,tr("//TODO Write your script here"));
        listview->setCurrentItem(item);
    }
}
void ScriptEditor::onTextChanged()
{
    if(listview->currentItem())
    {
        scripts.insert(listview->currentItem()->text(),getScript());
    }
}

void ScriptEditor::currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
 /*   if(0!=previous)
    {
        QString old = previous->data(Qt::DisplayRole).toString();
        if(!old.isEmpty())
        {
            scripts.insert(old,getScript());
        }
    }*/
    if(0!=current)
    {
        QString name = current->data(Qt::DisplayRole).toString();
        if(!name.isEmpty())
        {
            setScript(scripts.value(name));
        }
        editor->setEnabled(true);
    }


}



QString ScriptEditor::getScript(QString key)
{
    if(!key.isEmpty())
    {
        return scripts.value(key);
    }
    return editor->document()->toPlainText();
}




void ScriptEditor::setScript(QString script,QString key)
{
    if(!key.isEmpty())
    {
        new QListWidgetItem(key,listview);
        scripts.insert(key,script);
    }
    editor->document()->clear();
    editor->document()->setPlainText(script);

}
void ScriptEditor::printText(QString x)
{
    QString old = output->document()->toPlainText();
   // output->document()->setPlainText(old+QDateTime::currentDateTime().toString()+ ": "+x+"\n");
    output->document()->setPlainText(old+x+"\n");

}
void ScriptEditor::nhideOutput(bool hide)
{
    hideOutput(!hide);
}

void ScriptEditor::hideOutput(bool on)
{
    outpanel->setVisible(!on);
   // output->setVisible(!on);
}

ScriptEditor* ScriptEditor::script;

ScriptEditor* ScriptEditor::createEditor(QWidget *parent)
{
    if(!script)
    {
        script = new ScriptEditor(parent);
    }
    return script;
}

void ScriptEditor::deleteEditor()
{
    if(script)
    {
        delete script;
        script = NULL;
    }
}
QStringList  ScriptEditor::getScripts()
{
    if(0<listview->selectedItems().count())
    {
        scripts.insert(listview->selectedItems().first()->data(Qt::DisplayRole).toString(),getScript());
    }
    return scripts.keys();
}
void ScriptEditor::saveScriptFile(QString script)
{
    QFile file(scriptdir+"/"+script+".js");
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(0,tr("Cannot open file for writing: "),file.errorString());
        return;
    }

    QTextStream myfile(&file);

    myfile << scripts[script] <<endl;
}

void ScriptEditor::openScriptFile(QString path)
{
    QFileInfo fi(path);
    QString name = fi.baseName();
    QString data;
    if(!scripts.value(name).isEmpty())
    {
        QMessageBox::warning(0,tr("Error"),tr("Script with this name already exists."));
        return;
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0,tr("Cannot open file for reading: "),file.errorString());
        return;
    }

    QTextStream in(&file);



    data = in.readAll();

    new QListWidgetItem(name,listview);
    scripts.insert(name,data);

}
