#ifndef STRINGTEST_H
#define STRINGTEST_H


#include "AutoTest.h"

class TestQString: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};

void TestQString::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}


DECLARE_TEST(TestQString)

#endif // STRINGTEST_H
