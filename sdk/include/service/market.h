#ifndef SDK_SERVICE_MARKET_H
#define SDK_SERVICE_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include <vector>

#include "core/security/instrument.h"
#include "service/finder.h"
#include "api/eod/eod.h"

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
    friend void sdk::api::Eod::_handleExchange(sdk::api::Call* reply);
};

#endif // SDK_SERVICE_MARKET_H
