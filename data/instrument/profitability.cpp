#include "profitability.h"

using namespace data;

data::Profitability::Profitability(QObject* parent) : QObject(parent)
{
    //
}

float Profitability::    roe() const { return _roe; }
void  Profitability::set_roe(float newRoe)
{
    if (qFuzzyCompare(_roe, newRoe))
        return;
    _roe = newRoe;
    emit roeChanged(_roe);
}

float Profitability::    roa() const { return _roa; }
void  Profitability::set_roa(float newRoa)
{
    if (qFuzzyCompare(_roa, newRoa))
        return;
    _roa = newRoa;
    emit roaChanged(_roa);
}

float Profitability::    margin_gros() const { return _margin_gros; }
void  Profitability::set_margin_gros(float new_gross_margin)
{
    if (qFuzzyCompare(_margin_gros, new_gross_margin))
        return;
    _margin_gros = new_gross_margin;
    emit marginGrosChanged(_margin_gros);
}

float Profitability::    margin_oper() const { return _margin_oper; }
void  Profitability::set_margin_oper(float new_operating_margin)
{
    if (qFuzzyCompare(_margin_oper, new_operating_margin))
        return;
    _margin_oper = new_operating_margin;
    emit marginOperChanged(_margin_oper);
}

float Profitability::    netincome() const { return _netincome; }
void  Profitability::set_netincome(float newNet_income)
{
    if (qFuzzyCompare(_netincome, newNet_income))
        return;
    _netincome = newNet_income;
    emit netincomeChanged(_netincome);
}

namespace data {
    QDataStream& operator<<(QDataStream& s, const Profitability& p)
    { return s << p._margin_gros << p._margin_oper << p._netincome << p._roa << p._roe; }

    QDataStream& operator>>(QDataStream& s, Profitability& p)
    { return s >> p._margin_gros >> p._margin_oper >> p._netincome >> p._roa >> p._roe; }
}
