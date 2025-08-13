#include "loader.h"
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickWindow>

#include "api/alphavantage.h"
#include "api/eod.h"
#include "api/finnhub.h"
#include "api/ibkr.h"
#include "api/marketstack.h"
#include "api/openai.h"
#include "api/twelvedata.h"
#include "api/yahoo.h"

#define qmlREGS qmlRegisterSingletonInstance
#define qmlREGU qmlRegisterUncreatableType

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
    registerSingletons();

    setParent(qApp);
    QObject::connect(this, &Loader::prepared, this, &Loader::start);

    _market = new data::Market(qApp);
    prepare();
}

void Loader::reset_engine()
{
    if (_engine != nullptr)
        delete _engine;

    _engine = new QQmlApplicationEngine(this);
    QObject::connect(_engine, &QQmlApplicationEngine::objectCreationFailed, this, &Loader::exit,
                     Qt::QueuedConnection);
}

settings::Network*     Loader::network()        const { return settings::Network::instance(); }
data::User*            Loader::account()        const { return _user; }
data::Market*          Loader::market()         const { return _market; }
model::SearchTag*      Loader::search_tag()     const { return _tag_search; }
model::AssetList*      Loader::asset_model()    const { return _asset_model; }
model::TickerList*     Loader::favorite_model() const { return _favorite_model; }

QString Loader::status()     const { return _status; }
float   Loader::percentage() const { return _percentage; }

Loader::Loader() : _engine(nullptr), _splash(false), _visible(false)
{
    //
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
    load_data();
    load_user();
    create_model();
    loadMainUI();
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::registerSingletons()
{
    QString str("Created in C++");
    qmlREGU     <data::User>   ("Cpp", 1, 0, "UserCpp",    str);
    qmlREGU     <data::Market> ("Cpp", 1, 0, "MarketCpp",  str);
    qmlREGU <settings::Network>("Cpp", 1, 0, "NetworkCpp", str);

    qmlREGU <model::SearchTag>     ("Cpp", 1, 0, "SearchTagCpp",      str);
    qmlREGU <model::AssetList>     ("Cpp", 1, 0, "AssetListCpp",      str);
    qmlREGU <model::TickerList>    ("Cpp", 1, 0, "InstrumentListCpp", str);

    // qmlREGU <Loader>("Cpp", 1, 0, "LoaderCpp", str);
    // --------------------------------------------------------------------------------------------
    qmlREGS <api::OpenAI>      ("Cpp", 1, 0, "OpenAI",       api::OpenAI      ::instance());
    qmlREGS <api::Eod>         ("Cpp", 1, 0, "EOD",          api::Eod         ::instance());
    qmlREGS <api::FinnHub>     ("Cpp", 1, 0, "FinnHub",      api::FinnHub     ::instance());
    qmlREGS <api::TwelveData>  ("Cpp", 1, 0, "TwelveData",   api::TwelveData  ::instance());
    qmlREGS <api::MarketStack> ("Cpp", 1, 0, "MarketStack",  api::MarketStack ::instance());
    qmlREGS <api::AlphaVantage>("Cpp", 1, 0, "AlphaVantage", api::AlphaVantage::instance());

    qmlREGS <Loader>("Cpp", 1, 0, "Nexus", this);
}

void Loader::create_model()
{
    _tag_search = new model::SearchTag(this);
    _asset_model = new model::AssetList(account()->asset_list(), this);
    _favorite_model = new model::TickerList(account()->favorite_list(), account(),
                                            &data::User::favoriteListUpdated);
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::loadScreenSaver()
{
    reset_engine();
    _engine->load("qrc:/qt/qml/Stock/SplashScreen.qml");
}

bool Loader::visible() const { return _visible; }
void Loader::setVisible(bool v)
{
    _visible = v;
    emit visibleChanged(v);
}

void Loader::loadMainUI()
{
    reset_engine();
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
}

void Loader::load_data()
{
    for (const auto& it : _market->_instruments){
        it->load();
    }
}

void Loader::load_user()
{
    _user = new data::User(this);
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Loader::update_data()
{
    //
}
// ================================================================================================
