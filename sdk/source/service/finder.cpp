#include "service/finder.h"

sdk::Finder::Finder(const sdk::Symbol& symbol) : _symbol(symbol)
{
    //
}

sdk::Symbol sdk::Finder::symbol() const { return _symbol; }

sdk::Ticker* sdk::Finder::operator->() const noexcept { return ticker; }
bool sdk::Finder::found() const { return ticker != nullptr; }
bool sdk::Finder::exist() const { return instrument != nullptr; }

bool sdk::Finder::ensure()
{
    if (found() || not exist()) return false;
    for (auto& it : instrument->create()->listings())
        if (it.symbol() == _symbol){
            ticker = &it;
            return true;
        }
    return false;
}
