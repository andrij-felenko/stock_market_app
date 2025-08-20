#ifndef DATA_TICKER_H
#define DATA_TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "../instrument/quotes.h"
#include "../geo/geo.h"
#include "utilities/features.h"
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
    Q_PROPERTY(geo::Currency currency READ currency               NOTIFY currencyChanged)
    Q_PROPERTY(QString   currency_str READ currency_str           NOTIFY currencyChanged)
    Q_PROPERTY(QString exchange   READ exchange                   NOTIFY exchangeChanged)
    Q_PROPERTY(QString country    READ country                    NOTIFY  countryChanged)
    Q_PROPERTY(QString symbol     READ symbol_str WRITE setSymbol NOTIFY   symbolChanged)
public:
    Quotes* quotes() const;
    geo::Currency currency() const;
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
    void setSymbol(QString new_symbol);

    void set_symbol(QString new_symbol);
    void set_symbol(QString code, QString exch);
    void set_symbol(QString code, geo::Exchange e);

signals:
    void currencyChanged(geo::Currency currency);
    void exchangeChanged(QString exchange);
    void countryChanged(QString country);
    void symbolChanged(QString symbol);
    void primaryChanged(bool is_primary);

    void signal_update();
    void signal_save();

private:
    Ticker(Instrument* parent = nullptr);

    Quotes* _quotes = nullptr;
    ticker::Symbol _symbol;

    friend class Instrument;
    // friend class Market;

    template <typename T, typename... Args>
    requires (std::is_pointer_v <T> ? util::DataStreamReadable <std::remove_pointer_t <T>>
                                    : util::DataStreamReadable <T>)
    friend QDataStream& util::list_from_stream(QDataStream& s, std::vector <T>& d, Args&&...args);

    Ticker(const Ticker&) = delete;        // still disallow copy-construct
    Ticker& operator = (Ticker&&) = delete;  // and moving, if you prefer
    Ticker& operator = (const Ticker& other);

    friend QDataStream& operator << (QDataStream& s, const Ticker& d);
    friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
};

#endif // DATA_TICKER_H
