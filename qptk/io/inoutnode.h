#ifndef INOUTNODE_H
#define INOUTNODE_H

#include <io/sourcenode.h>
namespace io{
class PROCESSINGFRAMEWORK_SHARED_EXPORT InoutNode : public SourceNode{
    Q_OBJECT
    Q_PROPERTY( bool recording READ recording WRITE setRecording)
public:
    explicit InoutNode(QObject *parent);
    bool recording() const;
public slots:
    /*!
     * \brief setUrl
     * \param value
     */
    void setUrl(QString value);
    /*! Включить/выключить запись
      */
    virtual void setRecording(bool value);
    /*!
     * \brief reset обнуляется _url по умолчанию
     */
    virtual void reset();
protected:
    virtual void receiveProcess(esp data);
    virtual bool save(QString path)=0;
    virtual bool load(QString path)=0;
    virtual void record(esp data)=0;
    virtual void clear()=0;
    virtual void play(esp data);
private:
    bool _recording;//! Флаг записи
};
}
#endif // INOUTNODE_H


