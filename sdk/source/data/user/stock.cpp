#include "data/user/stock.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "loader.h"
#include "data/market.h"
#include "data/instrument.h"
#include "utilities/features.h"

using namespace data;

data::Stock::Stock(QObject* parent) : QObject(parent), _ticker(nullptr)
{
    //
}

Ticker* Stock::ticker() const { return _ticker; }
float   Stock::count()  const { return _count;  }
float   Stock::price()  const { return _price;  }
float   Stock::value()  const { return _value;  }

Instrument* Stock::instrument() const { return _ticker->instrument(); }

namespace data {
    QDataStream& operator << (QDataStream& s, const Stock& d) {
        util::list_to_stream(s, d._list);
        return s << d._count << d._price << d._value
                 << d.instrument()->primary_ticker(true)->symbol();
    }

    QDataStream& operator >> (QDataStream& s, Stock& d) {
        util::list_from_stream(s, d._list);
        s >> d._count >> d._price >> d._value;

        QString tag;
        s >> tag;
        auto t = Nexus.market()->find(tag);
        if (t.has_value())
            d._ticker = t.value();

        return s;
    }
}
