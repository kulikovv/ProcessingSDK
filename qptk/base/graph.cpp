#include "graph.h"
#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QMetaType>

using namespace base;

Graph::Graph(QObject *parent) :
    Node(parent)
{
    qRegisterMetaType<timestamp>("timestamp");
    qRegisterMetaType<base::timestamp>("base::timestamp");
    qRegisterMetaType<esp>("esp");
    setNodeType("Graph");
    setEnable(true);
    _lastid=0;
    _allowCyclic = true;
}
Graph::~Graph(){
   foreach(Node* node,_nodes.values())
   {
       if(!node)
       {
           continue;
       }
       if(nodeOwner(node).isEmpty())
       {
           delete node;
       }
       node = 0;
   }
}

void Graph::reset()
{

    foreach(Node* node,_nodes.values())
    {
        if(!node)
        {
            continue;
        }
        if(0==qobject_cast<Graph*>(node)){
            node->reset();
        }
    }
}

double Graph::globalExecutionTime()
{
    double maxTotalLag=0;
    foreach(Node* node,_nodes.values())
    {
        if(!node)
        {
            continue;
        }
        if(node->enable())
        {
            if(maxTotalLag<node->globalExecutionTime())
            {
                maxTotalLag = node->globalExecutionTime();
            }
        }
    }
    return maxTotalLag;
}
double Graph::estimatedFps()
{
    return 1000.0f/globalExecutionTime();
    /*double totalLag=0;
    foreach(Node* node,_nodes.values())
    {
        totalLag+= node->nodeExecutionTime();
    }
    return totalLag;*/
}

const QList<Graph::Connection> Graph::getConnections()
{
    return _connections;
}

bool Graph::unlink(QString name)
{
    return unlink(node(name));
}
bool Graph::unlink(Node* node)
{
    if(0!=node)
    {
        foreach(Graph::Connection conn,_connections)
        {
            if(conn.second==node)
            {
                conn.first->disconnect(node);
                 _connections.removeOne(conn);
            }
            if(conn.first==node)
            {
                _connections.removeOne(conn);
            }
        }

        disconnect(node,SLOT(receive(esp)));//We disconnect only graph send/recive connection
    }
    return true;
}

bool Graph::unlink(int id)
{
    return unlink(node(id));
}

bool Graph::link(int in,int out)
{

    return link(_nodes.value(in),_nodes.value(out));
}
bool Graph::link(QString in,QString out)
{
    return link(node(in),node(out));
}

bool Graph::addGraph(Graph* graph,QString source)
{
    graph->setObjectName(source);
    add(graph);

    foreach(Node* node,graph->nodes("all"))
    {
     //   graph->disconnect(node);
     //   QObject::disconnect(node,SLOT(receive(esp)));
    //    qDebug() << "Disconnected" << node->objectName();
        add(node,source);
    }
    foreach(Graph::Connection connection,graph->getConnections())
    {
        graph->unlink(connection.first);
        graph->unlink(connection.second);
        _connections.push_back(connection);
        connection.first->onConnected();
   //     link(connection.first,connection.second);
    }

    return true;
}

int Graph::add(Node* node, QString source)
{
    if(_nodes.values().contains(node)){
        return 0;
    }

    if(!source.isEmpty())
    {
        _externalNodes[node] = source;
    }

    QObject::disconnect(node,SLOT(userCommand(QVariant,QVariant)));
    if(0==qobject_cast<Graph*>(node)){
        connect(this,SIGNAL(userCommandSignal(QVariant,QVariant)),node,SLOT(userCommand(QVariant,QVariant)));
    }
    connect(node,SIGNAL(status(QString,unsigned long,Node::Status)),this,SIGNAL(status(QString,unsigned long,Node::Status)));

    //asyncNode(node);
    _lastid+=1;
    _nodes.insert(_lastid,node);

    return _lastid;
}

QString Graph::nodeOwner(Node* pNode)
{
    return _externalNodes.value(pNode,QString());
}

Node* Graph::node(QString nodename)
{
    foreach(Node* node,_nodes.values())
    {
        if(!node)
        {
            continue;
        }
        if(0==QString::compare(node->objectName(),nodename))
        {
            return node;
        }
    }
    return 0;
}

Node* Graph::node(int id)
{
    return _nodes.value(id,0);
}
int Graph::nodeID(Node* node)
{
    return _nodes.key(node,-1);
}

bool Graph::remove(Node* node)
{
     unlink(node);
    _nodes.remove(_nodes.key(node));
    if(!nodeOwner(node).isEmpty())
    {
        _externalNodes.remove(node);
        return true;
    }
    delete node;
    return true;
}

bool Graph::allowCyclic()
{
    return _allowCyclic;
}

void Graph::setAllowCyclic(bool value)
{
    _allowCyclic = value;
}

QList<Node*> Graph::nodes(QString filter)
{
    if("all"==filter)
    {
        return _nodes.values();
    }
    QList<Node*> output;    
    foreach(Node* node,_nodes.values())
    {
		if(!node)
		{
			continue;
		}

        if(!nodeOwner(node).isEmpty())
        {
            continue;
        }

        if(0==QString::compare(node->getNodeType(),filter)||filter.isEmpty())
        {
            output << node;
        }
    }

    return output;
}

bool Graph::link(Node *in,Node *out)
{
    if(0==in||0==out||in==out)
    {
        return false;
    }


    //Проверка цикличности графа
    if(checkCyclic(in,out))
    {
        return false;
    }
    if(_connections.contains(Graph::Connection(in,out)))
    {
        return true;
    }
   // qDebug() << "Connected:" << in->objectName() << out->objectName();

    _connections.push_back(Graph::Connection(in,out));


    if(this==in||this==out)
    {
        if(this==in)
        {
            this->connect(in,SIGNAL(sendInside(esp)),out,SLOT(receive(esp)),Qt::UniqueConnection);
          //  add(out);
        }
        if(this==out)
        {
            this->connect(in,SIGNAL(send(esp)),out,SIGNAL(send(esp)),Qt::UniqueConnection);
         //   add(in);
        }        
    }else{
        /*
         * send and recieve are connected using queued connection to make sure, that processing is finished before sending results
         */

        connect(in,SIGNAL(send(esp)),out,SLOT(receive(esp)),Qt::UniqueConnection);//,Qt::QueuedConnection);

     //   add(in);
     //   add(out);
    }
    in->onConnected();
    return true;
}
//31 + 15 + 12,5
bool Graph::checkCyclic(Node* in,Node* out)
{
    if(_allowCyclic)
    {
        //Все соединения разрешены
        return false;
    }
    if(in==out)
    {
        return true;
    }
    foreach(Graph::Connection pair,_connections)
    {
        if(pair.first==out)
        {
            if(checkCyclic(in,pair.second))
            {
                return true;
            }
        }
    }
    return false;
}

void Graph::userCommand(QVariant key, QVariant value)
{
    emit userCommandSignal(key,value);
}
void Graph::receiveProcess(esp data)
{
    emit sendInside(data);
}
