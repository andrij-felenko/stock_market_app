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
    ExchangeRole,
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
        case ExchangeRole: return _list[index.row()].exchange;
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
    roles[ExchangeRole] = "exchange";
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
    qDebug() << "find by part" << str;
    clear();
    if (str.isEmpty())
        return;

    std::vector <Matches> ret;
    ret.reserve(30);
    static int max = 30;

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

    std::unordered_set<QString> seen;
    std::erase_if(ret, [&](const Matches& m) {
        QString key = m.symbol + "|" + m.name;
        if (seen.contains(key)) {
            return true;  // видаляємо
        }
        seen.insert(key);
        return false;     // залишаємо
    });

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
                m.name = m.name.trimmed();
                m.name.replace("Corp.", "Corporation");
                m.type = type;
                _data.emplace_back(m);
            }
        }
        file.close();
        qDebug() << "SearchTag load" << size << _data.size();
    };

    for (auto it = edsm_map.begin(); it != edsm_map.end(); ++it){
        load_as(it.key(), it.value(), basePath);
        load_as(it.key(), it.value(), ":/rc/tags/");
    }

    qDebug() << "SEARCH TAGE ALL DATA SIZE" << _data.size();
    reorginize();
}

void model::SearchTag::reorginize()
{
    std::vector <Matches> data;
    data.resize(_data.size());

    QMap <int, int> count;

    for (const auto& it : _data)
        if (it.type == "Common Stock")
            data.emplace_back(it);

    // видалити всі не stock параметри
    std::erase_if(data, [](const Matches& m) { return m.type != "Common Stock"; });

    // викликати сортування списку що задишився
    std::ranges::sort(data, {}, [](const Matches& m){ return m.name.length(); });

    for ( const auto& it : data){
        if (it.name.length() > 5)
            break;

        qDebug() << "data" << it.name;
    }

    // в циклі запустити пошук масиву спільного для компаній
    while (not data.empty()){
        // отримаємо список споріднених Matches
        std::vector <Matches> list = _find(data, data[0]);

        if (list.size() == 41)
            qDebug() << "==================================" << data[0].name;

        bool solo = false;
        if (list.size() == 1)
            solo = list[0].exchange == "PINK" || list[0].exchange == "BATS" || list[0].exchange == "OTCGREY" || list[0].exchange == "NMFQS";

        if (not solo)
           for (const auto& it : list)
               qDebug() << "______" << it.symbol << it.name << it.exchange;

        // коли отримаємо результат то створюємо нову компанію і додаємо її на маркет
        // заповнюємо данні але виключаємо некоректні пункти типу PINK
        // видаляємо весь список щоб можна було далі почати цикл
        std::erase_if(data, [&](const Matches& d) {
            return std::any_of(list.begin(), list.end(), [&](const Matches& m) {
                return m.symbol == d.symbol;
            });
        });

        if (not solo){
            qDebug() << data.size() << list.size();
            count[list.size()] = count.value(list.size(), 0) + 1;
        }
        // break;
    }

    qDebug() << "@@@@@@@@@@@@@@@@" << count;
}
// беремо перше слово якщо воно не менше 3 літер


// шукаємо список які так само починаються,
// перевіряємо .US назви, якщо ж є NASDAQ NYSE акції з різними символами,
// це значить що це звичайне слово, тому додаємо ще одне слово пок не знайдемо
// слово вважається знайденим якщо або є однакова назва на двох біржах різних
// або залишилось тільки одне співпадіння
// далі шукаємо весь список який співпадає з цим знайденим словосполученням

// return ret;
/// беремо найменшу назву в списку
/// беремо перше слово якщо воно не менше 3 літер
/// шукаємо список які так само починаються,
/// перевіряємо .US назви, якщо ж є NASDAQ NYSE акції з різними символами,
/// це значить що це звичайне слово, тому додаємо ще одне слово пок не знайдемо
/// слово вважається знайденим якщо або є однакова назва на двох біржах різних
/// або залишилось тільки одне співпадіння
/// далі шукаємо весь список який співпадає з цим знайденим словосполученням
///
/// маємо вектор назв, шукаємо головну біржу
/// якщо є на біржі США, але при цьому це не ADR або PINK
/// тоді це позодження США, якщо ні то шукаємо серед мелких бірж другорядних
/// далі дивимось LSE біржу, якщо назва не з 0 починажться то головна біржа LSE
/// далі вже другорядні а якщо ні то шукаємо PA XETRA, SA, якщо ні то BE
///
/// далі видаляємо PINK, групужмо по валюті і записуємо в масив новий














// можна відправити тікер або початок назви компанії як str
// exch змінна відповідає або за повну назву Біржі (etc NASDAQ) або останні цифри тегу типу US
model::SearchTag::MatchesList
model::SearchTag::_find(const MatchesList& list, QString str, QStringList exch)
{
    std::vector <Matches> ret;
    ret.reserve(20);

    std::function try_add = [&ret](Matches m){
        for (const auto& it : ret)
            if (it.name == m.name || it.symbol == m.symbol)
                return;

        ret.emplace_back(m);
    };

    for (const auto& it : list){
        // якщо список не порожній, тоді не впливає, якщо є то відсіюємо зайве спочатку
        if (not exch.isEmpty()){
            bool found_exch = false;
            for (const auto& ex : std::as_const(exch)){
                if (it.exchange == ex || it.symbol.endsWith(ex)){
                    found_exch = true;
                    break;
                }
            }
            if (not found_exch)
                continue;
        }

        // якщо str пустий, можливо коли ми перевіряємо тільки тікери
        if (str.isEmpty()){
            try_add(it);
            continue;
        }

        bool same_len = it.name.length() == str.length();
        // перевіряємо тепер назву на співпадіння
        if (same_len && it.name.startsWith(str, Qt::CaseInsensitive)){
            try_add(it);
            continue;
        }

        else if (it.name.startsWith(str + " ", Qt::CaseInsensitive)){
            if (it.name.length() )
            try_add(it);
            continue;
        }

        // перевіряємо відмінності окремих літер в кінці назви
        // але ми маємо бути впевнені що це самостійне слово
        // якщо перевіряємо і це має різну довжину то крешу не буде, && відсіє
        bool is_cut_word = not same_len && it.name[str.size()] != ' ';
        if (same_len && not is_cut_word)
            if (it.name.startsWith(str.left(str.length() - 1))){
                try_add(it);
                continue;
            }
    }

    return ret;
}

// @@@@@@@@@@@@@@@@ QMap((1, 21634)(2, 2755)(3, 699)(4, 120)(5, 30)(6, 11)(7,  7)(8,  9)(9, 2)              (13, 1)(14, 2)(15, 1)       (18, 1)       (22, 1)(24, 2)              (29, 1)       (41, 1))
// @@@@@@@@@@@@@@@@ QMap((1, 20227)(2, 3005)(3, 804)(4, 172)(5, 44)(6, 22)(7, 11)(8, 12)(9, 4)(11, 2)(12, 1)(13, 4)(14, 1)(15, 1)(16, 2)       (20, 1)(22, 1)(24, 2)(26, 1)(28, 1)(29, 1)(30, 1)        (42, 1))
// @@@@@@@@@@@@@@@@ QMap((1, 12798)(2, 3005)(3, 804)(4, 172)(5, 44)(6, 22)(7, 11)(8, 12)(9, 4)(11, 2)(12, 1)(13, 4)(14, 1)(15, 1)(16, 2)       (20, 1)(22, 1)(24, 2)(26, 1)(28, 1)(29, 1)(30, 1)        (42, 1))
// @@@@@@@@@@@@@@@@ QMap((1, 13005)(2, 2795)(3, 689)(4, 148)(5, 36)(6, 15)(7, 10)(8, 11)(9, 3)(11, 1)(12, 1)(13, 2)(14, 1)(15, 1)(16, 2)(18, 1)(20, 1)(21, 1)(24, 1)(26, 1)              (30, 2)        (42, 1))
model::SearchTag::MatchesList model::SearchTag::_find(const MatchesList& list, Matches m)
{
    std::vector <Matches> ret;
    ret.reserve(20);

    // шукаємо слово яке є ключовим спочатку ------------------------------------------------------
    int index = 0;
    QStringList names = m.name.split(" ");
    qDebug() << "+++" << m.name << names;
    QString current;
    while (names.length() > index){
        if (not current.isEmpty())
            current += " ";
        current += names[index];
        index++;

        // якщо це перше слово, то беремо тільки якщо воно більше ніж 3 символа, чи разом
        if (current.length() < 5)
            continue;

        // шукаємо можливі варіанти, краще зробити запит тут на все,
        // а вже потім зменшувати поки не повернемо результат готовий
        ret = _find(list, current);

        // фільтр назв по NYSE NASDAQ
        auto us = _find(ret, "", { "US" });
        // маємо одного, отже це акція з США, отже ми знайшли назву точно, або немає на США зовсім
        if (us.size() < 2)
            break;
        else {
            QStringList tickers;
            for (const auto& it : us){
                tickers.push_back(it.symbol);
                tickers.last().chop(3);
            }
            // find min string by size
            QString min = tickers[0];
            for (const auto& it : us)
                if (it.symbol.length() < min.length())
                    min = it.symbol;

            // TODO maybe set min to 1 symbol, not 2 as is now
            // надто коротко щоб взагалі вважати це нормальним для перевірки
            if (min.length() < 3)
                continue;

            bool allfromsame = true;
            for (const auto& it : std::as_const(tickers))
                if (not it.startsWith(min)){
                    allfromsame = false;
                    break;
                }
            if (allfromsame)
                break;
        }
    }


    // знаходимо весь список з похідних і повертаємо список
    qDebug() << current;

    if (ret.empty())
        return { m };

    return ret;
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
