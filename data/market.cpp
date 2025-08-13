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
#include <QMap>

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

data::Market::Market(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    _instruments.reserve( 60'000);
    _ticker_meta.reserve(100'000);
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
            if (sym->symbol().code().startsWith(str, Qt::CaseInsensitive) ||
                sym->symbol().code().contains  (str, Qt::CaseInsensitive)){
                sub_add(it);
                break;
            }
        }

        if (ret.size() >= max)
            break;
    }

    return ret;
}

std::optional <data::Ticker*> data::Market::find(data::ticker::Symbol tag)
{
    // qDebug() << Q_FUNC_INFO << tag;
    for (auto& it : _instruments){
        auto ret = (*it)[tag];
        if (ret != nullptr)
            return ret;
    }
    return std::nullopt;
}

data::Instrument* const data::Market::ensure(data::ticker::Symbol tag)
{
    // qDebug() << Q_FUNC_INFO << tag;
    Instrument* instr = nullptr;

    auto ticker = find(tag);
    if (ticker.has_value())
        instr = ticker.value()->instrument();
    else {
        instr = new Instrument(tag, this);
        _instruments.push_back(instr);
    }

    return instr;
}

data::Instrument* const data::Market::ensure(meta::Ticker meta)
{
    // qDebug() << Q_FUNC_INFO;
    Instrument* instr = nullptr;

    auto ticker = find(meta.symbol);
    if (ticker.has_value())
        instr = ticker.value()->instrument();
    else {
        instr = new Instrument(meta, this);
        _instruments.push_back(instr);
    }

    return instr;
}

void data::Market::add_meta(meta::Ticker meta)
{
    qDebug() << Q_FUNC_INFO << meta.symbol;
    for (auto& it : _ticker_meta){
        if (it.symbol == meta.symbol){
            it = meta;
            return;
        }
    }

    _ticker_meta.push_back(meta);
    // TODO add an instrument and meta to global list
}
// ================================================================================================


// --------------------------- LOAD & SAVE DATA ---------------------------------------------------
bool data::Market::empty() const { return _instruments.empty(); }

void data::Market::add_instrument_list_from_meta(QByteArrayList list, bool force)
{
    for (const auto& it : list){
        meta::Ticker meta(it);
        if (force)
            _instruments.push_back(new Instrument(meta, this));
        else
            ensure(meta);
    }
}

void data::Market::load_instruments()
{
    qDebug() << Q_FUNC_INFO;
    std::function load_from = [this](QString path){
        path += "/tickers_meta/";
        QFile file(path + "sorted.edsm");
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        bool same_thread = QThread::currentThread() == qApp->thread();
        int32_t size; in >> size;
        int32_t block_size = 20;
        QByteArrayList datalist;

        for (int i = 0, block = 0; i < size; i++){
            QByteArray data;
            in >> data;

            if (same_thread)
                add_instrument_list_from_meta({ data }, true);
            else {
                block++;
                datalist.push_back(data);

                if (block == block_size || i == size - 1){
                    QMetaObject::invokeMethod(this, [datalist, this]()
                    { add_instrument_list_from_meta(datalist, true); }, Qt::QueuedConnection);
                }

                datalist.clear();
                block = 0;
            }
        }

        file.close();
    };
    // load_from(":/rc");
    load_from(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    emit tickerMetaLoadFinish();
}

void data::Market::save_instruments()
{
    qDebug() << Q_FUNC_INFO;
    std::function save_to = [this](QString path){
        path += "/tickers_meta/";
        QDir().mkpath(path);

        QFile file(path + "sorted.edsm");
        if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
            qDebug() << "can`t open" << file.errorString();
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);

        out << int32_t(_instruments.size());
        int32_t index = 0;
        for (const auto& it : std::as_const(_instruments)){
            if (index % _instruments.size() / 100 == 0){
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents |
                                                QEventLoop::ExcludeSocketNotifiers |
                                                QEventLoop::X11ExcludeTimers, 25);
            }
            QCoreApplication::processEvents();
            meta::Ticker meta = *it;
            out << meta.data();
        }

        file.close();
    };
    // load_from(":/rc");
    save_to(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}
