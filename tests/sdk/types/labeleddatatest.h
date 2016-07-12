#ifndef LABELEDDATATEST_H
#define LABELEDDATATEST_H

#include "../../AutoTest.h"
#include <QObject>

class LabeledDataTest : public QObject
{
    Q_OBJECT
private slots:

    void InitializationTest();
    void AtTest();
    void ErrorTests();
    void BraketsTests();
    void MergeTest();
    void NanHeader();
};

DECLARE_TEST(LabeledDataTest)

#endif // LABELEDDATATEST_H
