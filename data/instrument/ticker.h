#ifndef DATA_TICKER_H
#define DATA_TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "../instrument/quotes.h"
#include "../currency/name.h"
#include "symbol.h"

namespace data {
    class Ticker;
    class Instrument;
}

class data::Ticker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Quotes* quotes  READ quotes CONSTANT)
    Q_PROPERTY(bool    primary READ is_primary NOTIFY primaryChanged)
    Q_PROPERTY(currency::Tag currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(QString   currency_str READ currency_str                NOTIFY currencyChanged)
    Q_PROPERTY(QString exchange   READ exchange   WRITE setExchange   NOTIFY exchangeChanged)
    Q_PROPERTY(QString country    READ country    WRITE setCountry    NOTIFY  countryChanged)
    Q_PROPERTY(QString symbol     READ symbol_str WRITE setSymbol     NOTIFY   symbolChanged)
public:
    Ticker(bool primary = false, Instrument* parent = nullptr);

    Quotes* quotes() const;
    currency::Tag currency() const;
    QString currency_str() const;
    QString exchange() const;
    QString country() const;
    ticker::Symbol symbol() const;
    QString symbol_str() const;
    bool is_primary() const;

    data::Instrument* instrument() const;
    void set_symbol(ticker::Symbol symbol);

    void save();

public slots:
    void setCurrency(const currency::Tag& new_currency);
    void setExchange(const QString& new_exchange);
    void setCountry(const QString& new_country);
    void setSymbol(QString new_symbol);

    void set_symbol(QString new_symbol);
    void set_symbol(QString code, QString exch);
    void set_symbol(QString code, ExchangeEnum e);

signals:
    void currencyChanged(currency::Tag currency);
    void exchangeChanged(QString exchange);
    void countryChanged(QString country);
    void symbolChanged(QString symbol);
    void primaryChanged(bool is_primary);

    void update_data();

private:
    Quotes* _quotes = nullptr;
    currency::Tag _currency;
    QString _exchange;
    QString _country;
    ticker::Symbol _symbol;
    bool _primary;

    friend class Instrument;
    friend class Market;

    friend QDataStream& operator << (QDataStream& s, const Ticker& d);
    friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
};

#endif // DATA_TICKER_H
