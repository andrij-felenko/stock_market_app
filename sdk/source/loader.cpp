#include "loader.h"
#include <QtCore/QCoreApplication>
#include "api/eod/eod.h"

SDK& SDK::reference() { return *instance(); }
SDK* const SDK::instance()
{
    static SDK* instance = nullptr;
    if (instance == nullptr)
        instance = new SDK();
    return instance;
}

void SDK::init()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();

    setParent(qApp);

    _market = new sdk::Market(qApp);
    _market->loadMeta();
    if (_market->empty())
        sdk::api::Eod::getAllExchangeTag();

    _roster = new sdk::Roster(qApp);
    _roster->loadMeta();

    if (!_sessionFactory)
        _sessionFactory = [](QObject* parent) { return new sdk::Session(parent); };
    _session = _sessionFactory(qApp);
    _session->attachRoster(_roster);
}

void SDK::configureSessionFactory(std::function<sdk::Session*(QObject* parent)> factory)
{
    _sessionFactory = std::move(factory);
}

sdk::api::EndPoints* SDK::network() const { return sdk::apiEndPoints.ptr(); }
sdk::Market*         SDK::market()  const { return _market; }
sdk::Roster*         SDK::roster()  const { return _roster; }
sdk::Session*        SDK::session() const { return _session; }

SDK::SDK() : QObject(qApp), _market(nullptr), _roster(nullptr), _session(nullptr)
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
    _sessionFactory = [](QObject* parent) { return new sdk::Session(parent); };
}


// // ---------------------------------------------------------------------------------------------
// void sdk::Loader::prepare()
// {
//     qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
//     load_pre_data();

//     emit prepared();
// }

// void sdk::Loader::start()
// {
//     qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
//     load_data();
//     load_user();
// }
// // =============================================================================================




// // ---------------------------------------------------------------------------------------------
// void sdk::Loader::load_pre_data()
// {
//     market()->load_meta();
//     if (market()->empty()){
//         api::Eod::get_all_exchange_tag();
//     }

//     QMap <int, int> size;
//     for (const auto& it : _market->_instruments){
//         int cur = it->tickers_ptr().size();
//         size[cur] = size.value(cur, 0) + 1;
//     }
//     qDebug() << Q_FUNC_INFO << "Sizes:" << size;
// }

// void sdk::Loader::load_data()
// {
//     qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
//     for (const auto& it : _market->_instruments)
//         it->load();
// }

// void sdk::Loader::load_user()
// {
//     qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
//     _user = new data::User(this);
// }
// // =============================================================================================




// // ---------------------------------------------------------------------------------------------
// void sdk::Loader::update_data()
// {
//     //
// }
// // =============================================================================================
