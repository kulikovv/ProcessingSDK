#include "labeleddatatest.h"

#include <types/data.h>


void LabeledDataTest::InitializationTest()
{
    QStringList header;
    header << "A" << "B" << "C";
    types::Data d(header);
    QVERIFY(d.header()==header);

    QVERIFY(3==d.size());


}

void LabeledDataTest::AtTest()
{
    try{
        QStringList header;
        header << "A" << "B" << "C";
        types::Data d(header);
        d.at("A")=5;
        d.at("B")=7;
        d.at(2)=8;
        QVERIFY(5==d[0]);
        int x=0;
        QVERIFY(5==d.at(x));
        QVERIFY(5==d.at("A"));
        QVERIFY(8==d.at("C"));
    }catch(int x){
        QVERIFY(-3==x);
    }catch(std::exception ex){
        qDebug() << ex.what();
    }catch(...){
        qDebug() << "Exception recvied";
    }
}

void LabeledDataTest::ErrorTests()
{
    QStringList header;
    header << "A" << "B" << "C";
    types::Data d(header);
    try{
    d.at("D")=5;
    }catch(types::unknown_index){
        qDebug() << "Unknown index";
    }catch(std::exception ex){
        qDebug() << ex.what();
    }catch(...){
        qDebug() << "Exception recvied";
    }

    return;
    try{
        d.at(7)=5;
    }catch(int x){
        qDebug() << "int exception" << x;
        QVERIFY(-3==x);
    }catch(std::exception ex){
        qDebug() << ex.what();
    }catch(...){
        qDebug() << "Exception recvied";
    }
}
void LabeledDataTest::NanHeader()
{
    QStringList header;
    header << "A" << "B" << "C";
    types::Data d(header,10);
    d("B")=5;
    QVERIFY(is_nan<float>(d[0]) );
    QVERIFY(!is_nan<float>(d[1]) );
}

void LabeledDataTest::BraketsTests(){
    QStringList header;
    header << "A" << "B" << "C";
    types::Data d(header,10);


    d("A")=5;
    d("B")=6;

    QVERIFY(5==d("A"));
    QVERIFY(6==d("B"));
    QVERIFY(5==d[0]);
    QVERIFY(6==d[1]);
}

void LabeledDataTest::MergeTest(){
    types::Data a(10);
    types::Data b(10);
    a.insert("A",1);
    a.insert("B",2);
    a.insert("C",3);
    b.insert("X",4);
    b.insert("A",5);
    try{
        types::Data ab = types::Data::merge(a,b);
        qDebug() << ab.header();
        qDebug() << ab("A");
        qDebug() <<  ab("X");
        QVERIFY(10==ab.frameId());
    }catch(...){
        QVERIFY2(false,"Unknown exception in block");
    }
    types::Data c(11);
    c.insert("X",4);
    c.insert("A",5);
    try{
        types::Data ac = types::Data::merge(a,c);
        QVERIFY2(false,"Exception asynchronous not fired");
    }catch(types::data_asynchronous da){
        qDebug() << da.what();
        QVERIFY(true);
    }catch(...){
        QVERIFY2(false,"Unknown exception in block");
    }

}
