#include "stock/stock.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "loader.h"
#include "services/market.h"
#include "instrument/instrument.h"
#include "utilities/features.h"

using namespace sdk;

Stock::Stock(QObject* parent) : QObject(parent), _ticker(nullptr)
{
    //
}

Ticker* Stock::ticker() const { return _ticker; }
float   Stock::count()  const { return _count;  }
float   Stock::price()  const { return _price;  }
float   Stock::value()  const { return _value;  }

Instrument* Stock::instrument() const { return _ticker->instrument(); }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Stock& d) {
        sdk::list_to_stream(s, d._list);
        return s << d._count << d._price << d._value
                 << d.instrument()->isin();
    }

    QDataStream& operator >> (QDataStream& s, Stock& d) {
        sdk::list_from_stream(s, d._list);
        s >> d._count >> d._price >> d._value;

        Isin isin;
        s >> isin;
        auto t = Nexus.market()->findInstrument(isin);

        return s;
    }
}
