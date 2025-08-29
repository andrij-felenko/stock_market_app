#include "market.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QTimer>
#include "data/instrument.h"
#include <QThread>
#include <QMap>
#include "instrument/meta.h"


data::Market::Market(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    _instruments.reserve(100'000);
}

std::vector <data::Instrument*> data::Market::search_by(QString str) const
{
    static int max = 30;
    std::vector <data::Instrument*> ret;

    std::function sub_add = [this, &ret](data::Instrument* in){
        for (const auto& it : ret)
            if (it == in)
                return;
        ret.push_back(in);
    };

    if (str.length() >= 3){
        for (const auto& it : _instruments){
            if (it->meta()->title().contains(str, Qt::CaseInsensitive)){
                if (ret.size() >= max)
                    break;
                sub_add(it);
            }
        }
    }

    for (const auto& it : _instruments){
        if (it->meta()->title().startsWith(str, Qt::CaseInsensitive)){
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

std::optional <data::Ticker*> data::Market::find(ticker::Symbol tag)
{
    for (auto& it : _instruments){
        auto ret = (*it)[tag];
        if (ret != nullptr)
            return ret;
    }
    return std::nullopt;
}

data::Instrument* const data::Market::ensure(ticker::Symbol tag)
{
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

data::Instrument* const data::Market::ensure(const data::Meta& meta, ticker::Symbol symbol)
{
    if (meta._isin_code.size() != 10)
        return nullptr;

    Instrument* in = nullptr;

    for (const auto& it : _instruments){
        if (it->meta()->isin_country() == meta.isin_country() &&
            it->meta()->isin_code() == meta.isin_code()){
            in = it;
            break;
        }
    }

    if (in == nullptr){
        in = ensure(symbol);
        *in->meta() = meta;
        qDebug() << Q_FUNC_INFO << _instruments.size();
        return in;
    }

    in->ensure(symbol);
    in->meta()->_title.push_back("|" + meta.title());
    qDebug() << Q_FUNC_INFO << _instruments.size();
    return in;
}

void data::Market::detect_main_ticker()
{
    for (const auto& in : _instruments)
        in->_sort_tickers();
}

void data::Market::load_meta()
{
    qDebug() << Q_FUNC_INFO;
    std::function _load = [this](QString path) -> bool {
        path += "/tickers_meta/";
        QDir().mkpath(path);

        QFile file(path + "meta.edsm");
        if (!file.open(QIODevice::ReadOnly)){
            qDebug() << "can`t open" << file.errorString();
            return false;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_10);

        int32_t size = 0; out >> size;
        qDebug() << Q_FUNC_INFO << "SIZE READ INST" << size;

        for (int i = 0; i < size; i++){
            QByteArray data;
            ticker::SymbolList list = data::Meta::load(out, data);
            if (list.empty())
                continue;

            Instrument* in = new Instrument(list[0], this);
            *in->meta() = data;
            for (const auto&t : list)
                in->ensure(t);
            _instruments.push_back(in);
        }

        file.close();
        return true;
    };

    if (not _load(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))){
        if (not _load(":/rc"))
            return;
    }
    emit metaLoaded();
}

void data::Market::save_meta() const
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
        it->meta()->save(out, it->tickers());

    file.close();
}

bool data::Market::empty() const { return _instruments.empty(); }
