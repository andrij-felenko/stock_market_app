#include "dividend.h"

using namespace data::ticker;

data::ticker::Dividend::Dividend(QObject* parent) : QObject(parent)
{
    //
}

float Dividend::    yield() const { return _yield; }
void  Dividend::set_yield(float new_yield)
{
    if (qFuzzyCompare(_yield, new_yield))
        return;
    _yield = new_yield;
    emit yieldChanged(_yield);
}

float Dividend::    per_share() const { return _per_share; }
void  Dividend::set_per_share(float new_per_share)
{
    if (qFuzzyCompare(_per_share, new_per_share))
        return;
    _per_share = new_per_share;
    emit per_shareChanged(_per_share);
}

float Dividend::    pay_ratio() const { return _pay_ratio; }
void  Dividend::set_pay_ratio(float new_payout_ratio)
{
    if (qFuzzyCompare(_pay_ratio, new_payout_ratio))
        return;
    _pay_ratio = new_payout_ratio;
    emit pay_ratioChanged(_pay_ratio);
}

QDate Dividend::    next_date() const { return _next_date; }
void  Dividend::set_next_date(const QDate& new_next_date)
{
    if (_next_date == new_next_date)
        return;
    _next_date = new_next_date;
    emit next_dateChanged(_next_date);
}

QDate Dividend::    prev_date() const { return _prev_date; }

void Dividend::set_history(QDate date, float amount)
{
    DividendList dl;
    dl.amount = amount;
    dl.date = date;
    _history.emplace_back(dl);
}
void  Dividend::set_prev_date(const QDate& new_prev_date)
{
    if (_prev_date == new_prev_date)
        return;
    _prev_date = new_prev_date;
    emit prev_dateChanged(_prev_date);
}

namespace data::ticker {
    QDataStream& operator<<(QDataStream& s, const Dividend& d) {
        s << d._yield
          << d._per_share
          << d._pay_ratio
          << d._next_date
          << d._prev_date;

        s << quint32(d._history.size());
        for (const DividendList& dl : d._history)
            s << dl.amount << dl.date;

        return s;
    }

    QDataStream& operator>>(QDataStream& s, Dividend& d) {
        s >> d._yield
          >> d._per_share
          >> d._pay_ratio
          >> d._next_date
          >> d._prev_date;

        quint32 size = 0;
        s >> size;

        d._history.clear();
        d._history.reserve(size);
        for (quint32 i = 0; i < size; ++i) {
            DividendList dl;
            float amount;

            s >> dl.amount >> dl.date;
            d._history.push_back(dl);
        }

        return s;
    }
}
