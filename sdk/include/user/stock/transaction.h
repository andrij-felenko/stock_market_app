#ifndef SDK_STOCK_TRANSACTION_H
#define SDK_STOCK_TRANSACTION_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "sdk_def.h"

class sdk::Transaction : Trackable
{ 
    // 0000 00xx -> pending - 00 / exec  - 01 / canceled - 10 / closed - 11
    // 0000 xx00 -> planned - 00 / order - 01 / alert    - 10 / ???
    // 000x 0000 -> buy    - 0 / sell  - 1
    // 00x0 0000 -> market - 0 / limit - 1
    // 0x00 0000 -> no stop - 0 / stop - 1
    // x000 0000 -> reserved
    uint8_t _;
public:
    Transaction(float price, float count, const QDateTime& dt, const QString& broker);

    bool isBuy() const;
    float price() const;
    float count() const;
    float value() const;
    QDateTime dtime() const;
    QString broker() const;

    virtual void fieldChanged(FieldType type) {}

private:
    Transaction();
    float _price;
    float _count;
    QString   _broker;
    QDateTime _dtime;

    friend class Stock;
    FieldTOpt setPrice(float price) { return set_if(this, _price, price, Transaction_price); }
    FieldTOpt setCount(float count) { return set_if(this, _count, count, Transaction_count); }
    FieldTOpt setDTime(const QDateTime& dt) { return set_if(this, _dtime, dt, Transaction_dtime); }
    FieldTOpt setBroker(const QString& broker)
    { return set_if(this, _broker, broker, Transaction_broker); }

    friend QDataStream& operator << (QDataStream& s, const Transaction& d);
    friend QDataStream& operator >> (QDataStream& s,       Transaction& d);

    template <typename T, typename... Args> requires StreamReadableFor <T>
    friend QDataStream& sdk::list_from_stream(QDataStream& s, std::vector <T>& d, Args&&...args);
};

#endif // SDK_STOCK_TRANSACTION_H
