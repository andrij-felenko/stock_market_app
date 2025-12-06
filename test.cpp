#include <QTest>
#include "data/instrument_test.h"
#include "data/instrument/ticker_test.h"

int main(int argc, char** argv)
{
    int status = 0;

    {
        Test_DataInstrument t;
        status |= QTest::qExec(&t, argc, argv);
    }

    {
        Test_DataTicker t;
        status |= QTest::qExec(&t, argc, argv);
    }

    return status;
}
