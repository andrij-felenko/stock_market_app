#ifndef SDK_INSTRUMENT_QUARTEL_DATA_H
#define SDK_INSTRUMENT_QUARTEL_DATA_H

#include "sdk.h" // IWYU pragma: keep
#include "symbol.h"

class sdk::QuartelData
{
public:
    QuartelData(uint16_t year = QDate().year(), Quartel q = Quartel::Annual);

    Quartel quartel() const;
    uint16_t year() const;

private:
    uint16_t _year = QDate().year();
    Quartel _quartel = Quartel::Annual;
};

#endif // SDK_INSTRUMENT_QUARTEL_DATE_H
