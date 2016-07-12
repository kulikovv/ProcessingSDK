#ifndef GRAPHTEST_H
#define GRAPHTEST_H

#include <QObject>
#include "../../AutoTest.h"

class GraphTest : public QObject
{
    Q_OBJECT
public:
    explicit GraphTest(QObject *parent = 0);
    
signals:
    
private slots:
    void SubGraphTest();
};
DECLARE_TEST(GraphTest)
#endif // GRAPHTEST_H
