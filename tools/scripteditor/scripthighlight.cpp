#include "scripthighlight.h"

#include <QTextDocument>
void ScriptHighlight::addKeyWord(QString newword)
{
    HighlightingRule rule;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\b"+newword+"\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);
}
void ScriptHighlight::addKeyWord(QStringList newword)
{
    foreach(QString word,newword)
    {
        addKeyWord(word);
    }
}

ScriptHighlight::ScriptHighlight(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\babstract\\b"
    << "\\bboolean\\b"
    << "\\bbreak\\b"
    << "\\bbyte\\b"
    << "\\bcase\\b"
    << "\\bcatch\\b"
    << "\\bchar\\b"
    << "\\bclass\\b"
    << "\\bconst\\b"
    << "\\bcontinue\\b"
    << "\\bdebugger\\b"
    << "\\bdefault\\b"
    << "\\bdelete\\b"
    << "\\bdo\\b"
    << "\\bdouble\\b"
    << "\\belse\\b"
    << "\\benum\\b"
    << "\\bexport\\b"
    << "\\bextends\\b"
    << "\\bfalse\\b"
    << "\\bfinal\\b"
    << "\\bfinally\\b"
    << "\\bfloat\\b"
    << "\\bfor\\b"
    << "\\bfunction\\b"
    << "\\bgoto\\b"
    << "\\bif\\b"
    << "\\bimplements\\b"
    << "\\bimport\\b"
    << "\\bin\\b"
    << "\\binstanceof\\b"
    << "\\bint\\b"
    << "\\binterface\\b"
    << "\\blong\\b"
    << "\\bnative\\b"
    << "\\bnew\\b"
    << "\\bnull\\b"
    << "\\bpackage\\b"
    << "\\bprivate\\b"
    << "\\bprotected\\b"
    << "\\bpublic\\b"
    << "\\breturn\\b"
    << "\\bshort\\b"
    << "\\bstatic\\b"
    << "\\bsuper\\b"
    << "\\bswitch\\b"
    << "\\bsynchronized\\b"
    << "\\bthis\\b"
    << "\\bthrow\\b"
    << "\\bthrows\\b"
    << "\\btransient\\b"
    << "\\btrue\\b"
    << "\\btry\\b"
    << "\\btypeof\\b"
    << "\\bvar\\b"
    << "\\bvoid\\b"
    << "\\bvolatile\\b"
    << "\\bwhile\\b"
    << "\\bwith\\b";
  /*  keywordPatterns << "\\bvar\\b" << "\\bfunction\\b" << "\\bthis\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\breturn\\b" << "\\bnew\\b";*/
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }




    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void ScriptHighlight::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
