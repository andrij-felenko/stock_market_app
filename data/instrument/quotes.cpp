#include "quotes.h"

using namespace data;

data::Quotes::Quotes(QObject* parent) : QObject(parent)
{
    //
}

const QVector<QuotesDate>& Quotes::raw_points() const
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

    _points.emplaceBack(p);
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

    _intraday.emplaceBack(p);
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
    if (! _intraday.isEmpty() && _last_intraday > date){
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

bool Quotes::empty() const
{
    return _points.empty();
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Quotes& q) {
        s << quint32(q._points.size());
        for (const auto& it : q._points)
            s << it.date << it.open << it.close << it.high << it.low << it.volume;

        return s;
    }

    QDataStream& operator >> (QDataStream& s, Quotes& q) {
        quint32 size = 0;
        s >> size;

        q._points.clear();
        q._points.reserve(size);
        for (quint32 i = 0; i < size; ++i) {
            QuotesDate qd;
            s >> qd.date >> qd.open >> qd.close >> qd.high >> qd.low >> qd.volume;
            q._points.emplaceBack(qd);
        }

        q.recalculate();
        return s;
    }
}
