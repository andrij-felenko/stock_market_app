#include "api/connectors/endpoints.h"
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>

using namespace sdk;

sdk::api::EndPoints::EndPoints(QObject* parent) : QObject(parent)
{
    // set default
    if (key_av ().isEmpty()) setAlphaVantageKey("539EYFYAYCXFMWIL");
    if (key_eod().isEmpty()) setEodKey("683ebb8bc59b60.11043967");
    if (key_fh ().isEmpty()) setFinnHubKey("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
    if (key_ms ().isEmpty()) setMarketStackKey("c68c8ac43610203b7b46616e0bb8124a");
    if (key_td ().isEmpty()) setTwelveDataKey("f9b33ba1139a4b5e8c0572bcd1e11258");
    if (key_figi().isEmpty()) setFigiKey("48b9c0fc-ae20-4c7a-a05a-bee87459348b");
    if (url_stock().isEmpty()) setStockUrl(QUrl("127.0.0.1"));

    if (key_oai().isEmpty())
        setOpenaiKey("sk-proj-vccDzzJrmHvKungJmFIz_U5X_yZI3wvadiKedhBomYzXUNv"
                     "4XVU7nP4l84VqJZ9TlMeVhQLmLXT3BlbkFJ21RmjsV5oOpor1XvYbNa"
                     "c2aJjNBNoz-S7SF8C84Lx4fqZHxEODILJk6bo8PyzBPCfXjDuQcJQA");
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
