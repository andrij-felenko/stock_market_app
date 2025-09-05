#include "data/user/transaction.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

using namespace data;

data::Transaction::Transaction(QObject* parent) : QObject(parent)
{
    //
}

bool  Transaction::buy()   const { return _buy;   }
float Transaction::price() const { return _price; }
float Transaction::count() const { return _count; }
QString   Transaction::broker() const { return _broker; }
QDateTime Transaction::dtime()  const { return _dtime;  }

namespace data {
    QDataStream& operator << (QDataStream& s, const Transaction& d) {
        return s << d._broker << d._buy << d._count << d._price << d._dtime;
    }

    QDataStream& operator >> (QDataStream& s, Transaction& d) {
        return s >> d._broker >> d._buy >> d._count >> d._price >> d._dtime;
    }
}
