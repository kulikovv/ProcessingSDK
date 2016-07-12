#ifndef LINECOUNTER_H
#define LINECOUNTER_H

#include <QtWidgets/QTextEdit>
#include "ScriptEditor_global.h"

class SCRIPTEDITORSHARED_EXPORT LineCounter : public QTextEdit
{
    Q_OBJECT
public:
    explicit LineCounter(QTextEdit *parent = 0);
    
signals:
    
public slots:

private slots:
    //! Изменение в кол-ве строк
    void linesCount();
    /*!
    * \brief highlightLine подчеркивает строку
    * \param line строка
    * \param color цвет
    */
    void highlightLine(int line,QColor color=Qt::red);
private:
    QTextEdit* _owner;//Хозяин (строки которого мы считаем)
    int _lineCount;//Количество строк в родительском документе
};

#endif // LINECOUNTER_H
