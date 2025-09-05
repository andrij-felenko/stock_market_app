#include "instrument/instrument.h"
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

sdk::Instrument::Instrument(const Isin& isin) : _isin(isin)
{
    //
}

sdk::Data* const sdk::Instrument::data() const { return _data; }
bool sdk::Instrument::loaded() const { return _data != nullptr; }

sdk::Data* sdk::Instrument::create()
{
    if (_data == nullptr)
        _data = new Data();
    return _data;
}

sdk::Country sdk::Instrument::country() const { return _isin.country(); }

void sdk::Instrument::_sort_tickers()
{
    auto weight = [&](sdk::Exchange ex) {
        if (sdk::exchange::otc(ex))      return 20;
        if (sdk::exchange::trash(ex))    return 0;   // найнижчий пріоритет
        if (sdk::exchange::nyse(ex))     return 100; // найважливіше
        if (sdk::exchange::nasdaq(ex))   return 90;
        if (sdk::exchange::us(ex))       return 80;
        if (sdk::exchange::euromajor(ex))return 70;
        if (sdk::exchange::eurominor(ex))return 60;
        if (sdk::exchange::asia(ex))     return 50;
        if (sdk::exchange::world(ex))    return 40;
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

    // // find better title
    // QStringList list = _meta->title().split('|', Qt::SkipEmptyParts);
    // list << _meta->_title;

    // // нормалізація
    // for (QString& s : list) s = s.trimmed();
    // list.removeAll(QString());
    // list.removeDuplicates();

    // if (list.isEmpty())
    //     return;

    // // 1) рахуємо найчастішу форму без урахування регістру
    // QMap<QString, int> counter;
    // for (const auto& s : std::as_const(list)) counter[s.toLower()]++;

    // QString bestLower;
    // int max = 0;
    // for (auto it = counter.cbegin(); it != counter.cend(); ++it) {
    //     if (it.value() > max) { max = it.value(); bestLower = it.key(); }
    // }
    // if (bestLower.isEmpty())
    //     return;

    // // 2) залишаємо лише кандидатів, що відповідають найчастішій формі
    // QStringList candidates;
    // for (const auto& s : std::as_const(list))
    //     if (s.compare(bestLower, Qt::CaseInsensitive) == 0)
    //         candidates << s;

    // if (candidates.isEmpty())
    //     return;

    // // 3) вибираємо «найкрасивішу» форму серед кандидатів
    // auto score = [](const QString& s) {
    //     int badCaps = 0;
    //     bool newWord = true;
    //     for (QChar ch : s) {
    //         if (ch.isSpace() || ch=='(' || ch==')' || ch=='-' || ch=='/' || ch=='.' || ch=='&')
    //             newWord = true;
    //         else {
    //             if (ch.isUpper() && !newWord)
    //                 ++badCaps; // велика літера не на початку слова
    //             newWord = false;
    //         }
    //     }
    //     return badCaps;
    // };

    // const QString& best =
    //     *std::min_element(candidates.begin(), candidates.end(),
    //                       [&](const QString& a, const QString& b) {
    //                           int sa = score(a), sb = score(b);
    //                           if (sa != sb) return sa < sb;       // менше «поганих» капсів
    //                           if (a.size() != b.size()) return a.size() < b.size(); // коротший
    //                           return a < b;                        // стабільний tie-breaker
    //                       });

    // _meta->set_title(best);
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
    QDataStream& operator << (QDataStream& s, const Instrument& d) {
        // s << uint8_t
        // if (d.loaded())
            // s << *d._data;
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Instrument& d) {
        // if (d.loaded())
            // s << *d._data;
        return s;
    }
}

