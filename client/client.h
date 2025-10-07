#ifndef STOCK_MANAGER_LOADER_H
#define STOCK_MANAGER_LOADER_H

#include <QtCore/QObject>
#include <QtQml/QQmlApplicationEngine>

#include <loader.h>
#include "model/asset_list.h"
#include "model/ticker_list.h"
#include "model/search_tag.h"
#include <api/connectors/endpoints.h>

class Client_t final : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ClientCpp)
    // Q_PROPERTY(sdk::Account*   user   READ account CONSTANT)
    // Q_PROPERTY(sdk::Market* market READ market  CONSTANT)
    // Q_PROPERTY(model::SearchTag*      search_tag     READ     search_tag CONSTANT)
    // Q_PROPERTY(model::AssetList*      asset_model    READ    asset_model CONSTANT)
    // Q_PROPERTY(model::TickerList*  favorite_model    READ favorite_model CONSTANT)
    // Q_PROPERTY(sdk::api::EndPoints*   settings_network READ network        CONSTANT)

    // Q_PROPERTY(QString status   READ status     NOTIFY     statusChanged)
    // Q_PROPERTY(float percentage READ percentage NOTIFY percantageChanged)

    // Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

public:    
    static Client_t* const instance();
    static Client_t&      reference();

    void init();

    // sdk::Account* account() const;
    // sdk::Market*  market()  const;
    // model::SearchTag* search_tag() const;
    // model::AssetList* asset_model() const;
    // model::TickerList* favorite_model() const;
    // sdk::api::EndPoints* network() const;

    QString status() const;
    float percentage() const;

    bool visible() const;

public slots:
    void setVisible(bool v);

signals:
    void prepared();

    void     statusChanged();
    void percantageChanged();

    void visibleChanged(bool);

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
    Client_t();

    QQmlApplicationEngine* _engine;
    void reset_engine();
    void exit();

    // models
    model::AssetList*         _asset_model;
    model::TickerList*     _favorite_model;
    model::SearchTag* _tag_search;

    QString _status;
    float _percentage;

    bool _splash;
    bool _visible;
};

inline Client_t& Client = Client_t::reference();

#endif // STOCK_MANAGER_LOADER_H
