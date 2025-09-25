#include "core/security/quarter/balance.h"

using namespace sdk;

// ------------------------------------------------------------------------------------------------

QDate Balance::filingDate() const { return _filing_date; }
FieldTOpt Balance::setFilingDate(const QDate& date)
{ return sdk::set_if(this, _filing_date, date, sdk::Fundam_balance); }

double  Balance::to  (int32_t d) const { return double(d) * sdk::currency::scale(_currency); }
int32_t Balance::from(double d)  const { return        d  / sdk::currency::scale(_currency); }

std::optional<sdk::FieldType> Balance::set_if(int32_t& field, const double& value)
{ return sdk::set_if(this, field, from(value), sdk::Fundam_balance); }
// ================================================================================================


// ------------------------------------------------------------------------------------------------
double Balance::cashAndShortTermInvestments() const
{ return cash() + shortTermInvestments(); }

double Balance::netDebt() const
{ return totalDebt() - cashAndCashEquivalents(); }

double Balance::netWorkingCapital() const
{ return totalCurrentAssets() - totalCurrentLiabilities(); }

double Balance::netInvestedCapital() const
{ return totalDebt() + totalShareholdersEquity() - cashAndCashEquivalents(); }
// ================================================================================================


// ------------------------------------------------------------------------------------------------
// totalAssets (виправив на std_if)
double    Balance::   totalAssets() const { return to(_total_assets); }
FieldTOpt Balance::setTotalAssets(double v)
{ return set_if(_total_assets, v); }

// totalLiabilities
double    Balance::   totalLiabilities() const { return to(_total_liabilities); }
FieldTOpt Balance::setTotalLiabilities(double v)
{ return set_if(_total_liabilities, v); }

// totalShareholdersEquity
double    Balance::   totalShareholdersEquity() const { return to(_total_shareholders_equity); }
FieldTOpt Balance::setTotalShareholdersEquity(double v)
{ return set_if(_total_shareholders_equity, v); }

// liabilitiesAndShareholdersEquity
double    Balance::   liabilitiesAndShareholdersEquity() const
{ return to(_liabilities_and_shareholders_equity); }
FieldTOpt Balance::setLiabilitiesAndShareholdersEquity(double v)
{ return set_if(_liabilities_and_shareholders_equity, v); }

// cash
double    Balance::   cash() const { return to(_cash); }
FieldTOpt Balance::setCash(double v)
{ return set_if(_cash, v); }

// cashAndCashEquivalents
double    Balance::   cashAndCashEquivalents() const { return to(_cash_and_cash_equivalents); }
FieldTOpt Balance::setCashAndCashEquivalents(double v)
{ return set_if(_cash_and_cash_equivalents, v); }

// shortTermInvestments
double    Balance::   shortTermInvestments() const { return to(_short_term_investments); }
FieldTOpt Balance::setShortTermInvestments(double v)
{ return set_if(_short_term_investments, v); }

// netReceivables
double    Balance::   netReceivables() const { return to(_net_receivables); }
FieldTOpt Balance::setNetReceivables(double v)
{ return set_if(_net_receivables, v); }

// inventory
double    Balance::   inventory() const { return to(_inventory); }
FieldTOpt Balance::setInventory(double v)
{ return set_if(_inventory, v); }

// otherCurrentAssets
double    Balance::   otherCurrentAssets() const { return to(_other_current_assets); }
FieldTOpt Balance::setOtherCurrentAssets(double v)
{ return set_if(_other_current_assets, v); }

// totalCurrentAssets
double    Balance::   totalCurrentAssets() const { return to(_total_current_assets); }
FieldTOpt Balance::setTotalCurrentAssets(double v)
{ return set_if(_total_current_assets, v); }

// longTermInvestments
double    Balance::   longTermInvestments() const { return to(_long_term_investments); }
FieldTOpt Balance::setLongTermInvestments(double v)
{ return set_if(_long_term_investments, v); }

// propertyPlantAndEquipment
double    Balance::   propertyPlantAndEquipment() const
{ return to(_property_plant_and_equipment); }
FieldTOpt Balance::setPropertyPlantAndEquipment(double v)
{ return set_if(_property_plant_and_equipment, v); }

// propertyPlantAndEquipmentGross
double    Balance::   propertyPlantAndEquipmentGross() const
{ return to(_property_plant_and_equipment_gross); }
FieldTOpt Balance::setPropertyPlantAndEquipmentGross(double v)
{ return set_if(_property_plant_and_equipment_gross, v); }

// propertyPlantAndEquipmentNet
double    Balance::   propertyPlantAndEquipmentNet() const
{ return to(_property_plant_and_equipment_net); }
FieldTOpt Balance::setPropertyPlantAndEquipmentNet(double v)
{ return set_if(_property_plant_and_equipment_net, v); }

// accumulatedDepreciation
double    Balance::   accumulatedDepreciation() const { return to(_accumulated_depreciation); }
FieldTOpt Balance::setAccumulatedDepreciation(double v)
{ return set_if(_accumulated_depreciation, v); }

// intangibleAssets
double    Balance::   intangibleAssets() const { return to(_intangible_assets); }
FieldTOpt Balance::setIntangibleAssets(double v)
{ return set_if(_intangible_assets, v); }

// goodwill
double    Balance::   goodwill() const { return to(_goodwill); }
FieldTOpt Balance::setGoodwill(double v)
{ return set_if(_goodwill, v); }

// otherNonCurrentAssets
double    Balance::   otherNonCurrentAssets() const { return to(_other_non_current_assets); }
FieldTOpt Balance::setOtherNonCurrentAssets(double v)
{ return set_if(_other_non_current_assets, v); }

// nonCurrentAssetsOther
double    Balance::   nonCurrentAssetsOther() const { return to(_non_current_assets_other); }
FieldTOpt Balance::setNonCurrentAssetsOther(double v)
{ return set_if(_non_current_assets_other, v); }

// nonCurrentAssetsTotal
double    Balance::   nonCurrentAssetsTotal() const { return to(_non_current_assets_total); }
FieldTOpt Balance::setNonCurrentAssetsTotal(double v)
{ return set_if(_non_current_assets_total, v); }

// deferredLongTermAssetCharges
double    Balance::   deferredLongTermAssetCharges() const
{ return to(_deferred_long_term_asset_charges); }
FieldTOpt Balance::setDeferredLongTermAssetCharges(double v)
{ return set_if(_deferred_long_term_asset_charges, v); }

// accumulatedAmortization
double    Balance::   accumulatedAmortization() const { return to(_accumulated_amortization); }
FieldTOpt Balance::setAccumulatedAmortization(double v)
{ return set_if(_accumulated_amortization, v); }

// netTangibleAssets
double    Balance::   netTangibleAssets() const { return to(_net_tangible_assets); }
FieldTOpt Balance::setNetTangibleAssets(double v)
{ return set_if(_net_tangible_assets, v); }

// accountsPayable
double    Balance::   accountsPayable() const { return to(_accounts_payable); }
FieldTOpt Balance::setAccountsPayable(double v)
{ return set_if(_accounts_payable, v); }

// otherCurrentLiabilities
double    Balance::   otherCurrentLiabilities() const { return to(_other_current_liabilities); }
FieldTOpt Balance::setOtherCurrentLiabilities(double v)
{ return set_if(_other_current_liabilities, v); }

// totalCurrentLiabilities
double    Balance::   totalCurrentLiabilities() const { return to(_total_current_liabilities); }
FieldTOpt Balance::setTotalCurrentLiabilities(double v)
{ return set_if(_total_current_liabilities, v); }

// currentDeferredRevenue
double    Balance::   currentDeferredRevenue() const { return to(_current_deferred_revenue); }
FieldTOpt Balance::setCurrentDeferredRevenue(double v)
{ return set_if(_current_deferred_revenue, v); }

// shortTermDebt
double    Balance::   shortTermDebt() const { return to(_short_term_debt); }
FieldTOpt Balance::setShortTermDebt(double v)
{ return set_if(_short_term_debt, v); }

// currentPortionOfLongTermDebt
double    Balance::   currentPortionOfLongTermDebt() const
{ return to(_current_portion_of_long_term_debt); }
FieldTOpt Balance::setCurrentPortionOfLongTermDebt(double v)
{ return set_if(_current_portion_of_long_term_debt, v); }

// longTermDebt
double    Balance::   longTermDebt() const { return to(_long_term_debt); }
FieldTOpt Balance::setLongTermDebt(double v)
{ return set_if(_long_term_debt, v); }

// longTermDebtTotal
double    Balance::   longTermDebtTotal() const { return to(_long_term_debt_total); }
FieldTOpt Balance::setLongTermDebtTotal(double v)
{ return set_if(_long_term_debt_total, v); }

// totalDebt
double    Balance::   totalDebt() const { return to(_total_debt); }
FieldTOpt Balance::setTotalDebt(double v)
{ return set_if(_total_debt, v); }

// commonStock
double    Balance::   commonStock() const { return to(_common_stock); }
FieldTOpt Balance::setCommonStock(double v)
{ return set_if(_common_stock, v); }

// capitalStock
double    Balance::   capitalStock() const { return to(_capital_stock); }
FieldTOpt Balance::setCapitalStock(double v)
{ return set_if(_capital_stock, v); }

// retainedEarnings
double    Balance::   retainedEarnings() const { return to(_retained_earnings); }
FieldTOpt Balance::setRetainedEarnings(double v)
{ return set_if(_retained_earnings, v); }

// otherShareholdersEquity
double    Balance::   otherShareholdersEquity() const { return to(_other_shareholders_equity); }
FieldTOpt Balance::setOtherShareholdersEquity(double v)
{ return set_if(_other_shareholders_equity, v); }

// commonStockTotalEquity
double    Balance::   commonStockTotalEquity() const { return to(_common_stock_total_equity); }
FieldTOpt Balance::setCommonStockTotalEquity(double v)
{ return set_if(_common_stock_total_equity, v); }

// preferredStockTotalEquity
double    Balance::   preferredStockTotalEquity() const
{ return to(_preferred_stock_total_equity); }
FieldTOpt Balance::setPreferredStockTotalEquity(double v)
{ return set_if(_preferred_stock_total_equity, v); }

// retainedEarningsTotalEquity
double    Balance::   retainedEarningsTotalEquity() const
{ return to(_retained_earnings_total_equity); }
FieldTOpt Balance::setRetainedEarningsTotalEquity(double v)
{ return set_if(_retained_earnings_total_equity, v); }

// treasuryStock
double    Balance::   treasuryStock() const { return to(_treasury_stock); }
FieldTOpt Balance::setTreasuryStock(double v)
{ return set_if(_treasury_stock, v); }

// additionalPaidInCapital
double    Balance::   additionalPaidInCapital() const { return to(_additional_paid_in_capital); }
FieldTOpt Balance::setAdditionalPaidInCapital(double v)
{ return set_if(_additional_paid_in_capital, v); }

// totalPermanentEquity
double    Balance::   totalPermanentEquity() const { return to(_total_permanent_equity); }
FieldTOpt Balance::setTotalPermanentEquity(double v)
{ return set_if(_total_permanent_equity, v); }

// noncontrollingInterest
double    Balance::   noncontrollingInterest() const { return to(_noncontrolling_interest); }
FieldTOpt Balance::setNoncontrollingInterest(double v)
{ return set_if(_noncontrolling_interest, v); }

// redeemableNoncontrolInterests
double    Balance::   redeemableNoncontrolInterests() const
{ return to(_redeemable_noncontrol_interests); }
FieldTOpt Balance::setRedeemableNoncontrolInterests(double v)
{ return set_if(_redeemable_noncontrol_interests, v); }

// otherComprehensIncomeAccumulat
double    Balance::   otherComprehensIncomeAccumulat() const
{ return to(_other_comprehens_income_accumulat); }
FieldTOpt Balance::setOtherComprehensIncomeAccumulat(double v)
{ return set_if(_other_comprehens_income_accumulat, v); }

// deferredLongTermLiabilities
double    Balance::   deferredLongTermLiabilities() const
{ return to(_deferred_long_term_liabilities); }
FieldTOpt Balance::setDeferredLongTermLiabilities(double v)
{ return set_if(_deferred_long_term_liabilities, v); }

// otherLiabilities
double    Balance::   otherLiabilities() const { return to(_other_liabilities); }
FieldTOpt Balance::setOtherLiabilities(double v)
{ return set_if(_other_liabilities, v); }

// nonCurrentLiabilitiesOther
double    Balance::   nonCurrentLiabilitiesOther() const
{ return to(_non_current_liabilities_other); }
FieldTOpt Balance::setNonCurrentLiabilitiesOther(double v)
{ return set_if(_non_current_liabilities_other, v); }

// nonCurrentLiabilitiesTotal
double    Balance::   nonCurrentLiabilitiesTotal() const
{ return to(_non_current_liabilities_total); }
FieldTOpt Balance::setNonCurrentLiabilitiesTotal(double v)
{ return set_if(_non_current_liabilities_total, v); }

// capitalLeaseObligations
double    Balance::   capitalLeaseObligations() const { return to(_capital_lease_obligations); }
FieldTOpt Balance::setCapitalLeaseObligations(double v)
{ return set_if(_capital_lease_obligations, v); }

// warrants
double    Balance::   warrants() const { return to(_warrants); }
FieldTOpt Balance::setWarrants(double v)
{ return set_if(_warrants, v); }

// redeemablePreferredStock
double    Balance::   redeemablePreferredStock() const { return to(_redeemable_preferred_stock); }
FieldTOpt Balance::setRedeemablePreferredStock(double v)
{ return set_if(_redeemable_preferred_stock, v); }

// negativeGoodwill
double    Balance::   negativeGoodwill() const { return to(_negative_goodwill); }
FieldTOpt Balance::setNegativeGoodwill(double v)
{ return set_if(_negative_goodwill, v); }

// capitalSurplus
double    Balance::   capitalSurplus() const { return to(_capital_surplus); }
FieldTOpt Balance::setCapitalSurplus(double v)
{ return set_if(_capital_surplus, v); }


namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Balance> d){
        s   << d->_filing_date
            << d->_total_assets
            << d->_total_liabilities
            << d->_total_shareholders_equity
            << d->_liabilities_and_shareholders_equity

            << d->_cash
            << d->_cash_and_cash_equivalents
            << d->_short_term_investments

            << d->_net_receivables
            << d->_inventory
            << d->_other_current_assets
            << d->_total_current_assets

            << d->_long_term_investments
            << d->_property_plant_and_equipment
            << d->_property_plant_and_equipment_gross
            << d->_property_plant_and_equipment_net
            << d->_accumulated_depreciation
            << d->_intangible_assets
            << d->_goodwill

            << d->_other_non_current_assets
            << d->_non_current_assets_other
            << d->_non_current_assets_total
            << d->_deferred_long_term_asset_charges
            << d->_accumulated_amortization
            << d->_net_tangible_assets

            << d->_accounts_payable
            << d->_other_current_liabilities
            << d->_total_current_liabilities
            << d->_current_deferred_revenue

            << d->_short_term_debt
            << d->_current_portion_of_long_term_debt
            << d->_long_term_debt
            << d->_long_term_debt_total
            << d->_total_debt

            << d->_common_stock
            << d->_capital_stock
            << d->_retained_earnings
            << d->_other_shareholders_equity
            << d->_common_stock_total_equity
            << d->_preferred_stock_total_equity
            << d->_retained_earnings_total_equity
            << d->_treasury_stock
            << d->_additional_paid_in_capital
            << d->_total_permanent_equity
            << d->_noncontrolling_interest
            << d->_redeemable_noncontrol_interests
            << d->_other_comprehens_income_accumulat

            << d->_deferred_long_term_liabilities
            << d->_other_liabilities
            << d->_non_current_liabilities_other
            << d->_non_current_liabilities_total
            << d->_capital_lease_obligations
            << d->_warrants
            << d->_redeemable_preferred_stock
            << d->_negative_goodwill
            << d->_capital_surplus;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Balance> d){
        s   >> d->_filing_date
            >> d->_total_assets
            >> d->_total_liabilities
            >> d->_total_shareholders_equity
            >> d->_liabilities_and_shareholders_equity

            >> d->_cash
            >> d->_cash_and_cash_equivalents
            >> d->_short_term_investments

            >> d->_net_receivables
            >> d->_inventory
            >> d->_other_current_assets
            >> d->_total_current_assets

            >> d->_long_term_investments
            >> d->_property_plant_and_equipment
            >> d->_property_plant_and_equipment_gross
            >> d->_property_plant_and_equipment_net
            >> d->_accumulated_depreciation
            >> d->_intangible_assets
            >> d->_goodwill

            >> d->_other_non_current_assets
            >> d->_non_current_assets_other
            >> d->_non_current_assets_total
            >> d->_deferred_long_term_asset_charges
            >> d->_accumulated_amortization
            >> d->_net_tangible_assets

            >> d->_accounts_payable
            >> d->_other_current_liabilities
            >> d->_total_current_liabilities
            >> d->_current_deferred_revenue

            >> d->_short_term_debt
            >> d->_current_portion_of_long_term_debt
            >> d->_long_term_debt
            >> d->_long_term_debt_total
            >> d->_total_debt

            >> d->_common_stock
            >> d->_capital_stock
            >> d->_retained_earnings
            >> d->_other_shareholders_equity
            >> d->_common_stock_total_equity
            >> d->_preferred_stock_total_equity
            >> d->_retained_earnings_total_equity
            >> d->_treasury_stock
            >> d->_additional_paid_in_capital
            >> d->_total_permanent_equity
            >> d->_noncontrolling_interest
            >> d->_redeemable_noncontrol_interests
            >> d->_other_comprehens_income_accumulat

            >> d->_deferred_long_term_liabilities
            >> d->_other_liabilities
            >> d->_non_current_liabilities_other
            >> d->_non_current_liabilities_total
            >> d->_capital_lease_obligations
            >> d->_warrants
            >> d->_redeemable_preferred_stock
            >> d->_negative_goodwill
            >> d->_capital_surplus;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
