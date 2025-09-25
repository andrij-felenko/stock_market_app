#ifndef DATA_TICKER_H
#define DATA_TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "core/security/listing/quotes.h"
#include "core/enum.h"
#include "core/symbol.h"
#include "dividend.h"
#include "quotes.h"
#include "short_interest.h"
#include "corporate_action.h"
#include "valuation.h"
#include "common/features.h"

class sdk::Ticker
{
public:
    sdk::Symbol symbol() const;
    Instrument* parent() const;
    Instrument* instrument() const;

    CorporateAction corp_action;
    Dividend        dividend;
    Quotes          quotes;
    ShortInterest   short_interst;
    // Valuation       valuation;

    void save();

private:
    Ticker(uint16_t parent = 0);
    Ticker(const sdk::Symbol& symbol, uint16_t parent = 0);

    sdk::Symbol _symbol;

    uint16_t _parent;
    friend class sdk::Data;
    friend class sdk::Instrument;
    friend class sdk::Market;

    template <typename T, typename... Args> requires StreamReadableFor <T>
    friend QDataStream& sdk::list_from_stream(QDataStream& stream, bool recursive,
                                              std::vector <T>& d, Args&&...args);

    // Ticker(const Ticker&) = delete;        // still disallow copy-construct
    // Ticker& operator = (Ticker&&) = delete;  // and moving, if you prefer
    Ticker& operator = (const Ticker& other);

    friend QDataStream& operator << (QDataStream& s, Wire <const Ticker> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Ticker> d);
};

#endif // DATA_TICKER_H
