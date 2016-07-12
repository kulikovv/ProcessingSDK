#include "datagenerator.h"
#include <types/data.h>
#include <math.h>

#include <QDebug>

DataGenerator::DataGenerator(QObject *parent) :
    SourceNode(parent)
{
    setFps(30);
    countObjects = 3;
    countArguments = 3;    
    typeData = true;

    //qDebug() << "sending dataset";
    //SEND(types::DataSettings, new types::DataSettings(countObjects, countArguments))
}

/*void DataGenerator::onConnected()
{
    qDebug() << "sending dataset";
    SEND(types::DataSettings, new types::DataSettings(countObjects, countArguments))
}*/

esp DataGenerator::get(const base::timestamp frame) {
    if (typeData) {
        Data *data = new Data();
        for (int i = 0; i < countObjects; i++) {
            Data tdata = genData(frame, i);
            for (int j = 0; j < tdata.size(); j++) {
                data->push_back(tdata[j]);
            }
        }
        SEND(Data, data)
    } else {
        pData *pdata = new pData();
        for (int i = 0; i < countObjects; i++) {
            pdata->push_back(genData(frame, i));
        }
        SEND(pData, pdata)
    }
    return esp();
}

Data DataGenerator::genData(base::timestamp frame, int objectId) {
    Data data = Data();
    for (int i = 0; i < countArguments; i++) {
        if (i % 2 == 0) {
            data.push_back(cos((double)frame / 20) + 2 + objectId + i * 0.1);
        } else {
            data.push_back(sin((double)frame / 20) + 2 + objectId + i * 0.1);
        }
    }
    return data;
}
