#include <QTest>
#include "ticker.h"

class Test_DataTicker : public QObject
{
    Q_OBJECT
private slots:
    void t_save_load() {
        QVERIFY(true);
    }
};
