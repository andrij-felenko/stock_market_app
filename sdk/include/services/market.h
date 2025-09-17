#ifndef DATA_MARKET_H
#define DATA_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include <vector>

#include "instrument/instrument.h"
#include "instrument/ticker.h"
#include "instrument/meta.h"
#include "api/eod.h"

class SDK;
namespace sdk   { class Loader; }
namespace model { class SearchTag; }

namespace api {
    class AlphaVantage;
    class Figi;
    class FinnHub;
    class TwelveData;
    class MarketStack;
}

class sdk::Market : public QObject
{
    Q_OBJECT
public:    
    class TickerPtr {
    public:
        TickerPtr(sdk::Symbol symbol);

        sdk::Symbol symbol() { return _symbol; }
        Ticker* ticker = nullptr;
        Instrument* instrument = nullptr;

        Ticker* operator->() const noexcept { return ticker; }
        bool found() const { return ticker != nullptr; }
        bool exist() const { return instrument != nullptr; }

    private:
        bool ensure(){
            if (found() || not exist()) return false;
            for (auto& it : instrument->create()->listings())
                if (it.symbol() == _symbol){
                    ticker = &it;
                    return true;
                }
            return false;
        }

        friend class api::Eod;
        friend class api::AlphaVantage;
        friend class api::Figi;
        friend class api::FinnHub;
        friend class api::TwelveData;
        friend class api::MarketStack;

    private:
        sdk::Symbol _symbol;
    };

    Market(QObject* parent = nullptr);
    Market& operator = (const Market&) = delete;

    TickerPtr   find_ticker    (const sdk::Symbol& tag);
    Instrument* find_instrument(const sdk::Isin&  isin);

    void save_meta() const;
    bool empty() const;

private:
    uint16_t _last_index = 0;

    std::vector <Instrument> _instruments;

    void load_meta();

    friend class ::SDK;

    TickerPtr add_ticker(const sdk::Symbol& tag, const Isin& isin,
                         const QString& name, Instype type);

    friend void api::Eod::_handle_exchange(api::Reply* reply);
};

#endif
