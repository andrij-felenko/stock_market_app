#include "loader.h"
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickWindow>

#include "api/alphavantage.h"
#include "api/eod.h"
#include "api/figi.h"
#include "api/finnhub.h"
#include "api/ibkr.h"
#include "api/marketstack.h"
#include "api/openai.h"
#include "api/tradernet.h"
#include "api/twelvedata.h"
#include "api/yahoo.h"

#define qmlREGS qmlRegisterSingletonInstance

void Loader::exit() { QGuiApplication::exit(-1); }
Loader* const Loader::instance() {
    static Loader* instance = nullptr;
    if (instance == nullptr)
        instance = new Loader();
    return instance;
}
Loader& Loader::reference() { return *instance(); }

void Loader::init()
{
    setParent(qApp);
    _engine = new QQmlApplicationEngine(this);
    QObject::connect(_engine, &QQmlApplicationEngine::objectCreationFailed, this, &Loader::exit,
                     Qt::QueuedConnection);
    QObject::connect(this, &Loader::prepared, this, &Loader::start);

    _market = new data::Market(qApp);
    prepare();
}

settings::Network*     Loader::network()        const { return _network; }
data::User*            Loader::account()        const { return _user; }
data::Market*          Loader::market()         const { return _market; }
model::SearchTag*      Loader::search_tag()     const { return _tag_search; }
model::AssetList*      Loader::asset_model()    const { return _asset_model; }
model::InstrumentList* Loader::favorite_model() const { return _favorite_model; }

QString Loader::status()     const { return _status; }
float   Loader::percentage() const { return _percentage; }

Loader::Loader()
{
    // there is main runner of application
    // QTimer::singleShot(250, this, &Loader::prepare);
}


// ------------------------------------------------------------------------------------------------
void Loader::prepare()
{
    loadScreenSaver();
    load_pre_data();

    emit prepared();
}

void Loader::start()
{
    //
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::registerSingletons()
{
    // qmlREGS <model::AssetList>      ("StockCpp", 1, 0, "AssetModel",     _asset_model);
    // qmlREGS <model::InstrumentList> ("StockCpp", 1, 0, "MarketModel", _favorite_model);
    // qmlREGS <model::SearchTag> ("StockCpp", 1, 0, "SearchTagCpp", _tag_search);

    qmlREGS <api::OpenAI>      ("StockCpp", 1, 0, "OpenAI",       api::OpenAI      ::instance());
    qmlREGS <api::Eod>         ("StockCpp", 1, 0, "EOD",          api::Eod         ::instance());
    qmlREGS <api::FinnHub>     ("StockCpp", 1, 0, "FinnHub",      api::FinnHub     ::instance());
    qmlREGS <api::TwelveData>  ("StockCpp", 1, 0, "TwelveData",   api::TwelveData  ::instance());
    qmlREGS <api::MarketStack> ("StockCpp", 1, 0, "MarketStack",  api::MarketStack ::instance());
    qmlREGS <api::AlphaVantage>("StockCpp", 1, 0, "AlphaVantage", api::AlphaVantage::instance());

    // qmlREGS <data::User>       ("StockCpp", 1, 0, "CurrentAccount", _user);
    // qmlREGS <settings::Network>("StockCpp", 1, 0, "SeetingsNetwork", _network);
}

void Loader::create_model()
{
    _asset_model = new model::AssetList(account()->asset_list(), this);
    _favorite_model = new model::InstrumentList(account()->favorite_list(), account(),
                                                &data::User::favoriteListUpdated);
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::loadScreenSaver()
{
    _engine->load("qrc:/qt/qml/Stock/SplashScreen.qml");
}

void Loader::loadMainUI()
{
    _engine->load("qrc:/qt/qml/Stock/main.qml");
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::load_pre_data()
{
    market()->load_instruments();
    if (market()->empty()){
        market()->load_ticker_meta();

        if (market()->_ticker_meta.empty())
            api::Eod::get_all_exchange_tag();
        else
            _market->clusterise_ticker_meta();
    }


    // qDebug() << Q_FUNC_INFO;
    // QThread* thread = QThread::create([this]() {
    //     load_instruments();
    //     if (_instruments.empty())
    //         load_ticker_meta();
    // });
    // connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    // thread->start();

    // if (_ticker_meta.empty())
    //     QMetaObject::invokeMethod(this, [](){ api::Eod::get_all_exchange_tag(); },
    //                               Qt::QueuedConnection);
    // else if (_instruments.empty())
    //     clusterise_ticker_meta();
}

void Loader::load_data()
{
    //
}

void Loader::load_user()
{
    //
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::update_data()
{
    //
}
// ================================================================================================
