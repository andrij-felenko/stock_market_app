#include "nexus.h"

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
    sdk::Nexus->init();
}

sdk::Market* Nexus_t::market() const { return sdk::Nexus->market(); }
sdk::Roster* Nexus_t::roster() const { return sdk::Nexus->roster(); }

sdk::api::EndPoints* Nexus_t::network() const { return sdk::Nexus->network(); }

Nexus_t::Nexus_t() : QObject(qApp)
{
    qDebug() << Q_FUNC_INFO << QDateTime::currentDateTime();
}
