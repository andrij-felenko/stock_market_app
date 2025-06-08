#include "market.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "api/finnhub.h"
#include <QTimer>

enum MarketRoles {
    TickerRole = Qt::UserRole + 1,
    TitleRole,
    CountryRole,
    IndustryRole,
    QuoteRole,
    LogoRole,
};

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
        if (t->identity()->ticker().toUpper() == tag.toUpper()){
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

    market->beginInsertRows(QModelIndex(), market->rowCount(), market->rowCount());
    Ticker* ticker = new Ticker(market);
    ticker->identity()->set_ticker(tag);
    market->_tickers.emplace_back(ticker);
    market->endInsertRows();

    connect(ticker, &Ticker::update_data, market, [=]() {
        int row = std::distance(market->_tickers.begin(),
                                std::find(market->_tickers.begin(),
                                          market->_tickers.end(), ticker));
        if (row >= 0 && row < market->_tickers.size()) {
            QModelIndex index = market->index(row);
            emit market->dataChanged(index, index, {QuoteRole});
        }
    });

}

data::Market::Market(QObject* parent) : QAbstractListModel(parent)
{
    _tickers.reserve(2000);
    // load_from_local_data();
    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, this, [this]() {
        int max = 5;
        Market* market = Market::instance();
        for (int i = 0; i < market->_tickers.size(); i++){
            if (market->_tickers[i]->identity()->title().isEmpty()){
                api::FinnHub::update_info_by_tag(market->_tickers[i]->identity()->ticker());
                max--;

                if (max == 0) return;
            }
        }
    });

    _timer->start(30'000);
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

int data::Market::rowCount(const QModelIndex& parent) const
{
    return _tickers.size();
}

QVariant data::Market::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_tickers.size()))
        return QVariant();

    Ticker* ticker = _tickers[index.row()];

    switch (role) {
        case TickerRole:  return ticker->identity()->ticker();
        case TitleRole:   return ticker->identity()->title();
        case CountryRole: return ticker->identity()->country();
        case IndustryRole:return ticker->identity()->industry();
        case QuoteRole:   return ticker->quotes()->current();
        case LogoRole:    return ticker->identity()->logo();
        default:          return QVariant();
    }
}

QHash<int, QByteArray> data::Market::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TickerRole]  = "ticker";
    roles[TitleRole]   = "title";
    roles[CountryRole] = "country";
    roles[IndustryRole]= "industry";
    roles[QuoteRole]   = "price";
    roles[LogoRole]    = "logo";
    return roles;
}
