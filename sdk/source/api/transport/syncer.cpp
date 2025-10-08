#include "api/transport/syncer.h"

sdk::api::Syncer::Syncer(QObject* parent) : Provider(QUrl(""), parent)
{
    //
}

sdk::api::Syncer* sdk::api::Syncer::instance()
{
    static Syncer* _instance = nullptr;
    if (_instance == nullptr)
        _instance = new Syncer(qApp);

    return _instance;
}

bool sdk::api::Syncer::request(Request type, const QString& name,
                               const sdk::Symbol& symbol, StringMap keys)
{
    return true;
}

void sdk::api::Syncer::handlerAnswer(Call* reply)
{
    qDebug() << "handler answer eod";

    switch (reply->type()){
        // case api::Request::Exchange: { _handleExchange(reply); break; }
        // case api::Request::Candle:   { _handleCandle  (reply); break; }
        // case api::Request::Info:     { _handleInfo    (reply); break; }
        default:;
    }
}
