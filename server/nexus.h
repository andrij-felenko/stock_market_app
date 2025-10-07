#ifndef STOCK_MANAGER_SERVER_NEXUS_H
#define STOCK_MANAGER_SERVER_NEXUS_H

#include <StockSDK/Loader>

class Nexus_t final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(sdk::Roster* roster READ roster CONSTANT)
    Q_PROPERTY(sdk::Market* market READ market CONSTANT)
    Q_PROPERTY(sdk::api::EndPoints* settings_network READ network CONSTANT)
public:
    static Nexus_t* const instance();
    static Nexus_t&      reference();

    void init();

    sdk::Market* market() const;
    sdk::Roster* roster() const;
    sdk::api::EndPoints* network() const;

private:
    Nexus_t();

    sdk::Market* _market;
    sdk::Roster* _roster;
};

inline constexpr sdk::Singleton <Nexus_t> DB {};

#endif // STOCK_MANAGER_SERVER_NEXUS_H
