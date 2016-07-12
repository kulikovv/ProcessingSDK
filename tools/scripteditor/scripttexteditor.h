#ifndef MYTEXTEDITOR_H
#define MYTEXTEDITOR_H

#include "ScriptEditor_global.h"
#include <QtWidgets/QTextEdit>
QT_BEGIN_NAMESPACE
class QCompleter;
QT_END_NAMESPACE
class  SCRIPTEDITORSHARED_EXPORT ScriptTextEditor : public QTextEdit
{
Q_OBJECT
public:
    explicit ScriptTextEditor(QWidget *parent = 0);
    explicit ScriptTextEditor(QString fname,QWidget *parent = 0);
    ~ScriptTextEditor();

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

    void highlightLine(int line);
protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private slots:
    void save();
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

private:
    QCompleter *c;
    QString _fullname;

};

#endif // MYTEXTEDITOR_H
