#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H

#include <io/outputnode.h>
#include <types/data.h>

namespace io{
/*! \brief Классы работы с текстом
  Содержит узлы для загрузки и сохранения текстовых данных
  \sa types::Data
  */
namespace text{

class PROCESSINGFRAMEWORK_SHARED_EXPORT DataOutput : public OutputNode
{
    Q_OBJECT
   // Q_PROPERTY( QString separator READ separator WRITE setSeparator )
//    Q_//PROPERTY( QString columns READ columns WRITE setColumns )
public:
    explicit DataOutput(QObject *parent = 0);
    ~DataOutput();
  //  QString separator() const;
 //   QString columns() const;
signals:

public slots:
   // void setSeparator(QString value);
  //  void setColumns(QString value);
    virtual void setUrl(QString value);
    virtual void stop();
protected:
    virtual void receiveProcess(esp data);
private:
    types::pData _data;
//    QString _separator;
 //   QString _columns;
};
}
}
#endif // DATAOUTPUT_H
