#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <io/sourcenode.h>
#include <types/data.h>

using namespace io;
using namespace types;

class DataGenerator : public SourceNode
{
    Q_OBJECT
    Q_PROPERTY(int countArguments READ getCountArguments WRITE setCountArguments)
    Q_PROPERTY(int countObjects READ getCountObjects WRITE setCountObjects)
    Q_PROPERTY(bool typeData READ getTypeData WRITE setTypeData)
public:
    explicit DataGenerator(QObject *parent = 0);

    virtual esp get(const base::timestamp frame);

    int getCountArguments() {
        return countArguments;
    }
    int getCountObjects() {
        return countObjects;
    }
    bool getTypeData() {
        return typeData;
    }

    //virtual void onConnected();

signals:
    
public slots:
    void setCountArguments(int value) {
        if (value > 0) {
            countArguments = value;
        }
    }
    void setCountObjects(int value) {
        if (value > 0) {
            countObjects = value;
        }
    }
    void setTypeData(bool value) {
        typeData = value;
    }

private:
    int countArguments;
    int countObjects;
    bool typeData;

    Data genData(base::timestamp frame, int objectId);
    
};

#endif // DATAGENERATOR_H
