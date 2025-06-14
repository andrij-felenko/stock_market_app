#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "utilities/res_dir.h"
#include "api/openai.h"
#include "api/eod.h"
#include "api/finnhub.h"
#include "api/twelvedata.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    app.setOrganizationDomain("some.io");
    app.setOrganizationName("Andrij Felenko");
    app.setApplicationName("Stock manager");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, [](){ QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    // util::ResDir::list_qrc_files();
    // engine.load("qrc:/Stock/main.qml");

    OpenAiApiClient client;
    // client.send_chat_request("Який найкращий спосіб аналізувати акції "
                             // "стабільних компаній?", "gpt-4.1-nano", false, 120);

    EodApiClient eod;
    // eod.fetch_ticker_data("PG.US");

    api::FinnHub finnhub;
    // finnhub.company_profile("C");
    // finnhub.company_quote("C");

    api::StringMap params;
    params["from"] = "2024-10-10";
    params["to"] = "2024-10-12";
    params["resolution"] = "30";
    // finnhub.request(api::Request::Candle, "C.US", params);

    QDate from = QDate(2022, 10, 10);
    QDate to = QDate(2024, 10, 15);

    api::StringMap params2 = {
        { "interval", "1day" },
        { "start_date", from.toString("yyyy-MM-dd") },
        { "end_date", to.toString("yyyy-MM-dd") }
    };

    auto twelve = new api::TwelveData;
    twelve->request(api::Request::Candle, "OLED.US", params2);

    return app.exec();
}
