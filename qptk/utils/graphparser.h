#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H
#include <interfaces.h>
#include <QString>

namespace base{
    class Graph;
    class Node;
}

namespace utils
{
/*!
 * \brief The GraphParser class
 * \deprecated
 */
class PROCESSINGFRAMEWORK_SHARED_EXPORT GraphParser
{
public:
    GraphParser();
protected:
    void parseGraph(base::Graph* graph);
    virtual void writeNode(int id,base::Node* node)=0;
    virtual void onConnection(int first,int second)=0;
    virtual void readNode(base::Node* node)=0;
};
}
#endif // GRAPHPARSER_H
