#include "espobjecttest.h"
#include <base/esp.h>
#include <types/data.h>

void EspObjectTest::InitializationTest()
{
    QVector<float> * pdata = new QVector<float>;
    pdata->push_back(1);
    pdata->push_back(2);
    pdata->push_back(3);
    pdata->push_back(4);
    pdata->push_back(5);

    esp ptr = esp::init(pdata,1,"TEST");

    QVERIFY(ptr=="TEST");
    QSharedPointer<QVector<float> > rdata = ptr.getReadOnly<QVector<float> >();
    QVERIFY(!rdata.isNull());
    QCOMPARE(pdata->size(),rdata->size());
    QCOMPARE(ptr.getFrameId(),unsigned long(1));
    for(int i=0;i<pdata->count();i++)
    {
        QCOMPARE(pdata->at(i),rdata->at(i));
    }
}

void EspObjectTest::CopyConstructorTest()
{
    esp bdata = esp::init(new QString("Hello"),1,"TEST");
    esp rdata = bdata;
    QCOMPARE(bdata.getReadOnly<QString>(), rdata.getReadOnly<QString>());
}

void EspObjectTest::TagSystemTest()
{
    esp bdata = esp::init(new QString("Hello"),1,"TEST");
    QVERIFY(bdata=="TEST");
    QSharedPointer<QString> data = bdata.getReadOnly<QString>();
    QCOMPARE(*(data.data()), QString("Hello"));

    QSharedPointer<int> dataInvError = bdata.getReadOnly<int>();
     QVERIFY(dataInvError.isNull());

}

void EspObjectTest::SendingTest()
{
    connect(this,SIGNAL(send(base::Esp)),this,SLOT(recive(base::Esp)));
    esp bdata = esp::init(new QString("Hello"),1);
    esp edata = DumbFunctionCreate("Hello");
    QCOMPARE(*(bdata.getReadOnly<QString>()),*(edata.getReadOnly<QString>()));
}
void EspObjectTest::recive(base::Esp data)
{
    QCOMPARE(*(data.getReadOnly<QString>()), QString("Hello"));
    qDebug("Recived");
}

void  EspObjectTest::CopyTest()
{
    esp bdata = esp::init(new QString("Hello"),1);
    esp edata = DumbFunction(bdata);

    QCOMPARE(edata.getReadOnly<QString>(),bdata.getReadOnly<QString>());

    esp rdata = DumbFunctionCreate("Hello");
    QCOMPARE(*(rdata.getReadOnly<QString>()),QString("Hello"));
}

base::Esp EspObjectTest::DumbFunction(base::Esp data)
{
    QSharedPointer<QString> adata = data.getReadOnly<QString>();
  //  QVERIFY(!adata.isNull());
  //  QCOMPARE(*adata.data(), QString("Hello"));

    return data;
}

base::Esp EspObjectTest::DumbFunctionCreate(QString data)
{
    esp bdata = esp::init(new QString(data),1);
    emit send(bdata);
    return bdata;
}
