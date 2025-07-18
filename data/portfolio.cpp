#include "portfolio.h"
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

data::Portfolio* data::Portfolio::instance()
{
    static Portfolio* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Portfolio(qApp);
        qDebug() << "df";
        _instance->load();
        qDebug() << "df2";
    }
    return _instance;
}

data::Portfolio::Portfolio(QObject* parent) : QAbstractListModel(parent)
{
    _stocks.clear();
    _stocks.reserve(2000);
}

// pdsm - portfolio data stock manager
void data::Portfolio::save() const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(basePath);
    QString filename = basePath + "/guest" + ".pdsm";

    QFile file(filename);
    qDebug() << "file" << file.fileName();
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);
    out << int32_t(_stocks.size());
    for (const auto& it : _stocks)
        out << *it;

    file.close();
    qDebug() << "Save to: " << filename;
}

void data::Portfolio::load()
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filename = basePath + "/guest" + ".pdsm";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);
    int32_t size = 0;
    in >> size;
    if (size > 0){
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + size - 1);
        for (int i = 0; i < size; i++){
            Stock* s = new Stock(this);
            in >> *s;
        }
        endInsertRows();
    }
    file.close();
}

int data::Portfolio::rowCount(const QModelIndex& parent) const
{
    return _stocks.size();
}

QVariant data::Portfolio::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_stocks.size()))
        return QVariant();

    Stock* stock = _stocks[index.row()];

    switch (role) {
        case TickerRole:  return stock->ticker()->identity()->ticker();
        case TitleRole:   return stock->ticker()->identity()->title();
        case CountryRole: return stock->ticker()->identity()->country();
        case IndustryRole:return stock->ticker()->identity()->industry();
        case QuoteRole:   return stock->ticker()->quotes()->current();
        case LogoRole:    return stock->ticker()->identity()->logo();
        default:          return QVariant();
    }
}

QHash<int, QByteArray> data::Portfolio::roleNames() const
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
