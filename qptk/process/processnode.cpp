#include "processnode.h"
using namespace base;
using namespace process;
ProcessNode::ProcessNode(QObject *parent):Node(parent)
{
    setNodeType("Process");
    setEnable(false);
    setBehaviour(Node::PassingNext);
}

ProcessNode::~ProcessNode()
{

}

