#include "market.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
// #include "api/finnhub.h"
// #include "api/alphavantage.h"
// #include "api/marketstack.h"
#include <QTimer>
#include <thread>
#include "data/instrument.h"
#include "api/eod.h"

enum MarketRoles {
    TickerRole = Qt::UserRole + 1,
    TitleRole,
    CountryRole,
    IndustryRole,
    QuoteRole,
    LogoRole,
    YearMin,
    YearMax,
    Currency,
};

// connect(_timer, &QTimer::timeout, this, [this]() {
//         int max = 7;
//         Market* market = Market::instance();
//         for (int i = 0; i < market->_tickers.size(); i++){
//             if (market->_tickers[i]->identity()->title().isEmpty()){
//                 api::FinnHub::update_info_by_tag(market->_tickers[i]->identity()->ticker());
//                 api::AlphaVantage::update_info_by_tag(market->_tickers[i]->identity()->ticker());
//                 max--;

//                 if (max == 0) return;
//             }
//         }
//     });

// int shift = 1;
// for (int i = 0; i < _instance->_tickers.size(); i++){
//     if (_instance->_tickers[i]->identity()->title().isEmpty()){
//         QString str = _instance->_tickers[i]->identity()->ticker();
//         if (str.toUpper().right(3) == ".US"){
//             QTimer::singleShot(shift * 5000, _instance, [str](){
//                 api::FinnHub::update_info_by_tag(str);
//             });
//         }
//         else if (str.toUpper().right(3) == ".DE" || str.toUpper().right(2) == ".L" ||
//                  str.toUpper().right(3) == ".EU" || str.toUpper().right(3) == ".PA" ||
//                  str.toUpper().right(3) == ".BE" || str.toUpper().right(4) == ".DEX")
//             continue;
//         else {
//             QTimer::singleShot(shift * 5000, _instance, [str](){
//                 api::MarketStack::update_info_by_tag(str);
//             });
//         }
//         shift++;
//     }

data::Market* data::Market::instance()
{
    static Market* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Market(qApp);
    }
    return _instance;
}

data::Market::Market(QObject* parent) : QObject(parent)
{
    _instruments.reserve(20'000);
    _ticker_meta.reserve(40'000);

    QTimer::singleShot(0, this, [this](){ this->_load_data(); });
}

void data::Market::_load_data()
{
    load_ticker_meta();
    load_instruments();
}

// --------------------------- Work with basic list of all market ---------------------------------
std::vector <data::Instrument*> data::Market::search_by(QString str) const
{
    static int max = 30;
    std::vector <data::Instrument*> ret;

    std::function sub_add = [this, &ret](data::Instrument* in){
        for (const auto& it : ret)
            if (it == in)
                return;
        ret.emplace_back(in);
    };

    if (str.length() >= 3){
        for (const auto& it : _instruments){
            if (it->identity()->title().contains(str, Qt::CaseInsensitive)){
                if (ret.size() >= max)
                    break;
                sub_add(it);
            }
        }
    }

    for (const auto& it : _instruments){
        if (it->identity()->title().startsWith(str, Qt::CaseInsensitive)){
            sub_add(it);
            continue;
        }

        for (const auto& sym : it->_tickers){
            if (sym->symbol().startsWith(str, Qt::CaseInsensitive) ||
                sym->symbol().contains  (str, Qt::CaseInsensitive)){
                sub_add(it);
                break;
            }
        }

        if (ret.size() >= max)
            break;
    }

    return ret;
}

std::optional <data::Ticker*> data::Market::find(QString tag)
{
    Market* market = Market::instance();
    for (auto& it : market->_instruments){
        auto ret = (*it)[tag];
        if (ret != nullptr)
            return ret;
    }
    return std::nullopt;
}

data::Instrument* const data::Market::add(QString tag)
{
    Market* market = Market::instance();
    Instrument* instr = nullptr;

    auto ticker = Market::find(tag);
    if (ticker.has_value())
        instr = ticker.value()->instrument();
    else {
        instr = new Instrument(market);
        instr->get(tag, true, true);
        instr->_primary_ticker = tag;
        market->_instruments.emplace_back(instr);
        qDebug() << "Market::add new" << tag << market->_instruments.size();
    }

    return instr;
}

void data::Market::add(TickerMeta meta)
{
    Market* market = instance();
    for (auto& it : market->_ticker_meta){
        if (it.symbol == meta.symbol){
            it = meta;
            return;
        }
    }

    market->_ticker_meta.emplace_back(meta);
    // TODO add an instrument and meta to global list
}
// ================================================================================================




// --------------------------- Clusterize meta data to instrument list ----------------------------
namespace market::meta {
    // можна відправити тікер або початок назви компанії як str
    // exch змінна відповідає або за повну назву Біржі (etc NASDAQ) або останні цифри тегу типу US
    std::vector <data::TickerMeta> find(const std::vector <data::TickerMeta>& list,
                                        QString str, QStringList exch = {});

    std::vector <data::TickerMeta> find(const std::vector <data::TickerMeta>& list,
                                        data::TickerMeta m);
}

void data::Market::clusterise_ticker_meta()
{
    std::thread([this](){ clusterise_ticker_meta(_ticker_meta); }).detach();
}

void data::Market::clusterise_ticker_meta(data::TickerMetaList metalist)
{
    QMap <int, int> counts;

    // видалити всі не stock параметри
    std::erase_if(metalist, [](const data::TickerMeta& m) { return m.type != "Common Stock"; });

    // викликати сортування списку що задишився
    std::ranges::sort(metalist, {}, [](const data::TickerMeta& m){ return m.name.length(); });

    // функція видаляє використані елементи зі списку
    std::function erase_list = [&](std::vector <data::TickerMeta> list){
        std::erase_if(metalist, [&](const data::TickerMeta& d) {
            return std::any_of(list.begin(), list.end(),
                               [&](const data::TickerMeta& m) { return m.symbol == d.symbol; });
        });
    };

    // цикл пошуку компаній всередині списку
    while (not metalist.empty()){
        // отримаємо список сопріднених мета данних по першому тікеру
        std::vector <data::TickerMeta> list = market::meta::find(metalist, metalist[0]);
        qDebug() << "clusterise_ticker_meta" << metalist.size() << "--------------------------"
                 << list.size() << metalist[0].name;

        // завжди список має бути хоча б з одного елемента
        // (насправді якщо це правда то це значить find погано працює)
        if (list.empty())
            list = { metalist[0] };

        // деякі символи мають тільки малоліквідні данні, і навпряд чи будуть колись корисні
        // видаляємо їх з списку взагалі і йдемо далі
        if (list.size() == 1 && (list[0].exchange.compare("pink",    Qt::CaseInsensitive) == 0 ||
                                 list[0].exchange.compare("bats",    Qt::CaseInsensitive) == 0 ||
                                 list[0].exchange.compare("otcgrey", Qt::CaseInsensitive) == 0 ||
                                 list[0].exchange.compare("nmfqs",   Qt::CaseInsensitive) == 0)){
            erase_list(list);
            continue;
        }

        // пошук головного символа серед всіх
        // ADR - позначка що тікер на біржі США але походження не з США
        // PINK - походження не США, але і офіційно немає на біржах США
        // LSE біржа має першу літеру число, значить біржа не рідна для біржі
        QString main_exchange("");
        if (list.size() == 1)
            main_exchange = list[0].symbol;

        // спочатку перевіряємо США, але тікер не має бути ADR або PINK
        {
            TickerMetaList sorted = market::meta::find(list, "", { "US" });
            bool adr = false;
            for (const auto& it : sorted)
                if (it.name.endsWith(" adr",  Qt::CaseInsensitive) ||
                    it.name.contains(" adr ", Qt::CaseInsensitive)){
                    adr = true;
                    break;
                }
            if (not adr){
                for (const auto& it : sorted){
                    if (it.exchange.compare("nyse", Qt::CaseInsensitive) == 0 ||
                        it.exchange.compare("nasdaq", Qt::CaseInsensitive) == 0){
                        if (main_exchange.isEmpty())
                            main_exchange = it.symbol;
                        else { // we have two symbol in one company, weird
                            main_exchange.clear();
                            break;
                        }
                    }
                }
            }
        }

        // якщо немає США то перевіряємо біржу LSE
        if (main_exchange.isEmpty()){
            TickerMetaList sorted = market::meta::find(list, "", { "LSE", "L" });
            for (const auto& it : sorted){
                if (it.symbol   .isEmpty()) continue;
                if (it.symbol[0].isDigit()) continue;

                if (main_exchange.isEmpty())
                    main_exchange = it.symbol;
                else { // we have two symbol in one company, weird
                    main_exchange.clear();
                    break;
                }
            }
        }

        std::function find_longer_name = [](const TickerMetaList& list) -> TickerMeta {
            TickerMeta ret;
            for (const auto& it : list){
                if (it.name.length() > ret.name.length())
                    ret = it;
            }
            return ret;
        };

        // після США та Лондону надаємо перевавгу біржам локальним
        // немає сенсу великим компаніям там бути якщо це не їх батьківщина
        if (main_exchange.isEmpty()){
            TickerMetaList sorted = market::meta::find(list, "", { "MC", "FI", "CO", "TW"
                                                                   "OL", "ST", "SW", "AU" });
            if (sorted.size() >= 1)
                main_exchange = find_longer_name(sorted).symbol;
        }

        // пріоритет великих національним біржам далі типу DE PA BE SA
        if (main_exchange.isEmpty()){
            for (const auto& exch : { "SA", "TO", "PA", "AM", "BE", "DE" }){
                TickerMetaList sorted = market::meta::find(list, "", { exch });
                if (sorted.size() >= 1){
                    main_exchange = find_longer_name(sorted).symbol;
                    break;
                }
            }
        }

        // далі неважливо, ставимо де найкоротший домен біржі і за алфівітом
        if (main_exchange.isEmpty()){
            main_exchange = std::min_element(list.begin(), list.end(),
                                             [](const auto& a, const auto& b){
                if (a.exchange.length() != b.exchange.length())
                    return a.exchange.length() < b.exchange.length();
                return a.exchange.toLower() < b.exchange.toLower();
            })->exchange;
        }


        // видаляємо знайдені символи з загального мета списку
        erase_list(list);

        // видаляємо некоректні пункти, поки тільки PINK
        std::erase_if(list, [](const TickerMeta& m) {
            return m.exchange.compare("PINK", Qt::CaseInsensitive) == 0;
        });


        // створюємо інтрументи з набору який ми знайшли, або доповнили новими символами
        QMetaObject::invokeMethod(Market::instance(), [main_exchange, list]()
        {
            Instrument* const in = Market::add(main_exchange);
            for (const auto& it : list){
                bool main_exch = main_exchange.compare(it.symbol, Qt::CaseInsensitive) == 0;
                auto ticker = in->get(it.symbol, true, main_exch);
                if (main_exch){
                    in->identity()->set_country(it.region);
                    in->identity()->set_title  (it.name);
                }
                else
                    ticker->set_country(it.region);

                ticker->set_currency(currency::Name::from_short(it.currency));
                ticker->set_exchange(it.exchange);
            }
        }, Qt::QueuedConnection);

        // заповнюємо список розміру компаній по символам
        counts[list.size()] = counts.value(list.size(), 0) + 1;
        for (const auto& it : list)
            qDebug() << it.name << it.symbol << it.currency << it.exchange << it.region << it.type;
        qDebug() << "end -----" << main_exchange << "----------------------------------------------";
    }
    qDebug() << "COMMON STOCK Counts";
    for (auto it = counts.begin(); it != counts.end(); it++)
        qDebug() << it.key() << it.value();
    QMetaObject::invokeMethod(Market::instance(), [](){
        Market::instance()->save_instruments(); }, Qt::QueuedConnection);
}

std::vector <data::TickerMeta> market::meta::find(const std::vector <data::TickerMeta>& list,
                                                  QString str, QStringList exch)
{
    std::vector <data::TickerMeta> ret;
    ret.reserve(20);

    std::function try_add = [&ret](data::TickerMeta m){
        for (const auto& it : ret)
            if (it.name == m.name && it.symbol == m.symbol)
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
        if (same_len){
            if (it.name.startsWith(str, Qt::CaseInsensitive)){
                try_add(it);
                continue;
            }
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

std::vector <data::TickerMeta> market::meta::find(const std::vector <data::TickerMeta>& list,
                                                  data::TickerMeta m)
{
    std::vector <data::TickerMeta> ret;
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

        if (current.startsWith("Renault")){
            qDebug() << "Renault";
        }

        // шукаємо можливі варіанти, краще зробити запит тут на все,
        // а вже потім зменшувати поки не повернемо результат готовий
        ret = find(list, current);

        // фільтр назв по NYSE NASDAQ
        auto us = find(ret, "", { "US" });
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
// ================================================================================================


// --------------------------- LOAD & SAVE DATA ---------------------------------------------------
void data::Market::load_instruments()
{
    std::function load_from = [this](QString path){
        path += "/tickers_meta/";
        QFile file(path + "sorted.edsm");
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        int32_t size;
        in >> size;
        for (int i = 0; i < size; i++){
            QString read;
            in >> read; // primary_ticker
            Instrument* instr = Market::add(read);
            in >> read; instr->identity()->set_country(read);
            in >> read; instr->identity()->set_title  (read);
            int32_t symbol_size; in >> symbol_size;
            for (int si = 0; si < symbol_size; si++){
                currency::Tag tag;
                in >> read >> tag; // symbol & currency
                Ticker* t = instr->get(read, true, false);
                t->set_currency(tag);
                in >> read; t->set_country (read);
                in >> read; t->set_exchange(read);
            }
        }

        file.close();
    };
    load_from(":/rc");
    load_from(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}

void data::Market::save_instruments()
{
    qDebug() << "void data::Market::save_instruments()" << _instruments.size();
    std::function load_from = [this](QString path){
        path += "/tickers_meta/";
        QDir().mkpath(path);

        QFile file(path + "sorted.edsm");
        qDebug() << "file" << file.fileName();
        if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
            qDebug() << "can`t open" << file.errorString();
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);

        out << int32_t(_instruments.size());
        qDebug() << "save instrument size" << int32_t(_instruments.size());
        for (const auto& it : std::as_const(_instruments)){
            out << it->_primary_ticker;
            out << it->identity()->country();
            out << it->identity()->title();
            qDebug() << "save_instr"
                     << it->_primary_ticker
                     << it->identity()->country()
                     << it->identity()->title();

            QStringList symbols = it->tickers();
            out << int32_t(symbols.length());
            for (const QString& s : std::as_const(symbols)){
                auto ticker = (*it)[s];
                qDebug() << ticker->symbol()  << ticker->currency_str()
                         << ticker->country() << ticker->exchange();
                out << ticker->symbol()  << ticker->currency()
                    << ticker->country() << ticker->exchange();
            }
        }

        file.close();
    };
    // load_from(":/rc");
    load_from(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
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

void data::Market::load_ticker_meta()
{
    qDebug() << "void data::Market::load_ticker_meta()";

    std::function load_as = [this](QString title, QString type, QString path){
        QFile file(path + "/tickers_meta/" + title + ".edsm");
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);
        int32_t size = 0;
        in >> size;
        _ticker_meta.reserve(_ticker_meta.size() + size);
        if (size > 0){
            for (int i = 0; i < size; i++){
                TickerMeta m;
                in >> m.currency >> m.exchange >> m.name >> m.region >> m.symbol;
                m.name = m.name.trimmed();
                m.name.replace("Corp.", "Corporation");
                m.type = type;

                // try to find if it already exist, if yes, just update
                bool found = false;
                for (auto& it : _ticker_meta){
                    if (it.symbol == m.symbol){
                        found = true;
                        it.currency = m.currency;
                        it.exchange = m.exchange;
                        it.region = m.region;
                        it.type = m.type;
                        it.name = m.name;
                        break;
                    }
                }
                if (not found)
                    _ticker_meta.emplace_back(m);
            }
        }
        file.close();
        qDebug() << "SearchTag load" << type << size << _ticker_meta.size();
    };

    for (auto it = edsm_map.begin(); it != edsm_map.end(); ++it){
        load_as(it.key(), it.value(), ":/rc");
        load_as(it.key(), it.value(),
                QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }

    qDebug() << "SEARCH TAGE ALL DATA SIZE" << _ticker_meta.size();

    if (_ticker_meta.empty())
        QMetaObject::invokeMethod(this, [](){ api::Eod::get_all_exchange_tag(); },
                                  Qt::QueuedConnection);
    else if (_instruments.empty())
        clusterise_ticker_meta();
}

void data::Market::save_ticker_meta()
{
    qDebug() << "SAVE TICKER META" << _instruments.size() << _ticker_meta.size();
    std::function save_as = [this](QString title, QString type){
        QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        basePath += "/tickers_meta/";
        QDir().mkpath(basePath);
        QString filename = basePath + title + ".edsm";

        QFile file(filename);
        if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
            qDebug() << "can`t open";
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);
        int32_t size = 0;
        for (const auto& it : std::as_const(_ticker_meta))
            if (it.type == type)
                size++;

        out << size;
        for (const auto& it : std::as_const(_ticker_meta))
            if (it.type == type)
                out << it.currency << it.exchange << it.name << it.region << it.symbol;

        file.close();
        qDebug() << "Save to: " << filename << size;
    };

    for (const auto& it : std::as_const(_ticker_meta))
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
// ================================================================================================
