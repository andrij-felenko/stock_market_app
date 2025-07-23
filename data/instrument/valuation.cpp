#include "valuation.h"

using namespace data;

data::Valuation::Valuation(QObject* parent) : QObject(parent)
{
    //
}

float Valuation::    eps() const { return _eps; }
void  Valuation::set_eps(float new_eps)
{
    if (qFuzzyCompare(_eps, new_eps))
        return;
    _eps = new_eps;
    emit epsChanged(_eps);
}

float Valuation::    pe_ratio() const { return _pe_ratio; }
void  Valuation::set_pe_ratio(float new_pe_ratio)
{
    if (qFuzzyCompare(_pe_ratio, new_pe_ratio))
        return;
    _pe_ratio = new_pe_ratio;
    emit pe_ratioChanged(_pe_ratio);
}

float Valuation::    pb_ratio() const { return _pb_ratio; }
void  Valuation::set_pb_ratio(float new_pb_ratio)
{
    if (qFuzzyCompare(_pb_ratio, new_pb_ratio))
        return;
    _pb_ratio = new_pb_ratio;
    emit pb_ratioChanged(_pb_ratio);
}

float Valuation::    book_value() const { return _book_value; }
void  Valuation::set_book_value(float new_book_value)
{
    if (qFuzzyCompare(_book_value, new_book_value))
        return;
    _book_value = new_book_value;
    emit book_valueChanged(_book_value);
}

float Valuation::    market_cap() const { return _market_cap; }
void  Valuation::set_market_cap(float new_market_cap)
{
    if (qFuzzyCompare(_market_cap, new_market_cap))
        return;
    _market_cap = new_market_cap;
    emit market_capChanged(_market_cap);
}

float Valuation::    share_count() const { return _share_count; }
void  Valuation::set_share_count(float new_share_count)
{
    if (qFuzzyCompare(_share_count, new_share_count))
        return;
    _share_count = new_share_count;
    emit share_countChanged(_share_count);
}

namespace data {
    QDataStream& operator<<(QDataStream& s, const Valuation& d) {
        return s << d._book_value << d._eps      << d._market_cap
                 << d._pb_ratio   << d._pe_ratio << d._share_count;
    }

    QDataStream& operator>>(QDataStream& s, Valuation& d) {
        return s >> d._book_value >> d._eps      >> d._market_cap
                 >> d._pb_ratio   >> d._pe_ratio >> d._share_count;
    }
}
