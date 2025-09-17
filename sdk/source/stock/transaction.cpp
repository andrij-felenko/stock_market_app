#include "stock/transaction.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

sdk::Transaction::Transaction(QObject* parent) : QObject(parent)
{
    //
}

bool  sdk::Transaction::buy()   const { return _buy;   }
float sdk::Transaction::price() const { return _price; }
float sdk::Transaction::count() const { return _count; }
QString   sdk::Transaction::broker() const { return _broker; }
QDateTime sdk::Transaction::dtime()  const { return _dtime;  }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Transaction& d) {
        return s << d._broker << d._buy << d._count << d._price << d._dtime;
    }

    QDataStream& operator >> (QDataStream& s, Transaction& d) {
        return s >> d._broker >> d._buy >> d._count >> d._price >> d._dtime;
    }
}
