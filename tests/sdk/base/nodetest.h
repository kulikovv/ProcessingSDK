#ifndef NODETEST_H
#define NODETEST_H

#include "../../AutoTest.h"

class NodeTest : public QObject
{
    Q_OBJECT
public:
    explicit NodeTest(QObject *parent = 0);
    
public slots:
    
};

DECLARE_TEST(NodeTest)

#endif // NODETEST_H
