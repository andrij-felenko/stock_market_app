#include "instrument/incomes.h"

using namespace sdk;


QDate Incomes::filingDate() const { return _filing_date; }
FieldTOpt Incomes::setFilingDate(const QDate& date)
{ return sdk::set_if(this, _filing_date, date, sdk::Fundam_balance); }


int64_t Incomes::to  (int32_t d) const { return int64_t(d) * sdk::currency::scale(_currency); }
int32_t Incomes::from(int64_t d) const { return         d  / sdk::currency::scale(_currency); }

std::optional<sdk::FieldType> Incomes::set_if(int32_t& field, const int64_t& value)
{ return sdk::set_if(this, field, from(value), sdk::Fundam_incomes); }



// ------------------------------------------------------------------------------------------------
int64_t Incomes::grossProfit() const { return to(_total_revenue - _cost_of_revenue); }

int64_t Incomes::totalOperatingExpenses() const
{ return to(_research_development + _selling_general_admin +
            _selling_marketing_expenses + _other_operating_expenses); }

int64_t Incomes::operatingIncome() const
{
    return to(_total_revenue - _cost_of_revenue - (_research_development +
                                                   _selling_general_admin +
                                                   _selling_marketing_expenses +
                                                   _other_operating_expenses));
}

int64_t Incomes::netInterestIncome() const
{
    const int32_t income  = _interest_income;
    const int32_t expense = _interest_expense;
    // Працює з будь-якою конвенцією знака: якщо витрата вже від’ємна — додаємо,
    // якщо додатна — віднімаємо.
    const int32_t net = income + ((expense < 0) ? expense : -expense);
    return to(net);
}

int64_t Incomes::incomeBeforeTax() const
{
    const int32_t op = _total_revenue
                       - _cost_of_revenue
                       - (_research_development +
                          _selling_general_admin +
                          _selling_marketing_expenses +
                          _other_operating_expenses);

    const int32_t net_interest = _interest_income + ((_interest_expense < 0) ?  _interest_expense
                                                                             : -_interest_expense);

    return to(op + _other_income_expense_net + net_interest);
}

int64_t Incomes::netIncomeToCommon() const
{
    const int32_t ni_to_common = _net_income - _minority_interest - _preferred_adj;
    return to(ni_to_common);
}
// ================================================================================================



// ------------------------------------------------------------------------------------------------
int64_t Incomes::totalRevenue() const { return to(_total_revenue); }
FieldTOpt Incomes::setTotalRevenue(int64_t v)
{ return set_if(_total_revenue, from(v)); }

int64_t Incomes::costOfRevenue() const { return to(_cost_of_revenue); }
FieldTOpt Incomes::setCostOfRevenue(int64_t v)
{ return set_if(_cost_of_revenue, from(v)); }

int64_t Incomes::researchDevelopment() const { return to(_research_development); }
FieldTOpt Incomes::setResearchDevelopment(int64_t v)
{ return set_if(_research_development, from(v)); }

int64_t Incomes::sellingGeneralAdmin() const { return to(_selling_general_admin); }
FieldTOpt Incomes::setSellingGeneralAdmin(int64_t v)
{ return set_if(_selling_general_admin, from(v)); }

int64_t Incomes::sellingMarketingExpens() const { return to(_selling_marketing_expenses); }
FieldTOpt Incomes::setSellingMarketingExpens(int64_t v)
{ return set_if(_selling_marketing_expenses, from(v)); }

int64_t Incomes::otherOperatingExpenses() const { return to(_other_operating_expenses); }
FieldTOpt Incomes::setOtherOperatingExpenses(int64_t v)
{ return set_if(_other_operating_expenses, from(v)); }

int64_t Incomes::deprAmortization() const { return to(_depr_amortization); }
FieldTOpt Incomes::setDeprAmortization(int64_t v)
{ return set_if(_depr_amortization, from(v)); }

int64_t Incomes::reconciledDepreciation() const { return to(_reconciled_depreciation); }
FieldTOpt Incomes::setReconciledDepreciation(int64_t v)
{ return set_if(_reconciled_depreciation, from(v)); }

int64_t Incomes::interestIncome() const { return to(_interest_income); }
FieldTOpt Incomes::setInterestIncome(int64_t v)
{ return set_if(_interest_income, from(v)); }

int64_t Incomes::interestExpense() const { return to(_interest_expense); }
FieldTOpt Incomes::setInterestExpense(int64_t v)
{ return set_if(_interest_expense, from(v)); }

int64_t Incomes::otherIncomeExpenseNet() const { return to(_other_income_expense_net); }
FieldTOpt Incomes::setOtherIncomeExpenseNet(int64_t v)
{ return set_if(_other_income_expense_net, from(v)); }

int64_t Incomes::incomeTaxExpense() const { return to(_income_tax_expense); }
FieldTOpt Incomes::setIncomeTaxExpense(int64_t v)
{ return set_if(_income_tax_expense, from(v)); }

int64_t Incomes::netIncomeContOps() const { return to(_net_income_cont_ops); }
FieldTOpt Incomes::setNetIncomeContOps(int64_t v)
{ return set_if(_net_income_cont_ops, from(v)); }

int64_t Incomes::discontinuedOps() const { return to(_discontinued_ops); }
FieldTOpt Incomes::setDiscontinuedOps(int64_t v)
{ return set_if(_discontinued_ops, from(v)); }

int64_t Incomes::minorityInterest() const { return to(_minority_interest); }
FieldTOpt Incomes::setMinorityInterest(int64_t v)
{ return set_if(_minority_interest, from(v)); }

int64_t Incomes::netIncome() const { return to(_net_income); }
FieldTOpt Incomes::setNetIncome(int64_t v)
{ return set_if(_net_income, from(v)); }

int64_t Incomes::preferredAdjustments() const { return to(_preferred_adj); }
FieldTOpt Incomes::setPreferredAdjustments(int64_t v)
{ return set_if(_preferred_adj, from(v)); }

int64_t Incomes::accountingEffects() const { return to(_acctg_effects); }
FieldTOpt Incomes::setAccountingEffects(int64_t v)
{ return set_if(_acctg_effects, from(v)); }
// ================================================================================================

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Incomes& d){
        return s << d._filing_date
                 << d._total_revenue
                 << d._cost_of_revenue

                 << d._research_development
                 << d._selling_general_admin
                 << d._selling_marketing_expenses
                 << d._other_operating_expenses

                 << d._depr_amortization
                 << d._reconciled_depreciation

                 << d._interest_income
                 << d._interest_expense
                 << d._other_income_expense_net

                 << d._income_tax_expense

                 << d._net_income_cont_ops
                 << d._discontinued_ops
                 << d._minority_interest

                 << d._net_income
                 << d._preferred_adj
                 << d._acctg_effects;
    }

    QDataStream& operator >> (QDataStream& s, Incomes& d){
        return s >> d._filing_date
                 >> d._total_revenue
                 >> d._cost_of_revenue

                 >> d._research_development
                 >> d._selling_general_admin
                 >> d._selling_marketing_expenses
                 >> d._other_operating_expenses

                 >> d._depr_amortization
                 >> d._reconciled_depreciation

                 >> d._interest_income
                 >> d._interest_expense
                 >> d._other_income_expense_net

                 >> d._income_tax_expense

                 >> d._net_income_cont_ops
                 >> d._discontinued_ops
                 >> d._minority_interest

                 >> d._net_income
                 >> d._preferred_adj
                 >> d._acctg_effects;
    }
}
