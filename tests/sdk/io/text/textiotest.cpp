#include "textiotest.h"
#include <types/data.h>
#include <iostream>

void TextIOTest::initTestCase()
{
    _node = new io::text::DataIONode(0);
}

void TextIOTest::cleanupTestCase()
{
    delete _node;
}

void TextIOTest::columnsTest()
{
    try{
    QStringList header;
    header << "X" << "Y" << "Z";
    types::Data gdata(header);
    gdata("X")=1;
    gdata("Y")=2;
    gdata("Z")=3;

    _node->setUrl("D:/test.txt");
    _node->setRecording(true);

    for(int i=0;i<10;i++){
        types::Data gdata(header,10-i);
        gdata("X")=i;
        esp ep = esp::init(new types::Data(gdata),i);
        _node->receive(ep);
    }
    _node->setRecording(false);
    }catch(unexpected_handler err){
        qDebug() << err;
    }catch(std::exception exp){
        qDebug() << exp.what();
    }catch(...){
        qDebug() << "unknow exception";
    }

   /* _node->setColumns("A;B;C");
    QCOMPARE(_node->columns(),QString("A;B;C"));
    _node->setColumns("A;B;C;D");
    QCOMPARE(_node->columns(),QString("A;B;C;D"));
    _node->reset();
    QCOMPARE(_node->columns(),QString(""));*/


}
void TextIOTest::sharedTest()
{
    io::text::DataIONode* xn = new io::text::DataIONode(0);
   // xn->setRecording(false);
    xn->setUrl("D:/test.txt");
    xn->setRecording(false);
    types::pData readed = xn->getData();

    QStringList header;
    header << "X" << "Y" << "Z";
    QVERIFY(readed.header()==header);
    QVERIFY(readed.size()==11);
    try{
    for(int i=1;i<10;i++){
        QVERIFY(readed[i]("X")==10-i);
    }
    }catch(std::runtime_error err){
        qDebug() << err.what();
    }

}

void TextIOTest::interpolationTest()
{

    _node->disconnect();
    _node->setUrl("D:/test.txt");
    _node->setRecording(true);
    types::Data *gdata = new types::Data();

   /* gdata->at(1);
    gdata->push_back(2);
    gdata->push_back(3);
    gdata->push_back(4);
    gdata->push_back(5);*/

    esp ep = esp::init(gdata,5);
    //Save data
    _node->receive(ep);

   // esp pointer =  _node->get(1);
   // qDebug()<<"Start";
    QSharedPointer<types::Data> res = _node->get(1).getReadOnly<types::Data>();
  //  qDebug() <<"End";
  /*  QStringList list;
    esp pointer =   _node->get(1);
    QSharedPointer<types::Data> res = pointer.getReadOnly<types::Data>();
    for(int i=0;i<pointer.getReadOnly<types::Data>()->size();i++)
    {
        list.push_back(QVariant((*pointer.getReadOnly<types::Data>())[i]).toString());
    }

    qDebug() << list;*/

  /*  QSharedPointer<types::Data> pres = ep.getReadOnly<types::Data>();
    QVERIFY(!res.isNull());
    QVERIFY(!pres.isNull());
    QCOMPARE(res->size(),pres->size());

    for(int i=0;i<res->size();i++)
    {
        QCOMPARE(res->at(i),gdata->at(i));
    }*/
    //_node->setRecording(false);

    // _node->setUrl("D:/test.txt");

}

void TextIOTest::ioTest() {


  /*  types::Data data = types::Data();
    data.push_back(0.1f);
    data.push_back(0.2f);
    data.push_back(0.3f);
    data.push_back(0.4f);
    data.push_back(0.5f);
    data.push_back(0.6f);
    data.push_back(0.7f);
    data.push_back(0.8f);
    data.push_back(0.9f);

    types::Data *data3 = new types::Data();
    data3->push_back(0.1f);
    data3->push_back(0.2f);
    data3->push_back(0.3f);
    data3->push_back(0.1f);
    data3->push_back(0.2f);
    data3->push_back(0.3f);
    data3->push_back(0.1f);
    data3->push_back(0.2f);
    data3->push_back(0.3f);

    types::Data *data1 = new types::Data();
    data1->push_back(0.4f);
    data1->push_back(0.5f);
    data1->push_back(0.6f);
    data1->push_back(0.4f);
    data1->push_back(0.5f);
    data1->push_back(0.6f);
    data1->push_back(0.4f);
    data1->push_back(0.5f);
    data1->push_back(0.6f);

    types::Data *data2 = new types::Data();
    data2->push_back(0.7f);
    data2->push_back(0.8f);
    data2->push_back(0.9f);
    data2->push_back(0.7f);
    data2->push_back(0.8f);
    data2->push_back(0.9f);
    data2->push_back(0.7f);
    data2->push_back(0.8f);
    data2->push_back(0.9f);

    //

    types::Data gdata1 = types::Data();

    types::Data gdata2 = types::Data();
    gdata2.push_back(0.1f);
    gdata2.push_back(0.2f);
    gdata2.push_back(0.3f);

    types::Data gdata3 = types::Data();
    gdata3.push_back(0.8f);
    gdata3.push_back(0.8f);
    gdata3.push_back(0.8f);

    types::pData *pdata1 = new types::pData();
    pdata1->push_back(gdata1);
    pdata1->push_back(gdata2);
    pdata1->push_back(gdata3);

    types::pData *pdata2 = new types::pData();
    pdata2->push_back(gdata3);
    pdata2->push_back(gdata1);
    pdata2->push_back(gdata2);

    types::pData *pdata3 = new types::pData();
    pdata3->push_back(gdata3);
    pdata3->push_back(gdata1);
    pdata3->push_back(gdata2);*/

    //

    /*_node1->disconnect();
    _node1->setUrl("C:/Coding/test1.txt"); // load
    _node1->setRecording(true);
    _node1->setEnable(true);
    esp ioesp1 = esp::init(data1, 1);
    esp ioesp2 = esp::init(data2, 2);
    esp ioesp3 = esp::init(data3, 5);
    esp ioesp4 = esp::init(pdata1, 7);
    esp ioesp5 = esp::init(pdata2, 8);
    esp ioesp6 = esp::init(pdata3, 9);
    _node1->receive(ioesp1);
    _node1->receive(ioesp2);
    _node1->receive(ioesp3);
    _node1->receive(ioesp4);
    _node1->receive(ioesp5);
    _node1->receive(ioesp6);
    _node1->setRecording(false); // save
    */
}
