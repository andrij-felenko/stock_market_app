#include "loader.h"
#include <QtCore/QCoreApplication>
#include <QtQuick/QQuickWindow>

#include "api/ibkr.h"
#include "api/yahoo.h"

SDK* const SDK::instance() {
    static SDK* instance = nullptr;
    if (instance == nullptr)
        instance = new SDK();
    return instance;
}
SDK& SDK::reference() { return *instance(); }

void SDK::init()
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();

    setParent(qApp);

    _market = new sdk::Market(qApp);
    _market->loadMeta();
}

settings::Network* SDK::network() const { return settings::Network::instance(); }
// data::User*        sdk::Loader::account() const { return _user; }
sdk::Market*       SDK::market()  const { return _market; }

SDK::SDK() : QObject(qApp)
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
}


// // ------------------------------------------------------------------------------------------------
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
// // ================================================================================================




// // ------------------------------------------------------------------------------------------------
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
// // ================================================================================================




// // ------------------------------------------------------------------------------------------------
// void sdk::Loader::update_data()
// {
//     //
// }
// // ================================================================================================
