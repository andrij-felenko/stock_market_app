#include "filefetch.h"
#include "loader.h"
#include "data/market.h"
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

api::FileFetch::FileFetch(QObject* parent) : API(parent) {}

api::FileFetch* api::FileFetch::instance() {
    static FileFetch* self = nullptr;
    if (!self) self = new FileFetch(qApp);
    return self;
}

bool api::FileFetch::_request(Request type, QString name, StringMap keys) {
    qDebug() << Q_FUNC_INFO << name;
    if (type != Request::Text) return false;
    const auto urlStr = keys.value("url");
    if (urlStr.isEmpty()) return false;

    QUrl url(urlStr);
    if (!url.isValid()) return false;

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setRawHeader("User-Agent", "StockManager/1.0");
    req.setRawHeader("Accept", "*/*");

    _add_reply(type, _netmanager.get(req), name);
    return true;
}

void api::FileFetch::_handler_answer(Request type, QByteArray data, QString name, bool) {
    // знайти тикер за tag (так само, як у AlphaVantage)
    qDebug() << Q_FUNC_INFO << name;
    auto found = Nexus.market()->find(name);
    if (!found.has_value()) {
        found = Nexus.market()->find(name);
        if (!found.has_value()) return;
    }

    data::Ticker* t = found.value();
    t->instrument()->identity()->set_logo_bytes(data);

    // опційне кешування на диск
    // save_cache(name, data);

    t->instrument()->save();
    t->save();
}

void api::FileFetch::save_cache(const QString& tag, const QByteArray& bytes, const QString& ext) {
    // кладемо поряд із .tdsm у AppDataLocation/stocks/logos
    QString root = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(root + "/stocks/logos");
    QFile f(root + "/stocks/logos/" + tag + "." + ext);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(bytes);
        f.close();
    }
}
