#ifndef PROCESSINGFRAMEWORK_GraphXml_H
#define PROCESSINGFRAMEWORK_GraphXml_H

#include <QStringList>
#include <QPair>
#include <utils/graphparser.h>
class QDir;
class QXmlStreamReader;
class QXmlStreamWriter;
class QPointF;

namespace synch{
    class Synch;
}
/*! \brief Вспомогательные классы, упрощающие работу с qptk
  В пространсве имен есть классы для упрощения работы с графическим интерфейсом, сохранением состояния и т.д.
    \deprecated
*/
namespace utils
{

struct NodeDecoration
{
    NodeDecoration()
    {
        place = QPointF(0,0);
        base = false;
    }

    NodeDecoration(QPointF pl)
    {
        place = pl;
        base = false;
    }

    QPointF place;
    bool base;
};


static QMap<QString,NodeDecoration> AVAL;
static QStringList LIBS;



class PROCESSINGFRAMEWORK_SHARED_EXPORT GraphXml: public GraphParser
{
public:
    explicit GraphXml();
    synch::Synch* createGraph(QString xmlname,QMap<QString,NodeDecoration> &decor=AVAL);
    static NodeLibrary* loadNodeLibrary(QString name);
    virtual void storeGraph(QString xmlname,base::Graph* graph,QList<QPair<QString,QPointF> > decor=QList<QPair<QString,QPointF> >());
private:
    QXmlStreamReader* _reader;
    QXmlStreamWriter* _writer;
    void skipUnknownElement(QXmlStreamReader* reader);
protected:
    virtual void writeNode(int id,base::Node* node);
    virtual void onConnection(int first,int second);
    virtual void readNode(base::Node* node);
};
}
#endif // PROCESSINGFRAMEWORK_GraphXml_H
