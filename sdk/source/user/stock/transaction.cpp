#include "user/stock/transaction.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

sdk::Transaction::Transaction() : Trackable() { /* */ }
sdk::Transaction::Transaction(float price, float count, const QDateTime& dt,
                              const QString& broker) : Trackable() { /* */ }

bool  sdk::Transaction::isBuy() const { return _count > 0; }
float sdk::Transaction::price() const { return _price; }
float sdk::Transaction::count() const { return _count; }
QString   sdk::Transaction::broker() const { return _broker; }
QDateTime sdk::Transaction::dtime()  const { return _dtime;  }

float sdk::Transaction::value() const { return _price * _count; }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Transaction& d) {
        return s << d._broker << d._count << d._price << d._dtime;
    }

    QDataStream& operator >> (QDataStream& s, Transaction& d) {
        return s >> d._broker >> d._count >> d._price >> d._dtime;
    }
}
