#ifndef DATA_TICKER_H
#define DATA_TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "../instrument/quotes.h"
#include "../currency/name.h"

namespace data {
    class Ticker;
    class Instrument;
}

class data::Ticker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Quotes* quotes  READ quotes CONSTANT)
    Q_PROPERTY(bool    primary READ is_primary NOTIFY primaryChanged)
    Q_PROPERTY(currency::Tag currency READ currency WRITE set_currency NOTIFY currencyChanged)
    Q_PROPERTY(QString   currency_str READ currency_str                NOTIFY currencyChanged)
    Q_PROPERTY(QString exchange   READ exchange   WRITE set_exchange   NOTIFY exchangeChanged)
    Q_PROPERTY(QString country    READ country    WRITE set_country    NOTIFY  countryChanged)
    Q_PROPERTY(QString symbol     READ symbol     WRITE set_symbol     NOTIFY   symbolChanged)
public:
    Ticker(bool primary = false, Instrument* parent = nullptr);

    Quotes* quotes() const;
    currency::Tag currency() const;
    QString currency_str() const;
    QString exchange() const;
    QString country() const;
    QString symbol() const;
    bool is_primary() const;

    data::Instrument* instrument() const;

    void save();

public slots:
    void set_currency(const currency::Tag& new_currency);
    void set_exchange(const QString& new_exchange);
    void set_country(const QString& new_country);
    void set_symbol(QString new_symbol);

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
    QString _symbol;
    bool _primary;

    friend class Instrument;

    friend QDataStream& operator << (QDataStream& s, const Ticker& d);
    friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
};

#endif // DATA_TICKER_H
