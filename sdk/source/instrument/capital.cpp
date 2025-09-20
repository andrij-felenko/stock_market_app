#include "instrument/capital.h"
#include "utilities/features.h"

uint64_t sdk::Capital::sharesOutstanding() const { return _shares_outstanding; }
FieldTOpt sdk::Capital::setSharesOutstanding(int64_t value)
{ return sdk::set_if(this, _shares_outstanding, value, sdk::Cap_shares_outstand); }

uint64_t sdk::Capital::sharesFloat() const { return _shares_float; }
FieldTOpt sdk::Capital::setSharesFloat(int64_t value)
{ return sdk::set_if(this, _shares_float, value, sdk::Cap_shares_float); }

double sdk::Capital::percentOfInsiders() const { return _percent_of_insiders; }
FieldTOpt sdk::Capital::setPercentOfInsiders(double perc)
{ return sdk::set_if(this, _percent_of_insiders, perc, sdk::Cap_percent_of_insiders, 1e-12); }

double sdk::Capital::percentOfInstitution() const { return _percent_institution; }
FieldTOpt sdk::Capital::setPercentOfInsitution(double perc)
{ return sdk::set_if(this, _percent_institution, perc, sdk::Cap_percent_institution, 1e-12); }

int64_t sdk::Capital::outstandShare(uint16_t year, sdk::Quartel quart)
{
    for (const auto& it : _outstand_shares)
        if (it.year == year && it.quartel == quart)
            return it.shares;
    return -1;
}

int16_t sdk::Capital::outstandShare(const QDate& date)
{ return outstandShare(date.year(), sdk::quartel::from_month(date.month())); }

FieldTOpt sdk::Capital::setOutstandShare(uint64_t shares, uint16_t year, sdk::Quartel quart)
{
    for (auto& it : _outstand_shares)
        if (it.year == year && quart == it.quartel){
            if (shares == it.shares)
                return std::nullopt;
            it.shares = shares;
            _last_updated = QDateTime::currentDateTime();
            return sdk::Cap_outstand_shares;
        }

    _outstand_shares.emplace_back(year, quart, shares);
    _last_updated = QDateTime::currentDateTime();
    return sdk::Cap_outstand_shares;
}

FieldTOpt sdk::Capital::setOutstandShare(uint64_t shares, QDate date)
{
    return setOutstandShare(shares, date.year(), sdk::quartel::from_month(date.month()));
}


bool sdk::Capital::SharesPoint::operator == (const SharesPoint& o) const noexcept
{
    return year == o.year && quartel == o.quartel && shares == o.shares;
}

bool sdk::Capital::lessByYearQuartel(const SharesPoint& a, const SharesPoint& b)
{
    if (a.year != b.year) return a.year < b.year;
    return +a.quartel < +b.quartel;
}


namespace sdk {
    QDataStream& operator << (QDataStream& s, const Capital& d){
        sdk::list_to_stream(s, d._outstand_shares);
        return s << d._shares_outstanding << d._shares_float
                 << d._percent_of_insiders << d._percent_institution;
    }

    QDataStream& operator >> (QDataStream& s, Capital& d){
        sdk::list_from_stream(s, d._outstand_shares);
        return s >> d._shares_outstanding >> d._shares_float
                 >> d._percent_of_insiders >> d._percent_institution;
    }

    QDataStream& operator << (QDataStream& s, const Capital::SharesPoint& d)
    { return s << d.shares << d.year << d.quartel; }

    QDataStream& operator >> (QDataStream& s, Capital::SharesPoint& d)
    { return s >> d.shares >> d.year >> d.quartel; }
}
