#include "shares.h"
#include "../instrument.h"

using namespace data;

data::Shares::Shares(Instrument* parent) : QObject(parent)
{
    //
}
Instrument* Shares::instrument() const { return static_cast <Instrument*> (parent()); }

double Shares::shares_outstanding() const { return _shares_outstanding; }
void   Shares::setSharesOutstanding(double newShares_outstanding)
{
    if (qFuzzyCompare(_shares_outstanding, newShares_outstanding))
        return;
    _shares_outstanding = newShares_outstanding;
    emit sharesOutstandingChanged(_shares_outstanding);
}

double Shares::shares_float() const { return _shares_float; }
void   Shares::setSharesFloat(double newShares_float)
{
    if (qFuzzyCompare(_shares_float, newShares_float))
        return;
    _shares_float = newShares_float;
    emit sharesFloatChanged(_shares_float);
}

float Shares::pct_insiders() const { return _pct_insiders;}
void  Shares::setPctInsiders(float newPct_insiders)
{
    if (qFuzzyCompare(_pct_insiders, newPct_insiders))
        return;
    _pct_insiders = newPct_insiders;
    emit pctInsidersChanged(_pct_insiders);
}

float Shares::pct_institutions() const { return _pct_institutions; }
void  Shares::setPctInstitutions(float newPct_institutions)
{
    if (qFuzzyCompare(_pct_institutions, newPct_institutions))
        return;
    _pct_institutions = newPct_institutions;
    emit pctInstitutionsChanged(_pct_institutions);
}

double Shares::shares_short() const { return _shares_short; }
void   Shares::setSharesShort(double newShares_short)
{
    if (qFuzzyCompare(_shares_short, newShares_short))
        return;
    _shares_short = newShares_short;
    emit sharesShortChanged(_shares_short);
}

float Shares::short_ratio() const { return _short_ratio; }
void  Shares::setShortRatio(float newShort_ratio)
{
    if (qFuzzyCompare(_short_ratio, newShort_ratio))
        return;
    _short_ratio = newShort_ratio;
    emit shortRatioChanged(_short_ratio);
}

float Shares::short_pct_out() const { return _short_pct_out; }
void  Shares::setShortPctOut(float newShort_pct_out)
{
    if (qFuzzyCompare(_short_pct_out, newShort_pct_out))
        return;
    _short_pct_out = newShort_pct_out;
    emit shortPctOutChanged(_short_pct_out);
}

float Shares::short_pct_float() const { return _short_pct_float; }
void  Shares::setShortPctFloat(float newShort_pct_float)
{
    if (qFuzzyCompare(_short_pct_float, newShort_pct_float))
        return;
    _short_pct_float = newShort_pct_float;
    emit shortPctFloatChanged(_short_pct_float);
}


namespace data {
    QDataStream& operator << (QDataStream& s, const Shares& d) {
        s << d._shares_outstanding
          << d._shares_float
          << d._shares_short
          << d._pct_insiders
          << d._pct_institutions
          << d._short_ratio
          << d._short_pct_out
          << d._short_pct_float;
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Shares& d) {
        s >> d._shares_outstanding
          >> d._shares_float
          >> d._shares_short
          >> d._pct_insiders
          >> d._pct_institutions
          >> d._short_ratio
          >> d._short_pct_out
          >> d._short_pct_float;
        return s;
    }
}
