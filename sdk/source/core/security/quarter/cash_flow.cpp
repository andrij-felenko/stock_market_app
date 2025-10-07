#include "core/security//quarter/cash_flow.h"

using namespace sdk;

QDate CashFlow::filingDate() const { return _filing_date; }
FieldTOpt CashFlow::setFilingDate(const QDate& date)
{ return sdk::set_if(&_track, _filing_date, date, sdk::Fundam_balance); }

int64_t CashFlow::to  (int32_t d) const { return uint64_t(d) * sdk::currency::scale(_currency); }
int32_t CashFlow::from(int64_t d) const { return          d  / sdk::currency::scale(_currency); }



int64_t CashFlow::investingNetsInvestments() const { return to(_investing_net_investments); }
FieldTOpt CashFlow::setInverstinNetsInvestment(int64_t v)
{ return set_if(&_track, _investing_net_investments, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingChangeLiabilities() const { return to(_operating_change_liabilities); }
FieldTOpt CashFlow::setOperatingChangeLiabilities(int64_t v)
{ return set_if(&_track, _operating_change_liabilities, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::investingTotal() const { return to(_investing_total); }
FieldTOpt CashFlow::setInvestingTotal(int64_t v)
{ return set_if(&_track, _investing_total, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingOtherAdjustments() const { return to(_operating_other_adjustments); }
FieldTOpt CashFlow::setOperatingOtherAdjustments(int64_t v)
{ return set_if(&_track, _operating_other_adjustments, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::financingEquityIssuance() const { return to(_financing_equity_issuance); }
FieldTOpt CashFlow::setFinancingEquityIssuance(int64_t v)
{ return set_if(&_track, _financing_equity_issuance, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::investingOther() const { return to(_investing_other); }
FieldTOpt CashFlow::setInvestingOther(int64_t v)
{ return set_if(&_track, _investing_other, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::financingDividendsPaid() const { return to(_financing_dividends_paid); }
FieldTOpt CashFlow::setFinancingDividendsPaid(int64_t v)
{ return set_if(&_track, _financing_dividends_paid, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingChangeInventory() const { return to(_operating_change_inventory); }
FieldTOpt CashFlow::setOperatingChangeInventory(int64_t v)
{ return set_if(&_track, _operating_change_inventory, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::financingBuybackNet() const { return to(_financing_buyback_net); }
FieldTOpt CashFlow::setFinancingBuybackNet(int64_t v)
{ return set_if(&_track, _financing_buyback_net, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::financingOther() const { return to(_financing_other); }
FieldTOpt CashFlow::setFinancingOther(int64_t v)
{ return set_if(&_track, _financing_other, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingNetIncomeAdjust() const { return to(_operating_net_income_adjust); }
FieldTOpt CashFlow::setOperatingNetIncomeAdjust(int64_t v)
{ return set_if(&_track, _operating_net_income_adjust, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::investingCapitalSpending() const { return to(_investing_capital_spending); }
FieldTOpt CashFlow::setInvestingCapitalSpending(int64_t v)
{ return set_if(&_track, _investing_capital_spending, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingChangeReceivables() const { return to(_operating_change_receivables); }
FieldTOpt CashFlow::setOperatingChangeReceivables(int64_t v)
{ return set_if(&_track, _operating_change_receivables, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingOther() const { return to(_operating_other); }
FieldTOpt CashFlow::setOperatingOther(int64_t v)
{ return set_if(&_track, _operating_other, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingNoncashOther() const { return to(_operating_noncash_other); }
FieldTOpt CashFlow::setOperatingNoncashOther(int64_t v)
{ return set_if(&_track, _operating_noncash_other, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::freeCash() const { return to(_free_cash); }
FieldTOpt CashFlow::setFreeCash(int64_t v)
{ return set_if(&_track, _free_cash, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::financingNetBorrowings() const { return to(_financing_net_borrowings); }
FieldTOpt CashFlow::setFinancingNetBorrowings(int64_t v)
{ return set_if(&_track, _financing_net_borrowings, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::netIncome() const { return to(_net_income); }
FieldTOpt CashFlow::setNetIncome(int64_t v)
{ return set_if(&_track, _net_income, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::operatingTotal() const { return to(_operating_total); }
FieldTOpt CashFlow::setOperatingTotal(int64_t v)
{ return set_if(&_track, _operating_total, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::depreciation() const { return to(_depreciation); }
FieldTOpt CashFlow::setDepreciation(int64_t v)
{ return set_if(&_track, _depreciation, from(v), sdk::Fundam_cashflow); }

int64_t CashFlow::stockBasedCompensation() const { return to(_stock_based_compensation); }
FieldTOpt CashFlow::setStockBasedCompensation(int64_t v)
{ return set_if(&_track, _stock_based_compensation, from(v), sdk::Fundam_cashflow); }

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const CashFlow> d){
        if (d.data()) s << d->_filing_date
                        << d->_investing_net_investments
                        << d->_operating_change_liabilities
                        << d->_investing_total
                        << d->_operating_other_adjustments
                        << d->_financing_equity_issuance
                        << d->_investing_other
                        << d->_financing_dividends_paid
                        << d->_operating_change_inventory
                        << d->_financing_buyback_net
                        << d->_financing_other
                        << d->_operating_net_income_adjust
                        << d->_investing_capital_spending
                        << d->_operating_change_receivables
                        << d->_operating_other
                        << d->_operating_noncash_other
                        << d->_free_cash
                        << d->_financing_net_borrowings
                        << d->_net_income
                        << d->_operating_total
                        << d->_depreciation
                        << d->_stock_based_compensation;

        return s << d->_track;
    }

    QDataStream& operator >> (QDataStream& s, Wire <CashFlow> d){
        if (d.data()) s >> d->_filing_date
                        >> d->_investing_net_investments
                        >> d->_operating_change_liabilities
                        >> d->_investing_total
                        >> d->_operating_other_adjustments
                        >> d->_financing_equity_issuance
                        >> d->_investing_other
                        >> d->_financing_dividends_paid
                        >> d->_operating_change_inventory
                        >> d->_financing_buyback_net
                        >> d->_financing_other
                        >> d->_operating_net_income_adjust
                        >> d->_investing_capital_spending
                        >> d->_operating_change_receivables
                        >> d->_operating_other
                        >> d->_operating_noncash_other
                        >> d->_free_cash
                        >> d->_financing_net_borrowings
                        >> d->_net_income
                        >> d->_operating_total
                        >> d->_depreciation
                        >> d->_stock_based_compensation;

        return s >> d->_track;
    }
}
