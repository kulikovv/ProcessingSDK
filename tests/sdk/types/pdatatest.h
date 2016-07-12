#ifndef PDATATEST_H
#define PDATATEST_H

#include "../../AutoTest.h"
#include <QObject>
#include <types/data.h>
class PDataTest : public QObject
{
    Q_OBJECT
private slots:
    void InitializationTest();
    void AddItems();
    void RetriveItems();
    void CopyItems();
    void SerialTest();
    void MergeSerialTest();
    void ErrorHandling();
private:
    types::pData data;
    QStringList header;
};
DECLARE_TEST(PDataTest)

#endif // PDATATEST_H
