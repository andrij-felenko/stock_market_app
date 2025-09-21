#include "core/security/instrument.h"
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

sdk::Instrument::Instrument(uint16_t index) : sdk::Instrument(Isin(), index)
{
    //
}

sdk::Instrument::Instrument(Instrument&& o) noexcept
    : _index(o._index),
      _isin(std::move(o._isin)),
      _name(std::move(o._name)),
      _type(o._type)
{
    _data = o._data;
    _usages.store(o._usages.load(std::memory_order_acquire), std::memory_order_release);
    o._data = nullptr;
}

sdk::Instrument::Instrument(const Isin& isin, uint16_t index) : _isin(isin), _index(index)
{
    _tickers = new std::vector <sdk::Symbol>();
}

sdk::Data* const sdk::Instrument::data() const { return _data; }

// --------------------------- Atomic section -----------------------------------------------------
sdk::Instrument& sdk::Instrument::operator ++() noexcept
{
    if (_usages.fetch_add(2, std::memory_order_acq_rel) == 0)
        create();

    return *this;
}

sdk::Instrument& sdk::Instrument::operator --() noexcept
{
    if ((_usages.fetch_sub(2, std::memory_order_acq_rel) & ~1) == 2)
        unload();

    return *this;
}

bool sdk::Instrument::has_data() const
{ return (_usages.load(std::memory_order_acquire) & 1) != 0; }

sdk::Data* sdk::Instrument::create()
{
    if (not has_data()){
        delete _tickers;
        _data = new Data(_index);
        _usages.fetch_or(0b1, std::memory_order_acq_rel);
    }
    return _data;
}

void sdk::Instrument::unload()
{
    if (has_data()){
        sdk::Data* p = _data;
        _tickers = new std::vector <sdk::Symbol>(p->tickersSymbolList());
        _usages.fetch_and(0b1111'1110, std::memory_order_acq_rel);
        delete p;
    }
}

void sdk::Instrument::release()
{
    if (auto n = _usages.load(std::memory_order_acquire); n == 1)
        unload();
}
// ================================================================================================


sdk::Country sdk::Instrument::country() const { return _isin.country(); }

std::vector<sdk::Symbol> sdk::Instrument::tickers() const
{
    if (has_data())
        return _data->tickersSymbolList();
    return *_tickers;
}

bool sdk::Instrument::contains(const sdk::Symbol& symbol) const
{
    for (const auto& it : tickers())
        if (symbol == it)
            return true;
    return false;
}

void sdk::Instrument::_sortTickers()
{
    auto weight = [&](sdk::Exchange ex) {
        if (sdk::exchange::otc(ex))       return 20;
        if (sdk::exchange::trash(ex))     return 0;   // найнижчий пріоритет
        if (sdk::exchange::nyse(ex))      return 100; // найважливіше
        if (sdk::exchange::nasdaq(ex))    return 90;
        if (sdk::exchange::us(ex))        return 80;
        if (sdk::exchange::euromajor(ex)) return 70;
        if (sdk::exchange::eurominor(ex)) return 60;
        if (sdk::exchange::asia(ex))      return 50;
        if (sdk::exchange::world(ex))     return 40;
        return 10; // дефолт
    };

    // std::ranges::sort(_tickers, [&](Ticker* a, Ticker* b){
    //     sdk::Country c_a = sdk::exchange::country(a->symbol().exchange());
    //     sdk::Country c_b = sdk::exchange::country(b->symbol().exchange());

    //     if (c_a == _meta->_isin_country && c_b != _meta->_isin_country)
    //         return true;
    //     if (c_b == _meta->_isin_country && c_a != _meta->_isin_country)
    //         return false;

    //     return weight(a->symbol().exchange()) > weight(b->symbol().exchange());
    // });
}

sdk::Ticker* sdk::Instrument::_addTicker(const Symbol& symbol)
{
    if (has_data())
        return &_data->addTicker(symbol);

    _tickers->push_back(symbol);
    return nullptr;
}

void sdk::Instrument::findBetterName(const QString& str)
{
    // 0) попередня обробка
    QString a = _name;
    QString b = str.trimmed();

    // якщо один порожній — повертаємо інший
    if (b.isEmpty()) return;
    if (a.isEmpty()){
        _name = b;
        return;
    }

    // 1) якщо рядки рівні з урахуванням регістру — повертаємо будь-який (наприклад, a)
    if (a == b) return;

    // 2) якщо рядки рівні без урахування регістру — обираємо «найкрасивішу» форму з двох
    auto score = [](const QString& s) {
        int badCaps = 0;
        bool newWord = true;
        for (QChar ch : s) {
            if (ch.isSpace() || ch=='(' || ch==')' || ch=='-' || ch=='/' || ch=='.' || ch=='&') {
                newWord = true;
            } else {
                if (ch.isUpper() && !newWord) ++badCaps; // велика літера не на початку слова
                newWord = false;
            }
        }
        return badCaps;
    };

    auto betterByBeauty = [&](const QString& x, const QString& y) {
        const int sx = score(x), sy = score(y);
        if (sx != sy) return sx < sy;                 // менше «поганих» капсів
        if (x.size() != y.size()) return x.size() < y.size(); // коротший
        return x < y;                                  // стабільний tie-breaker
    };

    // 3) якщо різні навіть без урахування регістру — немає «частотного» переможця,
    //    просто обираємо за метрикою краси (щоб уникнути зайвої складності з лічильниками)
    if (not betterByBeauty(a, b))
        _name = b;
}


bool sdk::Instrument::save() const
{
    if (locker)
        return false;

    qDebug() << Q_FUNC_INFO;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/stocks";
    QDir().mkpath(path);

    QFile file(path + "/" + _isin.full() + ".tdsm");
    qDebug() << "file" << file.fileName();
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qDebug() << "data::Instrument::save() can`t open file"
                 << file.fileName() << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);
    out << *this;
    file.close();
    return true;
}

bool sdk::Instrument::load()
{
    locker = true;
    std::function load_data = [this](QString path) -> bool{
        QFile file(path + "/stocks/" + _isin.full() + ".tdsm");
        if (!file.open(QIODevice::ReadOnly))
            return false;

        // TODO перевірити, чи при завантаженні тікера як масив ми не затерли новододаний
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_10);
        in >> *this;
        file.close();
        return true;
    };

    bool result = load_data(":/rc");
    if (not load_data(":/rc"))
        result = load_data(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    locker = false;
    return result;
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Instrument& d){
        s << d._isin << d._name << d._type;
        sdk::list_to_stream(s, d.tickers());
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Instrument& d){
        s >> d._isin >> d._name >> d._type;
        std::vector <sdk::Symbol> list;
        sdk::list_from_stream(s, list);
        if (not d.has_data())
            *d._tickers = list;
        return s;
    }
}

