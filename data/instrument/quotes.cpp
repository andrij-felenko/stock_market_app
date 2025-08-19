#include "quotes.h"
#include "utilities/features.h"

using namespace data;

data::Quotes::Quotes(QObject* parent) : QObject(parent)
{
    //
}

data::Quotes& Quotes::operator =(const Quotes& other)
{
    if (this == &other) return *this;

    _last_intraday = other._last_intraday;
    _intraday      = other._intraday;
    _points        = other._points;

    for (QObject* obj : std::as_const(_result))
        obj->deleteLater();
    _result.clear();

    recalculate();
    return *this;
}

const std::vector <QuotesDate>& Quotes::raw_points() const
{
    return _points;
}

QVector<QObject*> Quotes::points()
{
    for (QObject* o : std::as_const(_result))
        o->deleteLater();
    _result.clear();

    for (const QuotesDate& p : std::as_const(_points)) {
        QObject* obj = new QObject(this);
        obj->setProperty("date", p.date);
        obj->setProperty("open", p.open);
        obj->setProperty("close", p.close);
        obj->setProperty("high", p.high);
        obj->setProperty("low", p.low);
        obj->setProperty("volume", p.volume);
        _result.append(obj);
    }
    return _result;
}

void Quotes::recalculate()
{
    emit pointsChanged();
}

void Quotes::set_data(QDate d, float open, float close, float high, float low, quint64  v)
{
    for (auto& p : _points)
        if (d == p.date){
            p.open = open;
            p.close = close;
            p.high = high;
            p.low = low;
            p.volume = v;
            return;
        }

    // add new
    QuotesDate p;
    p.date = d;
    p.open = open;
    p.close = close;
    p.high = high;
    p.low = low;
    p.volume = v;

    _points.push_back(p);
}

void Quotes::set_data(QTime t, float open, float close, float high, float low, quint64 v)
{
    for (auto& p : _intraday)
        if (t == p.time){
            p.open = open;
            p.close = close;
            p.high = high;
            p.low = low;
            p.volume = v;
            return;
        }

    // add new
    QuotesTime p;
    p.time = t;
    p.open = open;
    p.close = close;
    p.high = high;
    p.low = low;
    p.volume = v;

    _intraday.push_back(p);
}

void Quotes::set_intraday(QDate date)
{
    _intraday.clear();
    _last_intraday = date;
}

float Quotes::year_max() const
{
    float max = 0;
    for (const auto& it : _points)
        if (it.date.daysTo(QDate::currentDate()) < 366)
            if (it.high > max)
                max = it.high;

    // if (max == 0)
        // return std::numeric_limits <float>::max();

    return max;
}

float Quotes::year_min() const
{
    float min = std::numeric_limits <float>::max();
    for (const auto& it : _points)
        if (it.date.daysTo(QDate::currentDate()) < 366)
            if (it.low < min)
                min = it.low;

    if (min == std::numeric_limits <float>::max())
        return 0;

    return min;
}

float Quotes::current() const
{
    float ret = 0.0;
    QDate date = QDate::fromJulianDay(0);
    for (auto& it : _points)
        if (it.date > date){
            ret = it.close;
            date = it.date;
        }
    if (! _intraday.empty() && _last_intraday > date){
        QTime time = QTime(0, 0);
        for (auto& it : _intraday){
            if (it.time > time){
                ret = it.close;
                time = it.time;
            }
        }
    }

    return ret;
}

bool Quotes::empty()
{
    return _points.empty();
}

namespace data {
// --------------------------------------------------------------------------------------
    QDataStream& operator << (QDataStream& s, const QuotesDate& q)
    { return s << q.date << q.open << q.close << q.high << q.low << q.volume; }

    QDataStream& operator >> (QDataStream& s,       QuotesDate& q)
    { return s >> q.date >> q.open >> q.close >> q.high >> q.low >> q.volume; }
// --------------------------------------------------------------------------------------
    QDataStream& operator << (QDataStream& s, const QuotesTime& q)
    { return s << q.time << q.open << q.close << q.high << q.low << q.volume; }

    QDataStream& operator >> (QDataStream& s,       QuotesTime& q)
    { return s >> q.time >> q.open >> q.close >> q.high >> q.low >> q.volume; }
// --------------------------------------------------------------------------------------
    QDataStream& operator << (QDataStream& s, const Quotes& q) {
        util::list_to_stream(s, q._points);
        util::list_to_stream(s, q._intraday);
        return s << q._last_intraday;
    }

    QDataStream& operator >> (QDataStream& s, Quotes& q) {
        q._points.clear();
        util::list_from_stream(s, q._points);
        util::list_from_stream(s, q._intraday);
        q.recalculate();
        return s >> q._last_intraday;
    }
// --------------------------------------------------------------------------------------
}
