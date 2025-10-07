#ifndef STOCK_MANAGER_SDK_LOADER_H
#define STOCK_MANAGER_SDK_LOADER_H

#include <QtCore/QObject>

#include "sdk_def.h"
#include "service/market.h"
#include "service/roster.h"
#include "api/connectors/endpoints.h"

class SDK final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(sdk::Roster* roster READ roster CONSTANT)
    Q_PROPERTY(sdk::Market* market READ market CONSTANT)
    Q_PROPERTY(sdk::api::EndPoints* settings_network READ network CONSTANT)
public:
    static SDK* const instance();
    static SDK&      reference();

    void init();

    sdk::Market* market() const;
    sdk::Roster* roster() const;
    sdk::api::EndPoints* network() const;

signals:
    void prepared();

// protected:
//     void prepare();
//     void start();

//     void load_pre_data();
//     void load_data();
//     void load_user();

//     void update_data();

private:
    SDK();

    sdk::Market* _market;
    sdk::Roster* _roster;
};

inline SDK& Nexus = SDK::reference();

#endif // STOCK_MANAGER_SDK_LOADER_H
