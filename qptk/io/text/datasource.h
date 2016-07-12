#ifndef PROCESSINGFRAMEWORK_DATASOURCE_H
#define PROCESSINGFRAMEWORK_DATASOURCE_H

#include <io/sourcenode.h>
#include <types/data.h>

namespace io{
namespace text{

class PROCESSINGFRAMEWORK_SHARED_EXPORT DataSource: public SourceNode
{
    Q_OBJECT
//    Q_PROPERTY( QString separator READ separator WRITE setSeparator )
public:
    explicit DataSource(QObject *parent);
    virtual esp get(const unsigned long frame);

    virtual void onConnected();
public slots:
    virtual void setUrl(QString value);
protected:
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
    types::pData _data;
};

}
}
#endif // DATASOURCE_H
