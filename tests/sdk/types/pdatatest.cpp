#include "pdatatest.h"


void PDataTest::InitializationTest()
{
    header << "A" << "B" << "C";
}

void PDataTest::AddItems()
{

    try{
        for(int i=0;i<10;i+=2)
        {
            types::Data x(header,i);
            x("A")=3*i;
            x("B")=2;
            data.push_back(x);
        }
    }catch(std::exception x){
        qDebug() << x.what();
        QVERIFY(false);
    }
}
void PDataTest::RetriveItems()
{
    for(int i=0;i<10;i++)
    {
        try{
            types::Data val = data[i];
            if(0!=i%2)
            {
                QVERIFY(val.isempty());
            }
           // if(0==i%2)
            {
               // qDebug() << i << val("A");
                QVERIFY(val("A")==i*3);
            }
        }catch(types::unexpected_header uh){
            qDebug() << uh.what();

        }catch(types::unknown_index ui){
            qDebug() << ui.what();

        }catch(types::data_inconsistent di){
            qDebug() << di.what();
        }catch(...){
            QVERIFY(false);
        }
    }
}
void PDataTest::CopyItems()
{
    types::pData clone = data;
    for(int i=0;i<10;i++)
    {
        types::Data val = clone[i];
        if(0!=i%2)
        {
            QVERIFY(val.isempty());
        }else{
            QVERIFY(val("A")==i*3);
        }

    }
}
void PDataTest::SerialTest()
{
    try{
        QString datax;
        QTextStream str(&datax);

        str << data;

        qDebug() <<"\n"<< datax;

        types::pData item;
        str >> item;

        QVERIFY(data.header()==item.header());
        QVERIFY(data[6]("A")==item[6]("A"));
        QVERIFY(is_nan<float>(item[0]("C")));
       // QVERIFY(data.size()==item.size());
    }catch(std::runtime_error err){
        QVERIFY2(false,err.what());
    }

}

void PDataTest::MergeSerialTest()
{
    try{
        types::pData superdata;
        for(int i=0;i<5;i++)
        {
            types::Data a(i);
            types::Data b(i);
            if(i%2==0){
                a.insert("A",i);
                a.insert("B",i*2);
            }else{
                b.insert("X",i*3);
                b.insert("Y",i*4);
            }
            superdata.push_back(a);
            superdata.push_back(b);

        }
        QString datax;
        QTextStream str(&datax);

        str << superdata;
        QVERIFY(4==superdata.header().size());

        qDebug() <<"\n"<< datax;

    }catch(std::runtime_error err){
        QVERIFY2(false,err.what());
    }

    try{
        types::pData superdata;
        for(int i=0;i<5;i++)
        {
            types::Data a(i);
            types::Data b(i);
            if(i%2!=0){
                a.insert("A",i);
                a.insert("B",i*2);
            }else{
                b.insert("X",i*3);
                b.insert("Y",i*4);
            }
            superdata.push_back(a);
            superdata.push_back(b);

        }
        QString datax;
        QTextStream str(&datax);

        str << superdata;
        QVERIFY(4==superdata.header().size());

        qDebug() <<"\n"<< datax;

    }catch(std::runtime_error err){
        QVERIFY2(false,err.what());
    }
}

void PDataTest::ErrorHandling()
{
    QStringList xheader;
    xheader << "A" << "B";

    types::Data xx(xheader,0);
    xx("A")=1;
    xx("B")=3;

    try{
        data.push_back(xx);

    }catch(types::unexpected_header ee){
        qDebug() << ee.what();
    }catch(types::data_ambiguous da){
        qDebug() << da.what();
    }


    try{
        types::Data pp(0);
        pp.insert("X",1);
        pp.insert("Y",2);
        qDebug() << data.header();
        data.push_back(pp);
        qDebug() << data.header();
        QStringList truelist;
        truelist << "A" << "B" << "C" << "X" << "Y";

        QVERIFY(data.header()==truelist);
    }catch(types::unexpected_header ee)
    {
        qDebug() << ee.what();
    }

  //  qDebug() << data.header();
    try{
        types::Data pp(1);

        pp.insert("X",1);
        pp.insert("Y",2);
        data.push_back(pp);
      //  QStringList truelist;
      //  truelist << "A" << "B" << "C" << "X" << "Y";
      //  QVERIFY(data.header()==truelist);
    }catch(types::data_asynchronous ee)
    {
        qDebug() << ee.what();
    }

    data.clear();
    data.push_back(xx);


}
