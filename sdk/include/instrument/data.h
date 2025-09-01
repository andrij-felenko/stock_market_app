#ifndef SDK_DATA_H
#define SDK_DATA_H

#include "sdk.h"
#include "ticker.h"
#include "legal.h"
#include "meta.h"
#include "finance.h"

class sdk::Data : Trackable
{
public:
    // публічні частини моделі
    Legal   legal;
    Meta    meta;
    Finance finance;

    void save() const;
    void load();

    // ----------------------- Listings -----------------------------------------------------------
    // блокуємо копії, можна рухати (за бажанням і move забороніть)
    Data(const Data&)           = delete;
    Data(      Data&&) noexcept = default;
    Data& operator = (const Data&)           = delete;
    Data& operator = (      Data&&) noexcept = default;

    // ---- додавання лістингів (видалення ззовні неможливе) ----
    template<class... Args>
    Ticker& emplaceListing(Args&&... args) {
        _list.emplace_back(std::forward<Args>(args)...);
        return _list.back();
    }

    Ticker& addListing(const Ticker& t);
    void reserveListings(std::size_t n);
    std::size_t listingsCount() const noexcept;

    // ---- ітерабельний вигляд без доступу до контейнерних mutator'ів ----
    auto listings()       noexcept { return std::ranges::subrange(_list.begin(), _list.end()); }
    auto listings() const noexcept { return std::ranges::subrange(_list.begin(), _list.end()); }
    // ============================================================================================

private:
    Data() = default;
    ~Data() = default;
    friend Instrument;

    std::vector<Ticker> _list; // приховано: зовні нема erase/clear/shrink_to_fit
};

#endif // SDK_DATA_H
