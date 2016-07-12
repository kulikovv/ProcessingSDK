#ifndef TEXTIOTEST_H
#define TEXTIOTEST_H

#include "../../../AutoTest.h"

#include <io/text/dataionode.h>
#include <io/text/multiobjectdata.h>

class TextIOTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void columnsTest();
    void interpolationTest();
    void sharedTest();
    void ioTest();    
private:
    io::text::DataIONode* _node;
    io::text::MultiObjectData* _node1;
};

DECLARE_TEST(TextIOTest)

#endif // TEXTIOTEST_H
