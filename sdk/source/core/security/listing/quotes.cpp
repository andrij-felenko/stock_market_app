#include "core/security/listing/quotes.h"
#include "common/features.h"

sdk::Quotes::Quotes()
{
    //
}

sdk::Quotes& sdk::Quotes::operator =(const Quotes& other)
{
    if (this == &other) return *this;

    _last_intraday = other._last_intraday;
    _intraday      = other._intraday;
    _points        = other._points;

    return *this;
}

const std::vector <sdk::QuotesDate>& sdk::Quotes::raw_points() const
{
    return _points._;
}

void sdk::Quotes::setData(QDate d, float open, float close, float high, float low, quint64  v)
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

    _points->push_back(p);
}

void sdk::Quotes::setData(QTime t, float open, float close, float high, float low, quint64 v)
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

    _intraday->push_back(p);
}

void sdk::Quotes::setIntraday(QDate date)
{
    _intraday->clear();
    _last_intraday = date;
}

float sdk::Quotes::yearMax() const
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

float sdk::Quotes::yearMin() const
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

float sdk::Quotes::current() const
{
    float ret = 0.0;
    QDate date = QDate::fromJulianDay(0);
    for (auto& it : _points)
        if (it.date > date){
            ret = it.close;
            date = it.date;
        }
    if (! _intraday->empty() && _last_intraday > date){
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

bool sdk::Quotes::empty()
{
    return _points->empty();
}

namespace sdk {
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
    QDataStream& operator << (QDataStream& s, Wire <const Quotes> d){
        if (d.data()) s << d->_points << d->_intraday << d->_last_intraday << d->_beta;
        return s << d->_track;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Quotes> d){
        if (d.data()) s >> d->_points >> d->_intraday >> d->_last_intraday >> d->_beta;
        return s >> d->_track;
    }
// --------------------------------------------------------------------------------------
}
