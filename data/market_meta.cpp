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
#include <QThread>
#include "loader.h"

using dtS = data::ticker::Symbol;

// --------------------------- Clusterize meta data to instrument list ----------------------------
namespace market::meta {
    // можна відправити тікер або початок назви компанії як str
    // exch змінна відповідає або за повну назву Біржі (etc NASDAQ) або останні цифри тегу типу US
    TickerMetaList find(const TickerMetaList& list, QString str, ExchangeEnumList exch = {});
    TickerMetaList find(const TickerMetaList& list, ::meta::Ticker m);
}

void data::Market::clusterise_ticker_meta()
{
    QThread* thread = QThread::create([this]() {
        clusterise_ticker_meta(_ticker_meta);
    });
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void data::Market::clusterise_ticker_meta(TickerMetaList metalist)
{
    QMap <int, int> counts;

    // видалити всі не stock параметри
    std::erase_if(metalist, [](const meta::Ticker& m) { return m.type != "Common Stock"; });

    // викликати сортування списку що задишився
    std::ranges::sort(metalist, {}, [](const meta::Ticker& m){ return m.name.length(); });

    // функція видаляє використані елементи зі списку
    std::function erase_list = [&](TickerMetaList list){
        std::erase_if(metalist, [&](const meta::Ticker& d) {
            return std::any_of(list.begin(), list.end(),
                               [&](const meta::Ticker& m) { return m.symbol == d.symbol; });
        });
    };

    // цикл пошуку компаній всередині списку
    while (not metalist.empty()){
        // отримаємо список сопріднених мета данних по першому тікеру
        TickerMetaList list =  market::meta::find(metalist, metalist[0]);
        qDebug() << "clusterise_ticker_meta" << metalist.size() << "--------------------------"
                 << list.size() << metalist[0].name;
        for (const auto& it : list)
            qDebug() << it.name << it.symbol << it.currency << it.region << it.type;

        // завжди список має бути хоча б з одного елемента
        // (насправді якщо це правда то це значить find погано працює)
        if (list.empty())
            list = { metalist[0] };

        // деякі символи мають тільки малоліквідні данні, і навпряд чи будуть колись корисні
        // видаляємо їх з списку взагалі і йдемо далі
        if (list.size() == 1 && list[0].symbol.trash()){
            erase_list(list);
            continue;
        }

        // пошук головного символа серед всіх
        // ADR - позначка що тікер на біржі США але походження не з США
        // PINK - походження не США, але і офіційно немає на біржах США
        // LSE біржа має першу літеру число, значить біржа не рідна для біржі
        ticker::Symbol main_symbol("");
        if (list.size() == 1)
            main_symbol = list[0].symbol;

        // спочатку перевіряємо США, але тікер не має бути ADR або PINK
        {
            TickerMetaList sorted = market::meta::find(list, "", dtS::us_sufix());
            bool adr = false;
            for (const auto& it : sorted)
                if (it.name.endsWith(" adr",  Qt::CaseInsensitive) ||
                    it.name.contains(" adr ", Qt::CaseInsensitive)){
                    adr = true;
                    break;
                }
            if (not adr){
                for (const auto& it : sorted){
                    if (it.symbol.nyse() || it.symbol.nasdaq()){
                        if (main_symbol.empty())
                            main_symbol = it.symbol;
                        else { // we have two symbol in one company, weird
                            main_symbol.clear();
                            break;
                        }
                    }
                }
            }
        }

        // якщо немає США то перевіряємо біржу LSE
        if (main_symbol.empty()){
            TickerMetaList sorted = market::meta::find(list, "", { dtS::LSE });
            for (const auto& it : sorted){
                if (it.symbol    .empty()) continue;
                if (it.symbol.lse_outer()) continue;

                if (main_symbol.empty())
                    main_symbol = it.symbol;
                else { // we have two symbol in one company, weird
                    main_symbol.clear();
                    break;
                }
            }
        }

        std::function find_longer_name = [](const TickerMetaList& list) -> meta::Ticker {
            meta::Ticker ret;
            for (const auto& it : list){
                if (it.name.length() > ret.name.length())
                    ret = it;
            }
            return ret;
        };

        // після США та Лондону надаємо перевавгу біржам локальним
        // немає сенсу великим компаніям там бути якщо це не їх батьківщина
        if (main_symbol.empty()){
            TickerMetaList sorted = market::meta::find(list, "", dtS::minor_europe_sufix());
            if (sorted.size() >= 1)
                main_symbol = find_longer_name(sorted).symbol;
        }

        if (main_symbol.empty()){
            TickerMetaList sorted = market::meta::find(list, "", dtS::other_worlds_sufix());
            if (sorted.size() >= 1)
                main_symbol = find_longer_name(sorted).symbol;
        }

        // пріоритет великих національним біржам далі типу DE PA BE SA
        if (main_symbol.empty()){
            const ExchangeEnumList& majoreurope = dtS::major_europe_sufix();
            for (const auto& exch : majoreurope){
                TickerMetaList sorted = market::meta::find(list, "", { exch });
                if (sorted.size() >= 1){
                    main_symbol = find_longer_name(sorted).symbol;
                    break;
                }
            }
        }

        // далі неважливо, ставимо де найкоротший домен біржі і за алфівітом
        if (main_symbol.empty()){
            main_symbol = std::min_element(list.begin(), list.end(),
                                           [](const meta::Ticker& a, const meta::Ticker& b){
                // if (a.symbol.exchange() != b.symbol.exchange())
                    return a.symbol.exchange() < b.symbol.exchange();
                // return a.exchange.toLower() < b.exchange.toLower();
            })->symbol;
        }


        // видаляємо знайдені символи з загального мета списку
        erase_list(list);

        // видаляємо некоректні пункти
        std::erase_if(list, [](const meta::Ticker& m) { return m.symbol.trash(); });


        // створюємо інтрументи з набору який ми знайшли, або доповнили новими символами
        if (QThread::currentThread() == qApp->thread())
            add_sorted_instrument(main_symbol, list);
        else
            QMetaObject::invokeMethod(this, [main_symbol, list, this]()
            { this->add_sorted_instrument(main_symbol, list); }, Qt::QueuedConnection);

        // заповнюємо список розміру компаній по символам
        counts[list.size()] = counts.value(list.size(), 0) + 1;
        qDebug() << "end -----" << main_symbol << "----------------------------------------------";
    }
    qDebug() << "COMMON STOCK Counts";
    for (auto it = counts.begin(); it != counts.end(); it++)
        qDebug() << it.key() << it.value();

    if (QThread::currentThread() == qApp->thread())
        save_instruments();
    else
        QMetaObject::invokeMethod(this, &Market::save_instruments, Qt::QueuedConnection);
}

void data::Market::add_sorted_instrument(const ticker::Symbol main, const TickerMetaList list)
{
    Instrument* const in = ensure(main);
    for (const auto& it : list){
        auto ticker = in->ensure(it.symbol);
        if (it.symbol == main){
            in->identity()->set_country(it.region);
            in->identity()->set_title  (it.name);
        }
        else {
            ticker->setCountry(it.region);
        }

        ticker->setCurrency(it.currency_tag());
        ticker->setSymbol(it.symbol);
    }
}

TickerMetaList market::meta::find(const TickerMetaList& list, QString str, ExchangeEnumList exch)
{
    TickerMetaList ret;
    ret.reserve(20);

    std::function try_add = [&ret](::meta::Ticker m){
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
                if (it.symbol.check_exchange(ex)){
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

TickerMetaList market::meta::find(const TickerMetaList& list, ::meta::Ticker m)
{
    TickerMetaList ret;
    ret.reserve(20);

    // шукаємо слово яке є ключовим спочатку ------------------------------------------------------
    int index = 0;
    QStringList names = m.name.split(" ");
    qDebug() << "\n+++" << m.name << names;
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
        auto us = find(ret, "", dtS::us_sufix());
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
                if (it.symbol.code().length() < min.length())
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
    qDebug() << Q_FUNC_INFO;

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
                meta::Ticker m;
                in >> m;
                m.name = m.name.trimmed();
                m.name.replace("Corp.", "Corporation");
                m.type = type;

                // try to find if it already exist, if yes, just update
                bool found = false;
                for (auto& it : _ticker_meta){
                    if (it.symbol == m.symbol){
                        found = true;
                        it = m;
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
}

void data::Market::save_ticker_meta()
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "SAVE TICKER META" << _instruments.size() << _ticker_meta.size();
    if (_ticker_meta.empty())
        return;

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
                out << it;

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
