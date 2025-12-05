#include "api/connectors/endpoints.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>

using namespace sdk;

namespace {

void loadEnvFile(const QString& filePath)
{
    QFile envFile(filePath);
    if (!envFile.exists())
        return;

    if (!envFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&envFile);
    while (!stream.atEnd()) {
        const auto line = stream.readLine().trimmed();

        if (line.isEmpty() || line.startsWith('#'))
            continue;

        const int separatorIndex = line.indexOf('=');
        if (separatorIndex <= 0)
            continue;

        const auto key = line.left(separatorIndex).trimmed();
        auto value = line.mid(separatorIndex + 1).trimmed();

        if ((value.startsWith('\"') && value.endsWith('\"')) || (value.startsWith('\'') && value.endsWith('\'')))
            value = value.mid(1, value.length() - 2);

        if (!key.isEmpty())
            qputenv(key.toUtf8(), value.toUtf8());
    }
}

void ensureEnvLoaded()
{
    static bool loaded = false;
    if (loaded)
        return;

    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir::current().filePath(".env"),
        appDir + "/.env",
        appDir + "/../.env"
    };

    for (const auto& candidate : candidates) {
        loadEnvFile(candidate);
    }

    loaded = true;
}

QString envValue(const QByteArray& key)
{
    return QString::fromUtf8(qgetenv(key.constData()));
}

}

sdk::api::EndPoints::EndPoints(QObject* parent) : QObject(parent)
{
    ensureEnvLoaded();

    const auto alphaVantage = envValue("ALPHAVANTAGE_API_KEY");
    const auto eodKey       = envValue("EOD_API_KEY");
    const auto finnhubKey   = envValue("FINNHUB_API_KEY");
    const auto marketStack  = envValue("MARKETSTACK_API_KEY");
    const auto twelveData   = envValue("TWELVEDATA_API_KEY");
    const auto figiKey      = envValue("FIGI_API_KEY");
    const auto openaiKey    = envValue("OPENAI_API_KEY");
    const auto stockUrl     = envValue("STOCK_SERVICE_URL");

    if (!alphaVantage.isEmpty()) setAlphaVantageKey(alphaVantage);
    if (!eodKey.isEmpty())       setEodKey(eodKey);
    if (!finnhubKey.isEmpty())   setFinnHubKey(finnhubKey);
    if (!marketStack.isEmpty())  setMarketStackKey(marketStack);
    if (!twelveData.isEmpty())   setTwelveDataKey(twelveData);
    if (!figiKey.isEmpty())      setFigiKey(figiKey);
    if (!openaiKey.isEmpty())    setOpenaiKey(openaiKey);
    if (!stockUrl.isEmpty())     setStockUrl(QUrl(stockUrl));
}

sdk::api::EndPoints* sdk::api::EndPoints::instance()
{
    static EndPoints* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new EndPoints(qApp);
    }
    return _instance;
}

QString api::EndPoints::key_av()    const { return QSettings().value("key_av"    ).toString(); }
QString api::EndPoints::key_eod()   const { return QSettings().value("key_eod"   ).toString(); }
QString api::EndPoints::key_fh()    const { return QSettings().value("key_fh"    ).toString(); }
QString api::EndPoints::key_oai()   const { return QSettings().value("key_openai").toString(); }
QString api::EndPoints::key_td()    const { return QSettings().value("key_td"    ).toString(); }
QString api::EndPoints::key_ms()    const { return QSettings().value("key_ms"    ).toString(); }
QString api::EndPoints::key_figi()  const { return QSettings().value("key_figi"  ).toString(); }
QUrl    api::EndPoints::url_stock() const { return QSettings().value("url_stock" ).toUrl(); }

void sdk::api::EndPoints::setAlphaVantageKey(const QString& new_key)
{
    if (key_av() == new_key)
        return;

    QSettings().setValue("key_av", new_key);
    emit key_avChanged(new_key);
}

void sdk::api::EndPoints::setEodKey(const QString& new_key)
{
    if (key_eod() == new_key)
        return;

    QSettings().setValue("key_eod", new_key);
    emit key_eodChanged(new_key);
}

void sdk::api::EndPoints::setFinnHubKey(const QString& new_key)
{
    if (key_fh() == new_key)
        return;

    QSettings().setValue("key_fh", new_key);
    emit key_fhChanged(new_key);
}

void sdk::api::EndPoints::setOpenaiKey(const QString& new_key)
{
    if (key_oai() == new_key)
        return;

    QSettings().setValue("key_openai", new_key);
    emit key_openaiChanged(new_key);
}

void sdk::api::EndPoints::setTwelveDataKey(const QString& new_key)
{
    if (key_td() == new_key)
        return;

    QSettings().setValue("key_td", new_key);
    emit key_tdChanged(new_key);
}

void sdk::api::EndPoints::setMarketStackKey(const QString& new_key)
{
    if (key_ms() == new_key)
        return;

    QSettings().setValue("key_ms", new_key);
    emit key_msChanged(new_key);
}

void sdk::api::EndPoints::setFigiKey(const QString& new_key)
{
    if (key_figi() == new_key)
        return;

    QSettings().setValue("key_figi", new_key);
    emit key_figiChanged(new_key);
}

void sdk::api::EndPoints::setStockUrl(const QUrl& new_url)
{
    if (url_stock() == new_url)
        return;

    QSettings().setValue("url_stock", new_url);
    emit url_stockChanged(new_url);
}
