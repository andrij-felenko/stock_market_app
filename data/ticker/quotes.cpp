#include "quotes.h"

using namespace data::ticker;

data::ticker::Quotes::Quotes(QObject* parent) : QObject(parent)
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

void Quotes::set_data(QDate d, float open, float close, float high, float low, uint64_t v)
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

namespace data::ticker {
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
