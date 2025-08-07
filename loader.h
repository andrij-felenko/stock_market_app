#ifndef STOCK_MANAGER_LOADER_H
#define STOCK_MANAGER_LOADER_H

#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>

#include "data/user.h"
#include "data/market.h"
#include "model/asset_list.h"
#include "model/instrument_list.h"
#include "model/search_tag.h"
#include "settings/network.h"

class Loader final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(data::User*   user   READ account CONSTANT)
    Q_PROPERTY(data::Market* market READ market  CONSTANT)
    Q_PROPERTY(model::SearchTag*      search_tag     READ     search_tag CONSTANT)
    Q_PROPERTY(model::AssetList*      asset_model    READ    asset_model CONSTANT)
    Q_PROPERTY(model::InstrumentList* favorite_model READ favorite_model CONSTANT)
    Q_PROPERTY(settings::Network*   settings_network READ network        CONSTANT)

    Q_PROPERTY(QString status   READ status     NOTIFY     statusChanged)
    Q_PROPERTY(float percentage READ percentage NOTIFY percantageChanged)
public:
    static Loader* const instance();
    static Loader&      reference();

    void init();

    data::User* account() const;
    data::Market* market() const;
    model::SearchTag* search_tag() const;
    model::AssetList* asset_model() const;
    model::InstrumentList* favorite_model() const;
    settings::Network* network() const;

    QString status() const;
    float percentage() const;

signals:
    void prepared();

    void     statusChanged();
    void percantageChanged();

protected:
    void prepare();
    void start();

    void registerSingletons();
    void create_model();

    void loadScreenSaver();
    void loadMainUI();

    void load_pre_data();
    void load_data();
    void load_user();

    void update_data();

private:
    Loader();

    QQmlApplicationEngine* _engine;
    void exit();

    // models
    model::AssetList*         _asset_model;
    model::InstrumentList* _favorite_model;
    model::SearchTag* _tag_search;

    data::User* _user;
    data::Market* _market;
    settings::Network* _network;

    QString _status;
    float _percentage;
};

inline Loader& Nexus = Loader::reference();

#endif // STOCK_MANAGER_LOADER_H
