#ifndef DATA_STOCK_H
#define DATA_STOCK_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "sdk_def.h"
#include "../instrument/ticker.h"
#include "transaction.h"

class sdk::Stock : public QObject
{
    Q_OBJECT
    // Q_PROPERTY(data::Ticker* ticker READ ticker CONSTANT)
    // Q_PROPERTY(float count READ count CONSTANT)
    // Q_PROPERTY(float price READ price CONSTANT)
    // Q_PROPERTY(float value READ value CONSTANT)
public:
    Stock(QObject* parent = nullptr);

    Ticker* ticker() const;
    float   count()  const;
    float   price()  const;
    float   value()  const;

    Instrument* instrument() const;

signals:
    void signal_update();

private:
    Ticker* _ticker;
    float   _count;
    float   _price;
    float   _value;

    std::vector <Transaction*> _list;

    friend QDataStream& operator << (QDataStream& s, const Stock& d);
    friend QDataStream& operator >> (QDataStream& s,       Stock& d);
};

#endif // DATA_STOCK_H
