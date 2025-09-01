#ifndef SDK_INSTRUMENT_TICKER_H
#define SDK_INSTRUMENT_TICKER_H

#include "sdk.h" // IWYU pragma: keep
#include "symbol.h"

class sdk::Ticker : Trackable
{
public:
    Ticker();

    const Symbol symbol() const { return _symbol; }
    FieldTOpt setListingCode(const QByteArray& code, sdk::Exchange exch)
    { return sdk::set_if(this, _symbol, Symbol(code, exch), sdk::Ticker_code); }

private:
    Symbol _symbol;
};

#endif // SDK_INSTRUMENT_TICKER_H
