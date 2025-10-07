#ifndef SDK_DATA_H
#define SDK_DATA_H

#include "sdk_def.h"
#include "listing/ticker.h"
#include "legal.h"
#include "meta.h"
#include "finance/finance.h"

class sdk::Data
{
public:
    // публічні частини моделі
    Legal   legal;
    Meta    meta;
    Finance finance;

    void save() const;
    void load();

    std::vector <sdk::Symbol> tickersSymbolList() const;
    sdk::List <Ticker, Data> tickers;

private:
    Data(uint16_t parent);
    ~Data() = default;
    // блокуємо копії, можна рухати (за бажанням і move забороніть)
    Data(const Data&)           = delete;
    Data(      Data&&) noexcept = delete;
    Data& operator = (const Data&)           = delete;
    Data& operator = (      Data&&) noexcept = delete;

    friend class sdk::Instrument;
    friend class sdk::Market;
    friend class api::Eod;

    uint16_t _parent;
    Trackable _track;

    Instrument* parent() const;
    Instrument* instrument() const;

    void update_parent();
    Ticker& addTicker(const sdk::Symbol& symbol);

    friend QDataStream& operator << (QDataStream& s, Wire <const Data> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Data> d);
};

#endif // SDK_DATA_H
