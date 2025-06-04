#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "utilities/res_dir.h"
#include "api/openai.h"
#include "api/eod.h"
#include "api/finnhub.h"

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
    finnhub.request(api::Request::Info, "C.US");

    return app.exec();
}
