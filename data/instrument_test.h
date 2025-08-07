#include <QTest>
#include "instrument.h"
#include "market.h"
#include <QDebug>

class Test_DataInstrument : public QObject
{
    Q_OBJECT
private slots:
    void t_save_load() {
        // qDebug() << 1;
        // data::Instrument* in = new data::Instrument(this);
        // qDebug() << 2;
        // data::Ticker* t = in->get("TEST", true, true);
        // qDebug() << 3;
        // t->quotes()->set_data(QDate::currentDate(), 2,3, 4, 5, 5);
        // qDebug() << 4;
        // in->save();
        // qDebug() << 5;

        // data::Instrument* in2 = new data::Instrument(this);
        // qDebug() << 6;
        // in2->get("TEST", true, true);
        // in2->load();
        // qDebug() << 7;

        // QVERIFY(not in ->primary_ticker()->quotes()->points().empty());
        // QVERIFY(not in2->primary_ticker()->quotes()->points().empty());
    }
};
