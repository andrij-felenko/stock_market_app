#include "client.h"
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickWindow>
#include <QDateTime>
#include <QtQml/qqml.h>

#include <api/eod/alphavantage.h>
#include <api/eod/eod.h>
#include <api/eod/finnhub.h>
#include <api/broker/ibkr.h>
#include <api/eod/marketstack.h>
#include <api/ai/openai.h>
#include <api/eod/twelvedata.h>
#include <api/eod/yahoo.h>

#define qmlREGS qmlRegisterSingletonInstance
#define qmlREGU qmlRegisterUncreatableType

void Client_t::exit() { QGuiApplication::exit(-1); }
Client_t* const Client_t::instance() {
    static Client_t* instance = nullptr;
    if (instance == nullptr)
        instance = new Client_t();
    return instance;
}
Client_t& Client_t::reference() { return *instance(); }

void Client_t::init()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    registerSingletons();

    setParent(qApp);
    QObject::connect(this, &Client_t::prepared, this, &Client_t::start);

    sdk::Nexus->init();
    prepare();
}

void Client_t::reset_engine()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    if (_engine != nullptr)
        delete _engine;

    _engine = new QQmlApplicationEngine(this);
    QObject::connect(_engine, &QQmlApplicationEngine::objectCreationFailed, this, &Client_t::exit,
                     Qt::QueuedConnection);
}

// sdk::api::EndPoints*   Client_t::network()        const { return Nexus.network(); }
// sdk::Account*          Client_t::account()        const { return nullptr; }
// sdk::Market*           Client_t::market()         const { return Nexus.market(); }
// model::SearchTag*      Client_t::search_tag()     const { return _tag_search; }
// model::AssetList*      Client_t::asset_model()    const { return _asset_model; }
// model::TickerList*     Client_t::favorite_model() const { return _favorite_model; }

QString Client_t::status()     const { return _status; }
float   Client_t::percentage() const { return _percentage; }

Client_t::Client_t() : _engine(nullptr), _splash(false), _visible(false)
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
}


// ------------------------------------------------------------------------------------------------
void Client_t::prepare()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    loadScreenSaver();
    load_pre_data();

    emit prepared();
}

void Client_t::start()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    load_data();
    load_user();
    create_model();
    loadMainUI();
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Client_t::registerSingletons()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    QString str("Created in C++");
    // qmlREGU     <data::User>   ("Cpp", 1, 0, "UserCpp",    str);
    // qmlREGU     <data::Market> ("Cpp", 1, 0, "MarketCpp",  str);
    // qmlREGU <settings::Network>("Cpp", 1, 0, "NetworkCpp", str);

    qmlREGU <model::SearchTag>     ("Cpp", 1, 0, "SearchTagCpp",      str);
    qmlREGU <model::AssetList>     ("Cpp", 1, 0, "AssetListCpp",      str);
    qmlREGU <model::TickerList>    ("Cpp", 1, 0, "InstrumentListCpp", str);

    // qmlREGU <Loader>("Cpp", 1, 0, "LoaderCpp", str);
    // --------------------------------------------------------------------------------------------
    // qmlREGS <api::OpenAI>      ("Cpp", 1, 0, "OpenAI",       api::OpenAI      ::instance());
    // qmlREGS <api::Eod>         ("Cpp", 1, 0, "EOD",          api::Eod         ::instance());
    // qmlREGS <api::FinnHub>     ("Cpp", 1, 0, "FinnHub",      api::FinnHub     ::instance());
    // qmlREGS <api::TwelveData>  ("Cpp", 1, 0, "TwelveData",   api::TwelveData  ::instance());
    // qmlREGS <api::MarketStack> ("Cpp", 1, 0, "MarketStack",  api::MarketStack ::instance());
    // qmlREGS <api::AlphaVantage>("Cpp", 1, 0, "AlphaVantage", api::AlphaVantage::instance());

    qmlREGS <SDK>("Cpp", 1, 0, "Nexus", sdk::Nexus.ptr());
}

void Client_t::create_model()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    // _tag_search = new model::SearchTag(this);
    // _asset_model = new model::AssetList(account()->asset_list(), this);
    // _favorite_model = new model::TickerList(account()->favorite_list(), account(),
    //                                         &data::User::favoriteListUpdated);
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Client_t::loadScreenSaver()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    reset_engine();
    _engine->load("qrc:/qt/qml/Stock/SplashScreen.qml");
}

bool Client_t::visible() const { return _visible; }
void Client_t::setVisible(bool v)
{
    _visible = v;
    emit visibleChanged(v);
}

void Client_t::loadMainUI()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    reset_engine();
    _engine->load("qrc:/qt/qml/Stock/main.qml");
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Client_t::load_pre_data()
{
    // market()->load_meta();
    // if (market()->empty()){
    //     api::Eod::get_all_exchange_tag();
    // }

    // QMap <int, int> size;
    // for (const auto& it : _market->_instruments){
    //     int cur = it->tickers_ptr().size();
    //     size[cur] = size.value(cur, 0) + 1;
    // }
    // qDebug() << Q_FUNC_INFO << "Sizes:" << size;
}

void Client_t::load_data()
{
    // qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    // for (const auto& it : market()->_instruments)
    //     it->load();
}

void Client_t::load_user()
{
    // qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    // _user = new data::User(this);
}
// ================================================================================================




// ------------------------------------------------------------------------------------------------
void Client_t::update_data()
{
    //
}
// ================================================================================================
