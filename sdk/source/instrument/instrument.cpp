#include "instrument/instrument.h"

sdk::Instrument::Instrument(const Isin& isin) : _isin(isin)
{
    //
}

sdk::Data* const sdk::Instrument::data() const { return _data; }
bool sdk::Instrument::loaded() const { return _data != nullptr; }

sdk::Data* sdk::Instrument::create()
{
    if (_data == nullptr)
        _data = new Data();
    return _data;
}

sdk::Country sdk::Instrument::country() const { return _isin.country(); }
