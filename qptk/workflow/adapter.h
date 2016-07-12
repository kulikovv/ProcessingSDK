#ifndef ADAPTER_H
#define ADAPTER_H
#include <workflow/workflownode.h>
namespace workflow
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT Adapter : public WorkflowNode
{
    Q_OBJECT

    Q_PROPERTY(int selected READ selected WRITE setSelected)
public:
    explicit Adapter(QObject *parent = 0);
    int selected() const;
signals:

public slots:
	virtual void addNode(base::Node* node);
    void setSelected(int x);
    virtual void userCommand(QVariant key,QVariant value);
protected:
    virtual void receiveProcess(esp data);
    int _selected;
    esp _mem;
};
}

#endif // ADAPTER_H
