#ifndef STOCK_MANAGER_SDK_LOADER_H
#define STOCK_MANAGER_SDK_LOADER_H

#include <QtCore/QObject>

#include "data/user.h"
#include "data/market.h"
#include "settings/network.h"

namespace sdk { class Loader; }

class sdk::Loader final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(data::User*   user   READ account CONSTANT)
    Q_PROPERTY(data::Market* market READ market  CONSTANT)
    Q_PROPERTY(settings::Network*   settings_network READ network CONSTANT)
public:    
    static Loader* const instance();
    static Loader&      reference();

    void init();

    data::User* account() const;
    data::Market* market() const;
    settings::Network* network() const;

signals:
    void prepared();

protected:
    void prepare();
    void start();

    void load_pre_data();
    void load_data();
    void load_user();

    void update_data();

private:
    Loader();

    data::User* _user;
    data::Market* _market;
};

inline sdk::Loader& Nexus = sdk::Loader::reference();

#endif // STOCK_MANAGER_SDK_LOADER_H
