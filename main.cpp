#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QtQuick/QQuickWindow>

#include "model/asset_list.h"
#include "model/instrument_list.h"
#include "api/openai.h"
#include "api/eod.h"
#include "api/finnhub.h"
#include "api/twelvedata.h"
#include "api/alphavantage.h"
#include "api/marketstack.h"
#include "model/search_tag.h"
#include "data/user.h"
#include "settings/network.h"
#include <QSslSocket>
#include <qstandardpaths.h>
#include "model/asset_list.h"
#include "model/instrument_list.h"

#define qmlREGS qmlRegisterSingletonInstance

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");

    app.setOrganizationDomain("some.io");
    app.setOrganizationName("AndrijFelenko");
    app.setApplicationName("StockManager");
    app.setWindowIcon(QIcon(":/rc/images/Stock_app_logo2.png"));

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, [](){ QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    data::Market::instance();
    auto    asset_model = new model::AssetList(data::account()->asset_list(), &app);

    auto favorite_model = new model::InstrumentList(data::account()->favorite_list(),
                                                    data::account(),
                                                    &data::User::favoriteListUpdated);

    qmlREGS <model::AssetList>      ("StockCpp", 1, 0, "AssetModel",     asset_model);
    qmlREGS <model::InstrumentList> ("StockCpp", 1, 0, "MarketModel", favorite_model);
    qmlREGS <model::SearchTag> ("StockCpp", 1, 0, "SearchTagCpp", model::SearchTag::instance());

    qmlREGS <api::OpenAI>      ("StockCpp", 1, 0, "OpenAI",       api::OpenAI      ::instance());
    qmlREGS <api::Eod>         ("StockCpp", 1, 0, "EOD",          api::Eod         ::instance());
    qmlREGS <api::FinnHub>     ("StockCpp", 1, 0, "FinnHub",      api::FinnHub     ::instance());
    qmlREGS <api::TwelveData>  ("StockCpp", 1, 0, "TwelveData",   api::TwelveData  ::instance());
    qmlREGS <api::MarketStack> ("StockCpp", 1, 0, "MarketStack",  api::MarketStack ::instance());
    qmlREGS <api::AlphaVantage>("StockCpp", 1, 0, "AlphaVantage", api::AlphaVantage::instance());

    qmlREGS <data::User>       ("StockCpp", 1, 0, "CurrentAccount", data::account());
    qmlREGS <settings::Network>("StockCpp", 1, 0, "SeetingsNetwork", settings::network());

    qDebug() << "Device supports OpenSSL: ";
    qDebug() << "Supports SSL: " << QSslSocket::supportsSsl();
    qDebug() << "Runtime uses: " << QSslSocket::sslLibraryVersionString();
    qDebug() << "Build uses: "   << QSslSocket::sslLibraryBuildVersionString();

    engine.load("qrc:/qt/qml/Stock/main.qml");
    // util::ResDir::list_qrc_files();

    return app.exec();
}
