#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QtQuick/QQuickWindow>

#include "utilities/res_dir.h"
#include "api/openai.h"
#include "api/eod.h"
#include "api/finnhub.h"
#include "api/twelvedata.h"
#include "data/market.h"
#include "api/alphavantage.h"
#include "api/marketstack.h"
#include "data/portfolio.h"
#include "model/search_tag.h"
#include "settings/network.h"
#include <QSslSocket>

#define qmlREGS qmlRegisterSingletonInstance

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");

    app.setOrganizationDomain("some.io");
    app.setOrganizationName("Andrij Felenko");
    app.setApplicationName("Stock manager");
    app.setWindowIcon(QIcon(":/rc/images/bull-market.png"));


    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, [](){ QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    qmlREGS <model::SearchTag> ("StockCpp", 1, 0, "SearchTagCpp", model::SearchTag::instance());

    qmlREGS <data::Market>    ("StockCpp", 1, 0, "MarketCpp",    data::Market   ::instance());
    qmlREGS <data::Portfolio> ("StockCpp", 1, 0, "PortfolioCpp", data::Portfolio::instance());

    qmlREGS <api::OpenAI>      ("StockCpp", 1, 0, "OpenAI",       api::OpenAI      ::instance());
    qmlREGS <api::Eod>         ("StockCpp", 1, 0, "EOD",          api::Eod         ::instance());
    qmlREGS <api::FinnHub>     ("StockCpp", 1, 0, "FinnHub",      api::FinnHub     ::instance());
    qmlREGS <api::TwelveData>  ("StockCpp", 1, 0, "TwelveData",   api::TwelveData  ::instance());
    qmlREGS <api::MarketStack> ("StockCpp", 1, 0, "MarketStack",  api::MarketStack ::instance());
    qmlREGS <api::AlphaVantage>("StockCpp", 1, 0, "AlphaVantage", api::AlphaVantage::instance());

    qmlREGS <settings::Network>("StockCpp", 1, 0, "SeetingsNetwork", settings::network());

    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    qDebug() << "Supports SSL: " << QSslSocket::supportsSsl();
    qDebug() << "Build uses: " << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "Runtime uses: " << QSslSocket::sslLibraryVersionString();

    engine.load("qrc:/qt/qml/Stock/main.qml");
    util::ResDir::list_qrc_files();

    return app.exec();
}
