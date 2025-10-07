#include "service/market.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "core/security/instrument.h"

sdk::Market::Market(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    _instruments.reserve(100'000);
}

sdk::market::Finder sdk::Market::findTicker(const sdk::Symbol& tag)
{
    market::Finder ret(tag);
    for (auto& it : _instruments){
        if (it.has_data()){
            for (auto& t : it.data()->tickers)
                if (t.symbol() == tag){
                    ret.ticker = &t;
                    ret.instrument = &it;
                    return ret;
                }
        }
        else {
            for (const auto& t : it.tickers())
                if (t == tag){
                    ret.instrument = &it;
                    return ret;
                }
        }
    }
    return ret;
}

sdk::Instrument* sdk::Market::findInstrument(const sdk::Isin& isin)
{
    for (auto& it : _instruments)
        if (it._isin == isin)
            return &it;
    return nullptr;
}

sdk::market::Finder sdk::Market::addTicker(const Symbol& tag, const Isin& isin,
                                           const QString& name, Instype type)
{
    market::Finder data = findTicker(tag);
    if (data.exist())
        return data;

    data.instrument = findInstrument(isin);
    if (data.exist()){ // isin founded
        data.instrument->findBetterName(name);
    }
    else { // isin not found, add new instrument
        Instrument in(++_last_index);
        _instruments.push_back(std::move(in));
        data.instrument = &_instruments.back();
        data.instrument->_isin = isin;
        data.instrument->_name = name;
        data.instrument->_type = type;
    }

    // add new ticker
    data.ticker = data.instrument->_addTicker(tag);
    return data;
}

// std::vector <data::Instrument*> data::Market::search_by(QString str) const
// {
//     static int max = 30;
//     std::vector <data::Instrument*> ret;

//     std::function sub_add = [this, &ret](data::Instrument* in){
//         for (const auto& it : ret)
//             if (it == in)
//                 return;
//         ret.push_back(in);
//     };

//     if (str.length() >= 3){
//         for (const auto& it : _instruments){
//             if (it->meta()->title().contains(str, Qt::CaseInsensitive)){
//                 if (ret.size() >= max)
//                     break;
//                 sub_add(it);
//             }
//         }
//     }

//     for (const auto& it : _instruments){
//         if (it->meta()->title().startsWith(str, Qt::CaseInsensitive)){
//             sub_add(it);
//             continue;
//         }

//         for (const auto& sym : it->_tickers){
//             if (sym->symbol().code().startsWith(str, Qt::CaseInsensitive) ||
//                 sym->symbol().code().contains  (str, Qt::CaseInsensitive)){
//                 sub_add(it);
//                 break;
//             }
//         }

//         if (ret.size() >= max)
//             break;
//     }

//     return ret;
// }

void sdk::Market::loadMeta()
{
    qDebug() << Q_FUNC_INFO;
    std::function _load = [this](QString path) -> bool {
        path += "/tickers_meta/";
        QDir().mkpath(path);

        QFile file(path + "meta.edsm");
        qDebug() << Q_FUNC_INFO << file.fileName();
        if (!file.open(QIODevice::ReadOnly)){
            qDebug() << "can`t open" << file.errorString();
            return false;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_10);

        int32_t size = 0; out >> size;
        for (int i = 0; i < size; i++){
            Instrument in(++_last_index); out >> io(in, WireMode::Data);
            _instruments.push_back(std::move(in));
        }

        file.close();
        return true;
    };

    if (not _load(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))){
        if (not _load(":/rc"))
            return;
        saveMeta();
    }
}

void sdk::Market::saveMeta() const
{
    qDebug() << Q_FUNC_INFO;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/tickers_meta/";
    QDir().mkpath(path);

    QFile file(path + "meta.edsm");
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qDebug() << "can`t open" << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    out << int32_t(_instruments.size());
    qDebug() << Q_FUNC_INFO << "WRITE SAVE SIZE" << int32_t(_instruments.size());
    int32_t index = 0;
    for (const auto& it : std::as_const(_instruments))
        out << io(it, WireMode::Data);

    file.close();
}

bool sdk::Market::empty() const { qDebug() << _instruments.size(); return _instruments.empty(); }
