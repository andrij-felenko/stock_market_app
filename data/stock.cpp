#include "stock.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "market.h"

using namespace data;

data::Stock::Stock(QObject* parent) : QObject(parent), _ticker(nullptr)
{
    //
}

Ticker* Stock::ticker() const { return _ticker; }
float   Stock::count()  const { return _count;  }
float   Stock::price()  const { return _price;  }
float   Stock::value()  const { return _value;  }

namespace data {
    QDataStream& operator << (QDataStream& s, const Stock& d) {
        s << d._count << d._price << d._value << d._ticker->identity()->ticker()
          << int(d._list.size());
        for (const auto& it : d._list)
            s << *it;
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Stock& d) {
        s >> d._count >> d._price >> d._value;

        QString tag;
        s >> tag;
        auto t = Market::find(tag);
        if (t.has_value())
            d._ticker = t.value();

        int size = 0;
        s >> size;
        for (int i = 0; i < size; i++){
            Transaction* t = new Transaction(&d);
            s >> *t;
            d._list.emplace_back(t);
        }

        return s;
    }
}
