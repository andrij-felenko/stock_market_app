#include "market.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

data::Market* data::Market::instance()
{
    static Market* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Market(qApp);
        _instance->load_from_local_data();
    }
    return _instance;
}

std::optional <data::Ticker*> data::Market::find(QString tag)
{
    qDebug() << "find start";
    for (auto* t : instance()->_tickers)
        if (t->identity()->ticker() == tag){
            qDebug() << "find ret t";
            return t;
        }

    qDebug() << "not found ticket" << tag;
    for (auto* t : instance()->_tickers)
        qDebug() << "...." << t->identity()->ticker();

    qDebug() << "find end";
    return std::nullopt;
}

std::vector <data::Ticker*> data::Market::all()
{
    return instance()->_tickers;
}

void data::Market::add(QString tag)
{    
    qDebug() << "add start";
    Market* market = instance();
    qDebug() << "mm";
    if (market->find(tag).has_value()){
        qDebug() << "add return zero";
        return;
    }

    Ticker* ticker = new Ticker(market);
    ticker->identity()->set_ticker(tag);
    market->_tickers.emplace_back(ticker);
    qDebug() << "add end";
}

data::Market::Market(QObject* parent) : QObject(parent)
{
    _tickers.reserve(2000);
    // load_from_local_data();
}

void data::Market::load_from_local_data()
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(basePath);
    if (!dir.exists())
        return;

    QStringList files = dir.entryList(QStringList() << "*.tdsm", QDir::Files);
    for (const QString& filename : std::as_const(files)) {
        QFile file(dir.filePath(filename));
        if (!file.open(QIODevice::ReadOnly))
            continue;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        Market::add(QFileInfo(filename).completeBaseName());
        if (auto t = Market::find(QFileInfo(filename).completeBaseName()); t.has_value())
            in >> *t.value();

        file.close();
    }
}
