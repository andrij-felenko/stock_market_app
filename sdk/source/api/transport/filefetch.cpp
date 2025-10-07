#include "api/transport/filefetch.h"
#include "loader.h"
#include "service/market.h"
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "core/isin.h"

sdk::api::FileFetch::FileFetch(QObject* parent) : Provider(QUrl(), parent) {}

sdk::api::FileFetch* sdk::api::FileFetch::instance()
{
    static FileFetch* self = nullptr;
    if (!self) self = new FileFetch(qApp);
    return self;
}


bool sdk::api::FileFetch::request(Request type, const QString& name,
                                  const sdk::Symbol& symbol, StringMap keys)
{
    qDebug() << Q_FUNC_INFO << name;
    if (type != Request::Text) return false;
    const auto urlStr = keys.value("url");
    if (urlStr.isEmpty()) return false;

    QUrl url(urlStr);
    if (!url.isValid()) return false;

    Call* post = add(type);
    post->name = name;
    post->suburl = url.toString();
    post->request()->setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                                  QNetworkRequest::NoLessSafeRedirectPolicy);
    post->request()->setRawHeader("User-Agent", "StockManager/1.0");
    post->request()->setRawHeader("Accept", "*/*");
    post->prepare();
    return true;
}


void sdk::api::FileFetch::handlerAnswer(Call* reply)
{
    // знайти тикер за tag (так само, як у AlphaVantage)
    qDebug() << Q_FUNC_INFO << reply->symbol << reply->name;
    auto in = Nexus->market()->findInstrument(reply->name.toLatin1());
    if (in == nullptr)
        return;

    in->create();
    in->data()->meta.updateLogo(sdk::Isin(reply->name.toLatin1()), reply->receiveData());

    // опційне кешування на диск
    // save_cache(name, data);

    in->save();
    in->release();
}
