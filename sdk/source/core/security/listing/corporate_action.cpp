#include "core/security/listing/corporate_action.h"

const QDate& sdk::CorporateAction::lastSplitDate() const { return _last_split_date; }
std::optional<sdk::FieldType> sdk::CorporateAction::setLastSplitDate(const QDate& v)
{ return sdk::set_if(this, _last_split_date, v, sdk::Corp_split_date); }

std::pair <int, int> sdk::CorporateAction::lastSplitFactor() const { return _last_split_factor; }
FieldTOpt sdk::CorporateAction::setLastSplitFactor(int a, int b)
{
    return sdk::set_if(this, _last_split_factor, std::make_pair(a, b),
                       sdk::Corp_last_split_factor);
}


namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const CorporateAction> d){
        s << d->_last_split_factor
          << d->_last_split_date;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <CorporateAction> d){
        s >> d->_last_split_factor
          >> d->_last_split_date;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
