#ifndef ESPOBJECTTEST_H
#define ESPOBJECTTEST_H

#include "../../AutoTest.h"
#include <base/esp.h>
class EspObjectTest : public QObject
{
    Q_OBJECT
signals:
    void send(base::Esp data);
private slots:

    void InitializationTest();
    void SendingTest();
    void CopyTest();
    void CopyConstructorTest();
    void TagSystemTest();

    void recive(base::Esp data);
private:
    base::Esp DumbFunction(base::Esp data);
    base::Esp DumbFunctionCreate(QString data);
};

DECLARE_TEST(EspObjectTest)

#endif // ESPOBJECTTEST_H
