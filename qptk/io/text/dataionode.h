#ifndef DATAIONODE_H
#define DATAIONODE_H

#include <io/inoutnode.h>
#include <types/data.h>
#include <vector>

namespace io
{

namespace text
{

class PROCESSINGFRAMEWORK_SHARED_EXPORT DataIONode : public InoutNode{
    Q_OBJECT
    Q_PROPERTY(bool interpolation READ interpolation WRITE setInterpolation)
public:
    explicit DataIONode(QObject *parent);
    ~DataIONode();

    /*!Call from synchronizer 
      * general rule frame = (0,length())
      * if frame = -1 it means to take next frame
      */
    virtual esp get(const unsigned long frame);
    virtual void onConnected();
    /*!
     * \brief interpolation механизм подключения интерполяции
     * \return если возвращает истину, значит интерполяция включения. По умолчанию она включена
     */
    bool interpolation();
    const types::pData& getData();
public slots:
    virtual void reset();
    void setInterpolation(bool value);
protected:
    virtual void play(esp data);
    virtual void clear();
    virtual bool save(QString path);
    virtual bool load(QString path);
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    types::pData _data;
    QString _separator;
    virtual void record(esp data);
    bool _interpolation;
};
}
}
#endif // DATAIONODE_H


