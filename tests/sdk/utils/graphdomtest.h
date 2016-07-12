#ifndef GRAPHDOMTEST_H
#define GRAPHDOMTEST_H

#include <QObject>
#include "../../AutoTest.h"

class GraphDomTest : public QObject
{
    Q_OBJECT
private slots:
    void writeTest();
    void readTest();
    
};
DECLARE_TEST(GraphDomTest)
#endif // GRAPHDOMTEST_H
