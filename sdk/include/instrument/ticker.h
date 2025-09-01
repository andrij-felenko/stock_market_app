#ifndef DATA_TICKER_H
#define DATA_TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "../instrument/quotes.h"
#include "geo/geo.h"
#include "symbol.h"
#include "dividend.h"
#include "quotes.h"
#include "short_interest.h"
#include "corporate_action.h"
#include "valuation.h"
#include "symbol.h"
#include "utilities/features.h"

class sdk::Ticker
{
public:
    Quotes* quotes() const;
    sdk::Currency currency() const;
    QString currency_str() const;
    QString exchange() const;
    QString country() const;
    sdk::Symbol symbol() const;
    QString symbol_str() const;
    bool is_primary() const;

    void set_symbol(sdk::Symbol symbol);

    void save();

public slots:
    void setSymbol(QString new_symbol);

    void set_symbol(QString new_symbol);
    void set_symbol(QString code, QString exch);
    void set_symbol(QString code, sdk::Exchange e);

private:
    Ticker();

    Dividend _dividend;
    Quotes _quotes;
    ShortInterest _short_interst;
    CorporateAction _corp_action;
    Valuation _valuation;
    sdk::Symbol _symbol;

    friend class Instrument;

    template <typename T, typename... Args>
    requires (std::is_pointer_v <T> ? sdk::DataStreamReadable <std::remove_pointer_t <T>>
                                    : sdk::DataStreamReadable <T>)
    friend QDataStream& sdk::list_from_stream(QDataStream& s, std::vector <T>& d, Args&&...args);

    Ticker(const Ticker&) = delete;        // still disallow copy-construct
    Ticker& operator = (Ticker&&) = delete;  // and moving, if you prefer
    Ticker& operator = (const Ticker& other);

    friend QDataStream& operator << (QDataStream& s, const Ticker& d);
    friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
};

#endif // DATA_TICKER_H
