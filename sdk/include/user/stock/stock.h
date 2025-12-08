#ifndef SDK_STOCK_STOCK_H
#define SDK_STOCK_STOCK_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "sdk_def.h"
#include "transaction.h"
#include "core/symbol.h"

class sdk::Stock : Trackable
{
public:
    Stock(const sdk::Symbol& symbol);

    const sdk::Symbol& symbol() const;
    float count() const;
    float price() const;
    float value() const;

    void buy (float price, float count, const QDateTime& dt, const QString& broker);
    void sell(float price, float count, const QDateTime& dt, const QString& broker);

    void updateCount (Transaction& t, float count);
    void updatePrice (Transaction& t, float count);
    void updateDTime (Transaction& t, const QDateTime& dt);
    void updateBroker(Transaction& t, const QString& broker);

    virtual void fieldChanged(FieldType type) {}

private:
    sdk::Symbol _tag;

    std::vector <Transaction> _list;

    friend QDataStream& operator << (QDataStream& s, Wire <const Stock> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Stock> d);

    friend QDataStream& operator << (QDataStream& s, const Stock& d);
    friend QDataStream& operator >> (QDataStream& s,       Stock& d);
};

#endif // SDK_STOCK_STOCK_H
