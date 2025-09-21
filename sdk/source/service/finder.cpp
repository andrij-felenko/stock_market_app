#include "service/finder.h"

sdk::market::Finder::Finder(const sdk::Symbol& symbol) : _symbol(symbol)
{
    //
}

sdk::Symbol sdk::market::Finder::symbol() const { return _symbol; }

sdk::Ticker* sdk::market::Finder::operator->() const noexcept { return ticker; }
bool sdk::market::Finder::found() const { return ticker != nullptr; }
bool sdk::market::Finder::exist() const { return instrument != nullptr; }

bool sdk::market::Finder::ensure()
{
    if (found() || not exist()) return false;
    for (auto& it : instrument->create()->tickers)
        if (it.symbol() == _symbol){
            ticker = &it;
            return true;
        }
    return false;
}
