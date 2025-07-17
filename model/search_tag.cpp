#include "search_tag.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

enum Tag {
    NameRole = Qt::UserRole + 1,
    TagRole,
    TypeRole,
    RegionRole,
    CurrencyRole,
};

model::SearchTag* model::SearchTag::instance()
{
    static SearchTag* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new SearchTag(qApp);
    }
    return _instance;
}

model::SearchTag::SearchTag(QObject* parent) : QAbstractListModel(parent)
{
    load();
}

int model::SearchTag::rowCount(const QModelIndex& parent) const
{
    return _list.size();
}

QVariant model::SearchTag::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_list.size()))
        return QVariant();

    switch (role) {
        case TagRole:      return _list[index.row()].symbol;
        case NameRole:     return _list[index.row()].name;
        case TypeRole:     return _list[index.row()].type;
        case RegionRole:   return _list[index.row()].region;
        case CurrencyRole: return _list[index.row()].currency;
        default:           return QVariant();
    }
}

QHash<int, QByteArray> model::SearchTag::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles[TagRole]      = "symbol";
    roles[NameRole]     = "title";
    roles[TypeRole]     = "type";
    roles[RegionRole]   = "region";
    roles[CurrencyRole] = "currency";
    return roles;
}

void model::SearchTag::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _list.clear();
    endRemoveRows();
}

void model::SearchTag::add(QString symbol, QString name,
                           QString type, QString region,
                           QString currency)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Matches m;
    m.symbol = symbol;
    m.name = name;
    m.type = type;
    m.region = region;
    m.currency = currency;
    _list.push_back(m);
    endInsertRows();
}

void model::SearchTag::add_data(QString symbol, QString name,
                                QString type, QString region,
                                QString currency, QString exchange)
{
    for (auto& it : _data)
        if (it.symbol == symbol){
            it.name = name;
            it.type = type;
            it.region = region;
            it.currency = currency;
            it.exchange = exchange;
            return;
        }

    Matches m;
    m.symbol = symbol;
    m.name = name;
    m.type = type;
    m.region = region;
    m.currency = currency;
    m.exchange = exchange;
    _data.push_back(m);
}

void model::SearchTag::find_by_part(QString str)
{
    clear();
    if (str.isEmpty())
        return;

    std::vector <Matches> ret;
    ret.reserve(10);
    static int max = 12;

    for (const auto& it : _data){
        if (it.symbol.startsWith(str, Qt::CaseInsensitive) ||
            it.name  .startsWith(str, Qt::CaseInsensitive)){
            ret.emplace_back(it);
            if (ret.size() >= max)
                break;
        }
    }

    for (const auto& it : _data){
        if (it.symbol.contains(str, Qt::CaseInsensitive)){
            ret.emplace_back(it);
            if (ret.size() >= max)
                break;
        }
    }

    if (str.length() >= 3){
        for (const auto& it : _data){
            if (it.name.contains(str, Qt::CaseInsensitive)){
                ret.emplace_back(it);
                if (ret.size() >= max)
                    break;
            }
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount() + ret.size() - 1);
    for (const auto& it : ret)
        _list.push_back(it);
    endInsertRows();
}


QMap <QString, QString> edsm_map = {
    { "common_stock", "Common Stock" },
    { "etf", "ETF" },
    { "fund", "FUND" },
    { "mutual_fund", "Mutual Fund" },
    { "preferred_stock", "Preferred Stock" },
    { "unit", "Unit" },
    { "notes", "Notes" },
    { "bond", "BOND" },
    { "etc", "ETC" },
    { "index", "INDEX" },
};

// edsm - exchange data stock manager
void model::SearchTag::load()
{
    _data.clear();

    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    basePath += "/tags/";

    std::function load_as = [this](QString title, QString type, QString path){
        // QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        // basePath += "/tags/";
        QString filename = path + "/" + title + ".edsm";

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);
        int32_t size = 0;
        in >> size;
        _data.reserve(_data.size() + size);
        if (size > 0){
            for (int i = 0; i < size; i++){
                Matches m;
                in >> m.currency >> m.exchange >> m.name >> m.region >> m.symbol;
                m.type = type;
                _data.emplace_back(m);
            }
        }
        file.close();
        qDebug() << "SearchTag load" << size << _data.size();
    };

    for (auto it = edsm_map.begin(); it != edsm_map.end(); ++it){
        load_as(it.key(), it.value(), basePath);
        load_as(it.key(), it.value(), ":/Stock/rc/tags/");
    }

    qDebug() << "SEARCH TAGE ALL DATA SIZE" << _data.size();
}

void model::SearchTag::save()
{
    std::function save_as = [this](QString title, QString type){
        QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        basePath += "/tags/";
        QDir().mkpath(basePath);
        QString filename = basePath + title + ".edsm";

        QFile file(filename);
        qDebug() << "file" << file.fileName();
        if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
            return;

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);
        uint32_t size = 0;
        for (const auto& it : std::as_const(_data))
            if (it.type == type)
                size++;

        out << size;
        for (const auto& it : std::as_const(_data))
            if (it.type == type)
                out << it.currency << it.exchange << it.name << it.region << it.symbol;

        file.close();
        qDebug() << "Save to: " << filename << size;
    };

    for (const auto& it : std::as_const(_data))
        if (it.type != "Common Stock" && it.type != "ETF" &&
            it.type != "FUND" && it.type != "Mutual Fund" &&
            it.type != "Preferred Stock" && it.type != "Unit" &&
            it.type != "Notes" && it.type != "ETC" &&
            it.type != "BOND" && it.type != "INDEX"){
            qDebug() << it.type << "ffffffffffffffffffffffffffffffffff";
        }

    for (auto it = edsm_map.begin(); it != edsm_map.end(); ++it)
        save_as(it.key(), it.value());
}
