#ifndef SDK_SERVICES_MARKET_H
#define SDK_SERVICES_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include <vector>

#include "instrument/instrument.h"
#include "services/finder.h"
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
    Market(QObject* parent = nullptr);
    Market& operator = (const Market&) = delete;

    void        findTicker    (Finder& finder);
    Finder      findTicker    (const sdk::Symbol& tag);
    Instrument* findInstrument(const sdk::Isin&  isin);

    void saveMeta() const;
    bool empty() const;

private:
    uint16_t _last_index = 0;

    std::vector <Instrument> _instruments;
    Finder addTicker(const sdk::Symbol& tag, const Isin& isin, const QString& name, Instype type);

    void loadMeta();

    friend class ::SDK;
    friend void api::Eod::_handleExchange(api::Reply* reply);
};

#endif // SDK_SERVICES_MARKET_H
