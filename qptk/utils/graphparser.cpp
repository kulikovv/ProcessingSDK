#include "graphparser.h"
#include <base/graph.h>

using namespace utils;

GraphParser::GraphParser()
{
}
void GraphParser::parseGraph(base::Graph* graph)
{
    //Saving all nodes from proposed graph
    writeNode(-1,graph);
    foreach(base::Node* nd,graph->nodes())
    {
        base::Graph *subgraph = qobject_cast<base::Graph*>(nd);//Check if this node is subgraph
        if(0!=subgraph){
            parseGraph(subgraph);//Save the subgraph
        }
        writeNode(graph->nodeID(nd),nd);//Save element
    }

    foreach(base::Graph::Connection con,graph->getConnections())
    {
        onConnection(graph->nodeID(con.first),graph->nodeID(con.second));//Save connection
    }
}
