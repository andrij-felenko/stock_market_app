#include "api/filefetch.h"
#include "loader.h"
#include "services/market.h"
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "instrument/isin.h"

api::FileFetch::FileFetch(QObject* parent) : API(QUrl(), parent) {}

api::FileFetch* api::FileFetch::instance()
{
    static FileFetch* self = nullptr;
    if (!self) self = new FileFetch(qApp);
    return self;
}


bool api::FileFetch::_request(Request type, const QString& name,
                              const sdk::Symbol& symbol, StringMap keys)
{
    qDebug() << Q_FUNC_INFO << name;
    if (type != Request::Text) return false;
    const auto urlStr = keys.value("url");
    if (urlStr.isEmpty()) return false;

    QUrl url(urlStr);
    if (!url.isValid()) return false;

    Reply* request = _add(type);
    request->name = name;
    request->suburl = url.toString();
    request->_request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                                   QNetworkRequest::NoLessSafeRedirectPolicy);
    request->_request.setRawHeader("User-Agent", "StockManager/1.0");
    request->_request.setRawHeader("Accept", "*/*");
    request->prepare();
    return true;
}


void api::FileFetch::_handler_answer(Reply* reply)
{
    // знайти тикер за tag (так само, як у AlphaVantage)
    qDebug() << Q_FUNC_INFO << reply->symbol << reply->name;
    auto in = Nexus.market()->find_instrument(reply->name.toLatin1());
    if (in == nullptr)
        return;

    in->create();
    in->data()->meta.updateLogo(sdk::Isin(reply->name.toLatin1()),
                                                  reply->receive_data());

    // опційне кешування на диск
    // save_cache(name, data);

    in->save();
    in->release();
}
