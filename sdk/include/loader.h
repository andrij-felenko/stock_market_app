#ifndef STOCK_MANAGER_SDK_LOADER_H
#define STOCK_MANAGER_SDK_LOADER_H

#include <QtCore/QObject>

#include "sdk_def.h"
#include "services/market.h"
// #include "data/user.h"
// #include "data/market.h"
#include "settings/network.h"


class SDK final : public QObject
{
    Q_OBJECT
//     Q_PROPERTY(data::User*   user   READ account CONSTANT)
    Q_PROPERTY(sdk::Market* market READ market  CONSTANT)
    Q_PROPERTY(settings::Network*   settings_network READ network CONSTANT)
public:
    static SDK* const instance();
    static SDK&      reference();

    void init();

//     data::User* account() const;
    sdk::Market* market() const;
    settings::Network* network() const;

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

//     data::User* _user;
    sdk::Market* _market;
};

inline SDK& Nexus = SDK::reference();

#endif // STOCK_MANAGER_SDK_LOADER_H
