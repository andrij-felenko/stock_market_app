#include "stock/stock.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "loader.h"
#include "services/market.h"
#include "instrument/instrument.h"
#include "utilities/features.h"

using namespace sdk;

Stock::Stock(const sdk::Symbol& symbol) : _tag(symbol)
{
    //
}

const Symbol& Stock::symbol() const { return _tag; }

float Stock::price() const { return value() / count(); }
float Stock::count() const
{
    float count = 0;
    for (const auto& it : _list)
        count += it._count;
    return count;
}

float Stock::value() const
{
    float value = 0;
    for (const auto& it : _list)
        value += it.value();
    return value;
}

void Stock::buy(float price, float count, const QDateTime& dt, const QString& broker)
{
    _list.emplace_back(price, count, dt, broker);
    fieldChanged(Stock_new);
}

void Stock::sell(float price, float count, const QDateTime& dt, const QString& broker)
{
    _list.emplace_back(price, -count, dt, broker);
    fieldChanged(Stock_new);
}

void Stock::updateCount(Transaction& t, float count)
{
    for (auto& it : _list)
        if (&t == &it){
            it.setCount(count);
            fieldChanged(Stock_count);
            return;
        }
}

void Stock::updatePrice(Transaction& t, float price)
{
    for (auto& it : _list)
        if (&t == &it){
            it.setCount(price);
            fieldChanged(Stock_price);
            return;
        }
}

void Stock::updateDTime(Transaction& t, const QDateTime& dt)
{
    for (auto& it : _list)
        if (&t == &it){
            it.setDTime(dt);
            fieldChanged(Transaction_dtime);
            return;
        }
}

void Stock::updateBroker(Transaction& t, const QString& broker)
{
    for (auto& it : _list)
        if (&t == &it){
            it.setBroker(broker);
            fieldChanged(Transaction_broker);
            return;
        }
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Stock& d) {
        sdk::list_to_stream(s, d._list);
        return s << d._tag;
    }

    QDataStream& operator >> (QDataStream& s, Stock& d) {
        sdk::list_from_stream(s, d._list);
        s >> d._tag;

        // Symbol tag; s >> tag;
        // d._ticker = Nexus.market()->findTicker(tag);
        return s;
    }
}
