#include "nexus.h"

#include <QDateTime>
#include <QDebug>

Nexus_t& Nexus_t::reference() { return *instance(); }
Nexus_t* const Nexus_t::instance()
{
    static Nexus_t* instance = nullptr;
    if (instance == nullptr)
        instance = new Nexus_t();
    return instance;
}

void Nexus_t::init()
{
    sdk::Nexus->configureSessionFactory([](QObject* parent) -> sdk::Session* {
        return new Session(parent);
    });
    sdk::Nexus->init();
    _session = qobject_cast<Session*>(sdk::Nexus->session());
}

sdk::Market* Nexus_t::market() const { return sdk::Nexus->market(); }
sdk::Roster* Nexus_t::roster() const { return sdk::Nexus->roster(); }

sdk::api::EndPoints* Nexus_t::network() const { return sdk::Nexus->network(); }
Session* Nexus_t::session() const { return _session; }

Nexus_t::Nexus_t() : QObject(qApp)
{
    qInfo() << "Server nexus initialized at" << QDateTime::currentDateTime();
    _session = nullptr;
}
