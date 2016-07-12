#include "linecounter.h"
#include <QScrollBar>

LineCounter::LineCounter(QTextEdit *parent) :
    QTextEdit(parent)
{
    _lineCount = 0;
    _owner = parent;

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(228, 228, 228));
    setPalette(p);
    setFixedWidth(40);
    setAlignment(Qt::AlignRight);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setReadOnly(true);
    setTextInteractionFlags(Qt::NoTextInteraction);

    connect(_owner, SIGNAL(textChanged()), this, SLOT(linesCount()));

}

void LineCounter::highlightLine(int line,QColor color)
{
    //TODO
}

void LineCounter::linesCount()
{
    QTextDocument *document = _owner->document();
    int numberLineCount = document->lineCount();


    // если пользователь стер одну строку,
    // или было добавлено более чем одна строка, то
    if (numberLineCount < _lineCount || (numberLineCount - _lineCount) > 1)
    {
        _lineCount = numberLineCount;

        clear();
        setAlignment(Qt::AlignRight);

        for (int i = 1; i <= _lineCount; ++i)
        {
            append(QString::number(i));
        }

        QScrollBar *numberScrollBar = verticalScrollBar();
        QScrollBar *textScrollBar   = _owner->verticalScrollBar();

        numberScrollBar->setSliderPosition(textScrollBar->sliderPosition());
    }
    else if (numberLineCount > _lineCount)
    {
         append(QString::number(numberLineCount));

        _lineCount = numberLineCount;
    }
}
