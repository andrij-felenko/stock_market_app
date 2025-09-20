#ifndef SDK_DATA_H
#define SDK_DATA_H

#include "sdk_def.h"
#include "ticker.h"
#include "legal.h"
#include "meta.h"
#include "finance.h"

namespace api { class Eod; }

class sdk::Data : Trackable
{
public:
    // публічні частини моделі
    Legal   legal;
    Meta    meta;
    Finance finance;

    void save() const;
    void load();

    std::vector <sdk::Symbol> tickersSymbolList() const;

    // ----------------------- Listings -----------------------------------------------------------
    // блокуємо копії, можна рухати (за бажанням і move забороніть)
    Data(const Data&)           = delete;
    Data(      Data&&) noexcept = delete;
    Data& operator = (const Data&)           = delete;
    Data& operator = (      Data&&) noexcept = delete;

    std::size_t listingsCount() const noexcept;

    // ---- ітерабельний вигляд без доступу до контейнерних mutator'ів ----
    auto listings()       noexcept { return std::ranges::subrange(_list.begin(), _list.end()); }
    auto listings() const noexcept { return std::ranges::subrange(_list.begin(), _list.end()); }
    // ============================================================================================

private:
    Data(uint16_t parent);
    ~Data() = default;

    friend class sdk::Instrument;
    friend class sdk::Market;
    friend class api::Eod;

    uint16_t _parent;
    std::vector <Ticker> _list;

    void update_parent();
    Ticker& addTicker(const sdk::Symbol& symbol);

    friend QDataStream& operator << (QDataStream& s, const Data& d);
    friend QDataStream& operator >> (QDataStream& s,       Data& d);
};

#endif // SDK_DATA_H
