#include "loader.h"
#include <QtCore/QCoreApplication>
#include <QtQuick/QQuickWindow>

#include "api/alphavantage.h"
#include "api/eod.h"
#include "api/finnhub.h"
#include "api/ibkr.h"
#include "api/marketstack.h"
#include "api/openai.h"
#include "api/twelvedata.h"
#include "api/yahoo.h"

sdk::Loader* const sdk::Loader::instance() {
    static Loader* instance = nullptr;
    if (instance == nullptr)
        instance = new Loader();
    return instance;
}
sdk::Loader& sdk::Loader::reference() { return *instance(); }

void sdk::Loader::init()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();

    setParent(qApp);
    QObject::connect(this, &Loader::prepared, this, &Loader::start);

    _market = new data::Market(qApp);
    prepare();
}

settings::Network* sdk::Loader::network() const { return settings::Network::instance(); }
data::User*        sdk::Loader::account() const { return _user; }
data::Market*      sdk::Loader::market()  const { return _market; }

sdk::Loader::Loader() : QObject(qApp)
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
}


// ------------------------------------------------------------------------------------------------
void sdk::Loader::prepare()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    load_pre_data();

    emit prepared();
}

void sdk::Loader::start()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    load_data();
    load_user();
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void sdk::Loader::load_pre_data()
{
    market()->load_meta();
    if (market()->empty()){
        api::Eod::get_all_exchange_tag();
    }

    QMap <int, int> size;
    for (const auto& it : _market->_instruments){
        int cur = it->tickers_ptr().size();
        size[cur] = size.value(cur, 0) + 1;
    }
    qDebug() << Q_FUNC_INFO << "Sizes:" << size;
}

void sdk::Loader::load_data()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    for (const auto& it : _market->_instruments)
        it->load();
}

void sdk::Loader::load_user()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    _user = new data::User(this);
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void sdk::Loader::update_data()
{
    //
}
// ================================================================================================
