#ifndef WORKFLOWNODE_H
#define WORKFLOWNODE_H
#include <base/node.h>
#include <QList>
namespace workflow
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT WorkflowNode : public base::Node
{
    Q_OBJECT
    Q_PROPERTY(QString nodesNames READ nodesNames WRITE setNodesNames)
public:
    explicit WorkflowNode(QObject *parent = 0);
    QString nodesNames() const;
signals:

public slots:
    virtual void setNodesNames(QString value);
    virtual void addNode(base::Node* node);
    virtual QList<base::Node*> getNodes() const;
private slots:
    virtual void removeNode(QObject* node);
protected:
    QList<base::Node*> _nodes;
};
}

#endif // WORKFLOWNODE_H
