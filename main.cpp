#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>

// #include "utilities/res_dir.h"
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

#define qmlREGS qmlRegisterSingletonInstance

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");

    app.setOrganizationDomain("some.io");
    app.setOrganizationName("Andrij Felenko");
    app.setApplicationName("Stock manager");
    app.setWindowIcon(QIcon(":/Stock/ui/images/bull-market.png"));


    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, [](){ QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    qmlREGS <model::SearchTag> ("StockCpp", 1, 0, "SearchTagCpp", model::SearchTag::instance());

    qmlREGS <data::Market>    ("StockCpp", 1, 0, "Market",       data::Market   ::instance());
    qmlREGS <data::Portfolio> ("StockCpp", 1, 0, "PortfolioCpp", data::Portfolio::instance());

    qmlREGS <api::OpenAI>      ("StockCpp", 1, 0, "OpenAI",       api::OpenAI      ::instance());
    qmlREGS <api::Eod>         ("StockCpp", 1, 0, "EOD",          api::Eod         ::instance());
    qmlREGS <api::FinnHub>     ("StockCpp", 1, 0, "FinnHub",      api::FinnHub     ::instance());
    qmlREGS <api::TwelveData>  ("StockCpp", 1, 0, "TwelveData",   api::TwelveData  ::instance());
    qmlREGS <api::MarketStack> ("StockCpp", 1, 0, "MarketStack",  api::MarketStack ::instance());
    qmlREGS <api::AlphaVantage>("StockCpp", 1, 0, "AlphaVantage", api::AlphaVantage::instance());

    qmlREGS <settings::Network>("StockCpp", 1, 0, "SeetingsNetwork", settings::network());

    // util::ResDir::list_qrc_files();
    engine.load("qrc:/Stock/main.qml");

    // OpenAiApiClient client;
    // client.send_chat_request("Який найкращий спосіб аналізувати акції "
                             // "стабільних компаній?", "gpt-4.1-nano", false, 120);

    // EodApiClient eod;
    // eod.fetch_ticker_data("PG.US");

    // api::FinnHub finnhub;
    // finnhub.company_profile("C");
    // finnhub.company_quote("C");

    // api::StringMap params;
    // params["from"] = "2024-10-10";
    // params["to"] = "2024-10-12";
    // params["resolution"] = "30";
    // // finnhub.request(api::Request::Candle, "C.US", params);

    // QDate from = QDate(2015, 6, 8);
    // QDate to = QDate(2025, 06, 8);

    // api::StringMap params2 = {
    //     { "interval", "1day" },
    //     { "start_date", from.toString("yyyy-MM-dd") },
    //     { "end_date", to.toString("yyyy-MM-dd") }
    // };

    // api::TwelveData twelve;
    // twelve.request(api::Request::Candle, "FMC.US", params2);

    // api::AlphaVantage::
    // api::AlphaVantage::daily_candle_by_tag("POLN.EU");
    // api::AlphaVantage::update_info_by_tag("O");
    // api::AlphaVantage::daily_candle_by_tag("MBG.DEX");
    // api::AlphaVantage::today_candle_by_tag("MC");

    // api::OpenAI::instance()->recheck_tag("Vallourec");

    // api::Eod::get_all_exchange_tag();
    // model::SearchTag::instance();
    // api::MarketStack::instance()->update_quotes_by_tag({ "RUI.PA", "0JOP.L", "0NBX.L",
    //                                                      "COLO-B.CO", "WDS", "VOW3.DE",
    //                                                      "TITC.SA", "ULVR.L", "NHY.OL" });

    return app.exec();
}
