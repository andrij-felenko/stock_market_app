#include "data/instrument/valuation.h"
#include "data/instrument.h"

using namespace data;

data::Valuation::Valuation(Instrument* parent) : QObject(parent)
{
    //
}
Instrument* Valuation::instrument() const { return static_cast <Instrument*> (parent()); }

uint64_t Valuation::shares_outstanding() const { return _shares_outstanding; }
void     Valuation::setSharesOutstanding(uint64_t v) {
    if (_shares_outstanding == v) return;
    _shares_outstanding = v;
    emit shares_outChanged(v);
}

double Valuation::book_value_ps_mrq() const { return _book_value_ps_mrq; }
void   Valuation::setBookValuePsMrq(double v) {
    if (_book_value_ps_mrq == v) return;
    _book_value_ps_mrq = v;
    emit bvps_mrqChanged(v);
}

double Valuation::market_capitalization() const { return _market_capitalization; }
void   Valuation::setMarketCapitalization(double v) {
    if (_market_capitalization == v) return;
    _market_capitalization = v;
    emit market_capChanged(v);
}

double Valuation::enterprise_value() const { return _enterprise_value; }
void   Valuation::setEnterpriseValue(double v) {
    if (_enterprise_value == v) return;
    _enterprise_value = v;
    emit evChanged(v);
}

double Valuation::earnings_per_share_ttm() const { return _earnings_per_share_ttm; }
void   Valuation::setEarningsPerShareTtm(double v) {
    if (_earnings_per_share_ttm == v) return;
    _earnings_per_share_ttm = v;
    emit eps_ttmChanged(v);
}

float Valuation::price_to_earnings_ttm() const { return _price_to_earnings_ttm; }
void  Valuation::setPriceToEarningsTtm(float v) {
    if (_price_to_earnings_ttm == v) return;
    _price_to_earnings_ttm = v;
    emit pe_ttmChanged(v);
}

float Valuation::price_to_earnings_fwd() const { return _price_to_earnings_fwd; }
void  Valuation::setPriceToEarningsFwd(float v) {
    if (_price_to_earnings_fwd == v) return;
    _price_to_earnings_fwd = v;
    emit pe_fwdChanged(v);
}

float Valuation::price_to_book_mrq() const { return _price_to_book_mrq; }
void  Valuation::setPriceToBookMrq(float v) {
    if (_price_to_book_mrq == v) return;
    _price_to_book_mrq = v;
    emit pb_mrqChanged(v);
}

float Valuation::price_to_sales_ttm() const { return _price_to_sales_ttm; }
void  Valuation::setPriceToSalesTtm(float v) {
    if (_price_to_sales_ttm == v) return;
    _price_to_sales_ttm = v;
    emit ps_ttmChanged(v);
}

float Valuation::ev_to_sales_ttm() const { return _ev_to_sales_ttm; }
void  Valuation::setEvToSalesTtm(float v) {
    if (_ev_to_sales_ttm == v) return;
    _ev_to_sales_ttm = v;
    emit ev_sales_ttmChanged(v);
}

float Valuation::ev_to_ebitda() const { return _ev_to_ebitda; }
void  Valuation::setEvToEbitda(float v) {
    if (_ev_to_ebitda == v) return;
    _ev_to_ebitda = v;
    emit ev_ebitdaChanged(v);
}


namespace data {
    QDataStream& operator<<(QDataStream& s, const Valuation& d) {
        return s << d._shares_outstanding
                 << d._book_value_ps_mrq
                 << d._market_capitalization
                 << d._enterprise_value
                 << d._earnings_per_share_ttm
                 << d._price_to_earnings_ttm
                 << d._price_to_earnings_fwd
                 << d._price_to_book_mrq
                 << d._price_to_sales_ttm
                 << d._ev_to_sales_ttm
                 << d._ev_to_ebitda;
    }

    QDataStream& operator>>(QDataStream& s, Valuation& d) {
        return s >> d._shares_outstanding
                 >> d._book_value_ps_mrq
                 >> d._market_capitalization
                 >> d._enterprise_value
                 >> d._earnings_per_share_ttm
                 >> d._price_to_earnings_ttm
                 >> d._price_to_earnings_fwd
                 >> d._price_to_book_mrq
                 >> d._price_to_sales_ttm
                 >> d._ev_to_sales_ttm
                 >> d._ev_to_ebitda;
    }
}
