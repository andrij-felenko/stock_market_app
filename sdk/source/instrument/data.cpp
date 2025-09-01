#include "instrument/data.h"

sdk::Ticker& sdk::Data::addListing(const Ticker& t) {
    _list.push_back(t);
    return _list.back();
}

void sdk::Data::reserveListings(std::size_t n) { _list.reserve(n); }

std::size_t sdk::Data::listingsCount() const noexcept { return _list.size(); }


void sdk::Data::save() const
{
    //
}

void sdk::Data::load()
{
    //
}
