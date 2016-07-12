#ifndef MYHIGHLIGHT_H
#define MYHIGHLIGHT_H

#include "ScriptEditor_global.h"
#include <QObject>
#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class QTextDocument;
class SCRIPTEDITORSHARED_EXPORT ScriptHighlight : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        ScriptHighlight(QTextDocument *parent = 0);
        void addKeyWord(QString newword);
        void addKeyWord(QStringList newword);
    protected:
        void highlightBlock(const QString &text);

    private:
        struct HighlightingRule
        {
            QRegExp pattern;
            QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;

        QRegExp commentStartExpression;
        QRegExp commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat functionFormat;


};

#endif // MYHIGHLIGHT_H
