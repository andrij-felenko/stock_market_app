#include "settings/network.h"
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>

settings::Network* settings::network() { return Network::instance(); }

settings::Network::Network(QObject* parent) : QObject(parent)
{
    // set default
    if (key_av ().isEmpty()) setAlphaVantageKey("539EYFYAYCXFMWIL");
    if (key_eod().isEmpty()) setEodKey("683ebb8bc59b60.11043967");
    if (key_fh ().isEmpty()) setFinnHubKey("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
    if (key_ms ().isEmpty()) setMarketStackKey("c68c8ac43610203b7b46616e0bb8124a");
    if (key_td ().isEmpty()) setTwelveDataKey("f9b33ba1139a4b5e8c0572bcd1e11258");
    if (key_figi().isEmpty()) setFigiKey("48b9c0fc-ae20-4c7a-a05a-bee87459348b");

    if (key_oai().isEmpty()) setOpenaiKey("sk-proj-vccDzzJrmHvKungJmFIz_U5X_yZI3wvadiKedhBomYzXUNv"
                                          "4XVU7nP4l84VqJZ9TlMeVhQLmLXT3BlbkFJ21RmjsV5oOpor1XvYbNa"
                                          "c2aJjNBNoz-S7SF8C84Lx4fqZHxEODILJk6bo8PyzBPCfXjDuQcJQA");
}

settings::Network* settings::Network::instance()
{
    static Network* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Network(qApp);
    }
    return _instance;
}

QString settings::Network::key_av()  const { return QSettings().value("key_av"    ).toString(); }
QString settings::Network::key_eod() const { return QSettings().value("key_eod"   ).toString(); }
QString settings::Network::key_fh()  const { return QSettings().value("key_fh"    ).toString(); }
QString settings::Network::key_oai() const { return QSettings().value("key_openai").toString(); }
QString settings::Network::key_td()  const { return QSettings().value("key_td"    ).toString(); }
QString settings::Network::key_ms()  const { return QSettings().value("key_ms"    ).toString(); }
QString settings::Network::key_figi()const { return QSettings().value("key_figi"  ).toString(); }

void settings::Network::setAlphaVantageKey(const QString& new_key)
{
    if (key_av() == new_key)
        return;

    QSettings().setValue("key_av", new_key);
    emit key_avChanged(new_key);
}

void settings::Network::setEodKey(const QString& new_key)
{
    if (key_eod() == new_key)
        return;

    QSettings().setValue("key_eod", new_key);
    emit key_eodChanged(new_key);
}

void settings::Network::setFinnHubKey(const QString& new_key)
{
    if (key_fh() == new_key)
        return;

    QSettings().setValue("key_fh", new_key);
    emit key_fhChanged(new_key);
}

void settings::Network::setOpenaiKey(const QString& new_key)
{
    if (key_oai() == new_key)
        return;

    QSettings().setValue("key_openai", new_key);
    emit key_openaiChanged(new_key);
}

void settings::Network::setTwelveDataKey(const QString& new_key)
{
    if (key_td() == new_key)
        return;

    QSettings().setValue("key_td", new_key);
    emit key_tdChanged(new_key);
}

void settings::Network::setMarketStackKey(const QString& new_key)
{
    if (key_ms() == new_key)
        return;

    QSettings().setValue("key_ms", new_key);
    emit key_msChanged(new_key);
}

void settings::Network::setFigiKey(const QString& new_key)
{
    if (key_figi() == new_key)
        return;

    QSettings().setValue("key_figi", new_key);
    emit key_figiChanged(new_key);
}
