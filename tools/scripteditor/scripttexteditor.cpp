#include "scripttexteditor.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

ScriptTextEditor::ScriptTextEditor(QWidget *parent) :
    QTextEdit(parent)
{
    c=0;
    _fullname="";
}

ScriptTextEditor::ScriptTextEditor(QString fname,QWidget *parent):
    QTextEdit(parent)
{
    _fullname = fname;
    c=0;
    QFile file(_fullname);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot read script file";
        return;
    }
    QTextStream in(&file);
    setPlainText(in.readAll());
}

ScriptTextEditor::~ScriptTextEditor()
{
    //save();
}

void ScriptTextEditor::save()
{
    if(!_fullname.isEmpty())
    {
        QFile file(_fullname);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Cannot write script file";
            return;
        }

        QTextStream myfile(&file);
        myfile << document()->toPlainText() <<endl;
    }
}

void ScriptTextEditor::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);


    QObject::connect(c, SIGNAL(activated(const QString&)),
                     this, SLOT(insertCompletion(const QString&)));
}

QCompleter *ScriptTextEditor::completer() const
{
    return c;
}

void ScriptTextEditor::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);

}
//! [4]

//! [5]
QString ScriptTextEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    QString plain = this->toPlainText();
    int i=tc.position()-1;
    for(;i>=0;i--)
    {
        if(' '==plain[i]||'\n'==plain[i]||'\t'==plain[i])
            break;
    }
    i = i+1;
    return plain.mid(i,tc.position()-i);
}

void ScriptTextEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void ScriptTextEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // dont process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  /*  if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;*/

    static QString eow("~!@#$%^&*_+{}|:\"<>?,;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();
    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1 || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {

        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}

void ScriptTextEditor::highlightLine(int line)
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::red).lighter(160);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor.setPosition(line);
        // selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }

     setExtraSelections(extraSelections);
 }