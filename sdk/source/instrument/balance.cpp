#include "instrument/balance.h"

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
double    Balance::   totalAssets() const { return to(_totalAssets); }
FieldTOpt Balance::setTotalAssets(double v)
{ return set_if(_totalAssets, v); }

// totalLiabilities
double    Balance::   totalLiabilities() const { return to(_totalLiabilities); }
FieldTOpt Balance::setTotalLiabilities(double v)
{ return set_if(_totalLiabilities, v); }

// totalShareholdersEquity
double    Balance::   totalShareholdersEquity() const { return to(_totalShareholdersEquity); }
FieldTOpt Balance::setTotalShareholdersEquity(double v)
{ return set_if(_totalShareholdersEquity, v); }

// liabilitiesAndShareholdersEquity
double    Balance::   liabilitiesAndShareholdersEquity() const
{ return to(_liabilitiesAndShareholdersEquity); }
FieldTOpt Balance::setLiabilitiesAndShareholdersEquity(double v)
{ return set_if(_liabilitiesAndShareholdersEquity, v); }

// cash
double    Balance::   cash() const { return to(_cash); }
FieldTOpt Balance::setCash(double v)
{ return set_if(_cash, v); }

// cashAndCashEquivalents
double    Balance::   cashAndCashEquivalents() const { return to(_cashAndCashEquivalents); }
FieldTOpt Balance::setCashAndCashEquivalents(double v)
{ return set_if(_cashAndCashEquivalents, v); }

// shortTermInvestments
double    Balance::   shortTermInvestments() const { return to(_shortTermInvestments); }
FieldTOpt Balance::setShortTermInvestments(double v)
{ return set_if(_shortTermInvestments, v); }

// netReceivables
double    Balance::   netReceivables() const { return to(_netReceivables); }
FieldTOpt Balance::setNetReceivables(double v)
{ return set_if(_netReceivables, v); }

// inventory
double    Balance::   inventory() const { return to(_inventory); }
FieldTOpt Balance::setInventory(double v)
{ return set_if(_inventory, v); }

// otherCurrentAssets
double    Balance::   otherCurrentAssets() const { return to(_otherCurrentAssets); }
FieldTOpt Balance::setOtherCurrentAssets(double v)
{ return set_if(_otherCurrentAssets, v); }

// totalCurrentAssets
double    Balance::   totalCurrentAssets() const { return to(_totalCurrentAssets); }
FieldTOpt Balance::setTotalCurrentAssets(double v)
{ return set_if(_totalCurrentAssets, v); }

// longTermInvestments
double    Balance::   longTermInvestments() const { return to(_longTermInvestments); }
FieldTOpt Balance::setLongTermInvestments(double v)
{ return set_if(_longTermInvestments, v); }

// propertyPlantAndEquipment
double    Balance::   propertyPlantAndEquipment() const { return to(_propertyPlantAndEquipment); }
FieldTOpt Balance::setPropertyPlantAndEquipment(double v)
{ return set_if(_propertyPlantAndEquipment, v); }

// propertyPlantAndEquipmentGross
double    Balance::   propertyPlantAndEquipmentGross() const
{ return to(_propertyPlantAndEquipmentGross); }
FieldTOpt Balance::setPropertyPlantAndEquipmentGross(double v)
{ return set_if(_propertyPlantAndEquipmentGross, v); }

// propertyPlantAndEquipmentNet
double    Balance::   propertyPlantAndEquipmentNet() const
{ return to(_propertyPlantAndEquipmentNet); }
FieldTOpt Balance::setPropertyPlantAndEquipmentNet(double v)
{ return set_if(_propertyPlantAndEquipmentNet, v); }

// accumulatedDepreciation
double    Balance::   accumulatedDepreciation() const { return to(_accumulatedDepreciation); }
FieldTOpt Balance::setAccumulatedDepreciation(double v)
{ return set_if(_accumulatedDepreciation, v); }

// intangibleAssets
double    Balance::   intangibleAssets() const { return to(_intangibleAssets); }
FieldTOpt Balance::setIntangibleAssets(double v)
{ return set_if(_intangibleAssets, v); }

// goodwill
double    Balance::   goodwill() const { return to(_goodwill); }
FieldTOpt Balance::setGoodwill(double v)
{ return set_if(_goodwill, v); }

// otherNonCurrentAssets
double    Balance::   otherNonCurrentAssets() const { return to(_otherNonCurrentAssets); }
FieldTOpt Balance::setOtherNonCurrentAssets(double v)
{ return set_if(_otherNonCurrentAssets, v); }

// nonCurrentAssetsOther
double    Balance::   nonCurrentAssetsOther() const { return to(_nonCurrentAssetsOther); }
FieldTOpt Balance::setNonCurrentAssetsOther(double v)
{ return set_if(_nonCurrentAssetsOther, v); }

// nonCurrentAssetsTotal
double    Balance::   nonCurrentAssetsTotal() const { return to(_nonCurrentAssetsTotal); }
FieldTOpt Balance::setNonCurrentAssetsTotal(double v)
{ return set_if(_nonCurrentAssetsTotal, v); }

// deferredLongTermAssetCharges
double    Balance::   deferredLongTermAssetCharges() const
{ return to(_deferredLongTermAssetCharges); }
FieldTOpt Balance::setDeferredLongTermAssetCharges(double v)
{ return set_if(_deferredLongTermAssetCharges, v); }

// accumulatedAmortization
double    Balance::   accumulatedAmortization() const { return to(_accumulatedAmortization); }
FieldTOpt Balance::setAccumulatedAmortization(double v)
{ return set_if(_accumulatedAmortization, v); }

// netTangibleAssets
double    Balance::   netTangibleAssets() const { return to(_netTangibleAssets); }
FieldTOpt Balance::setNetTangibleAssets(double v)
{ return set_if(_netTangibleAssets, v); }

// accountsPayable
double    Balance::   accountsPayable() const { return to(_accountsPayable); }
FieldTOpt Balance::setAccountsPayable(double v)
{ return set_if(_accountsPayable, v); }

// otherCurrentLiabilities
double    Balance::   otherCurrentLiabilities() const { return to(_otherCurrentLiabilities); }
FieldTOpt Balance::setOtherCurrentLiabilities(double v)
{ return set_if(_otherCurrentLiabilities, v); }

// totalCurrentLiabilities
double    Balance::   totalCurrentLiabilities() const { return to(_totalCurrentLiabilities); }
FieldTOpt Balance::setTotalCurrentLiabilities(double v)
{ return set_if(_totalCurrentLiabilities, v); }

// currentDeferredRevenue
double    Balance::   currentDeferredRevenue() const { return to(_currentDeferredRevenue); }
FieldTOpt Balance::setCurrentDeferredRevenue(double v)
{ return set_if(_currentDeferredRevenue, v); }

// shortTermDebt
double    Balance::   shortTermDebt() const { return to(_shortTermDebt); }
FieldTOpt Balance::setShortTermDebt(double v)
{ return set_if(_shortTermDebt, v); }

// currentPortionOfLongTermDebt
double    Balance::   currentPortionOfLongTermDebt() const
{ return to(_currentPortionOfLongTermDebt); }
FieldTOpt Balance::setCurrentPortionOfLongTermDebt(double v)
{ return set_if(_currentPortionOfLongTermDebt, v); }

// longTermDebt
double    Balance::   longTermDebt() const { return to(_longTermDebt); }
FieldTOpt Balance::setLongTermDebt(double v)
{ return set_if(_longTermDebt, v); }

// longTermDebtTotal
double    Balance::   longTermDebtTotal() const { return to(_longTermDebtTotal); }
FieldTOpt Balance::setLongTermDebtTotal(double v)
{ return set_if(_longTermDebtTotal, v); }

// totalDebt
double    Balance::   totalDebt() const { return to(_totalDebt); }
FieldTOpt Balance::setTotalDebt(double v)
{ return set_if(_totalDebt, v); }

// commonStock
double    Balance::   commonStock() const { return to(_commonStock); }
FieldTOpt Balance::setCommonStock(double v)
{ return set_if(_commonStock, v); }

// capitalStock
double    Balance::   capitalStock() const { return to(_capitalStock); }
FieldTOpt Balance::setCapitalStock(double v)
{ return set_if(_capitalStock, v); }

// retainedEarnings
double    Balance::   retainedEarnings() const { return to(_retainedEarnings); }
FieldTOpt Balance::setRetainedEarnings(double v)
{ return set_if(_retainedEarnings, v); }

// otherShareholdersEquity
double    Balance::   otherShareholdersEquity() const { return to(_otherShareholdersEquity); }
FieldTOpt Balance::setOtherShareholdersEquity(double v)
{ return set_if(_otherShareholdersEquity, v); }

// commonStockTotalEquity
double    Balance::   commonStockTotalEquity() const { return to(_commonStockTotalEquity); }
FieldTOpt Balance::setCommonStockTotalEquity(double v)
{ return set_if(_commonStockTotalEquity, v); }

// preferredStockTotalEquity
double    Balance::   preferredStockTotalEquity() const { return to(_preferredStockTotalEquity); }
FieldTOpt Balance::setPreferredStockTotalEquity(double v)
{ return set_if(_preferredStockTotalEquity, v); }

// retainedEarningsTotalEquity
double    Balance::   retainedEarningsTotalEquity() const
{ return to(_retainedEarningsTotalEquity); }
FieldTOpt Balance::setRetainedEarningsTotalEquity(double v)
{ return set_if(_retainedEarningsTotalEquity, v); }

// treasuryStock
double    Balance::   treasuryStock() const { return to(_treasuryStock); }
FieldTOpt Balance::setTreasuryStock(double v)
{ return set_if(_treasuryStock, v); }

// additionalPaidInCapital
double    Balance::   additionalPaidInCapital() const { return to(_additionalPaidInCapital); }
FieldTOpt Balance::setAdditionalPaidInCapital(double v)
{ return set_if(_additionalPaidInCapital, v); }

// totalPermanentEquity
double    Balance::   totalPermanentEquity() const { return to(_totalPermanentEquity); }
FieldTOpt Balance::setTotalPermanentEquity(double v)
{ return set_if(_totalPermanentEquity, v); }

// noncontrollingInterest
double    Balance::   noncontrollingInterest() const { return to(_noncontrollingInterest); }
FieldTOpt Balance::setNoncontrollingInterest(double v)
{ return set_if(_noncontrollingInterest, v); }

// redeemableNoncontrolInterests
double    Balance::   redeemableNoncontrolInterests() const
{ return to(_redeemableNoncontrolInterests); }
FieldTOpt Balance::setRedeemableNoncontrolInterests(double v)
{ return set_if(_redeemableNoncontrolInterests, v); }

// otherComprehensIncomeAccumulat
double    Balance::   otherComprehensIncomeAccumulat() const
{ return to(_otherComprehensIncomeAccumulat); }
FieldTOpt Balance::setOtherComprehensIncomeAccumulat(double v)
{ return set_if(_otherComprehensIncomeAccumulat, v); }

// deferredLongTermLiabilities
double    Balance::   deferredLongTermLiabilities() const
{ return to(_deferredLongTermLiabilities); }
FieldTOpt Balance::setDeferredLongTermLiabilities(double v)
{ return set_if(_deferredLongTermLiabilities, v); }

// otherLiabilities
double    Balance::   otherLiabilities() const { return to(_otherLiabilities); }
FieldTOpt Balance::setOtherLiabilities(double v)
{ return set_if(_otherLiabilities, v); }

// nonCurrentLiabilitiesOther
double    Balance::   nonCurrentLiabilitiesOther() const
{ return to(_nonCurrentLiabilitiesOther); }
FieldTOpt Balance::setNonCurrentLiabilitiesOther(double v)
{ return set_if(_nonCurrentLiabilitiesOther, v); }

// nonCurrentLiabilitiesTotal
double    Balance::   nonCurrentLiabilitiesTotal() const
{ return to(_nonCurrentLiabilitiesTotal); }
FieldTOpt Balance::setNonCurrentLiabilitiesTotal(double v)
{ return set_if(_nonCurrentLiabilitiesTotal, v); }

// capitalLeaseObligations
double    Balance::   capitalLeaseObligations() const { return to(_capitalLeaseObligations); }
FieldTOpt Balance::setCapitalLeaseObligations(double v)
{ return set_if(_capitalLeaseObligations, v); }

// warrants
double    Balance::   warrants() const { return to(_warrants); }
FieldTOpt Balance::setWarrants(double v)
{ return set_if(_warrants, v); }

// redeemablePreferredStock
double    Balance::   redeemablePreferredStock() const { return to(_redeemablePreferredStock); }
FieldTOpt Balance::setRedeemablePreferredStock(double v)
{ return set_if(_redeemablePreferredStock, v); }

// negativeGoodwill
double    Balance::   negativeGoodwill() const { return to(_negativeGoodwill); }
FieldTOpt Balance::setNegativeGoodwill(double v)
{ return set_if(_negativeGoodwill, v); }

// capitalSurplus
double    Balance::   capitalSurplus() const { return to(_capitalSurplus); }
FieldTOpt Balance::setCapitalSurplus(double v)
{ return set_if(_capitalSurplus, v); }


namespace sdk {
    QDataStream& operator << (QDataStream& s, const Balance& d){
        return s << d._filing_date
                 << d._totalAssets
                 << d._totalLiabilities
                 << d._totalShareholdersEquity
                 << d._liabilitiesAndShareholdersEquity

                 << d._cash
                 << d._cashAndCashEquivalents
                 << d._shortTermInvestments

                 << d._netReceivables
                 << d._inventory
                 << d._otherCurrentAssets
                 << d._totalCurrentAssets

                 << d._longTermInvestments
                 << d._propertyPlantAndEquipment
                 << d._propertyPlantAndEquipmentGross
                 << d._propertyPlantAndEquipmentNet
                 << d._accumulatedDepreciation
                 << d._intangibleAssets
                 << d._goodwill

                 << d._otherNonCurrentAssets
                 << d._nonCurrentAssetsOther
                 << d._nonCurrentAssetsTotal
                 << d._deferredLongTermAssetCharges
                 << d._accumulatedAmortization
                 << d._netTangibleAssets

                 << d._accountsPayable
                 << d._otherCurrentLiabilities
                 << d._totalCurrentLiabilities
                 << d._currentDeferredRevenue

                 << d._shortTermDebt
                 << d._currentPortionOfLongTermDebt
                 << d._longTermDebt
                 << d._longTermDebtTotal
                 << d._totalDebt

                 << d._commonStock
                 << d._capitalStock
                 << d._retainedEarnings
                 << d._otherShareholdersEquity
                 << d._commonStockTotalEquity
                 << d._preferredStockTotalEquity
                 << d._retainedEarningsTotalEquity
                 << d._treasuryStock
                 << d._additionalPaidInCapital
                 << d._totalPermanentEquity
                 << d._noncontrollingInterest
                 << d._redeemableNoncontrolInterests
                 << d._otherComprehensIncomeAccumulat

                 << d._deferredLongTermLiabilities
                 << d._otherLiabilities
                 << d._nonCurrentLiabilitiesOther
                 << d._nonCurrentLiabilitiesTotal
                 << d._capitalLeaseObligations
                 << d._warrants
                 << d._redeemablePreferredStock
                 << d._negativeGoodwill
                 << d._capitalSurplus;
    }

    QDataStream& operator >> (QDataStream& s, Balance& d){
        return s >> d._filing_date
                 >> d._totalAssets
                 >> d._totalLiabilities
                 >> d._totalShareholdersEquity
                 >> d._liabilitiesAndShareholdersEquity

                 >> d._cash
                 >> d._cashAndCashEquivalents
                 >> d._shortTermInvestments

                 >> d._netReceivables
                 >> d._inventory
                 >> d._otherCurrentAssets
                 >> d._totalCurrentAssets

                 >> d._longTermInvestments
                 >> d._propertyPlantAndEquipment
                 >> d._propertyPlantAndEquipmentGross
                 >> d._propertyPlantAndEquipmentNet
                 >> d._accumulatedDepreciation
                 >> d._intangibleAssets
                 >> d._goodwill

                 >> d._otherNonCurrentAssets
                 >> d._nonCurrentAssetsOther
                 >> d._nonCurrentAssetsTotal
                 >> d._deferredLongTermAssetCharges
                 >> d._accumulatedAmortization
                 >> d._netTangibleAssets

                 >> d._accountsPayable
                 >> d._otherCurrentLiabilities
                 >> d._totalCurrentLiabilities
                 >> d._currentDeferredRevenue

                 >> d._shortTermDebt
                 >> d._currentPortionOfLongTermDebt
                 >> d._longTermDebt
                 >> d._longTermDebtTotal
                 >> d._totalDebt

                 >> d._commonStock
                 >> d._capitalStock
                 >> d._retainedEarnings
                 >> d._otherShareholdersEquity
                 >> d._commonStockTotalEquity
                 >> d._preferredStockTotalEquity
                 >> d._retainedEarningsTotalEquity
                 >> d._treasuryStock
                 >> d._additionalPaidInCapital
                 >> d._totalPermanentEquity
                 >> d._noncontrollingInterest
                 >> d._redeemableNoncontrolInterests
                 >> d._otherComprehensIncomeAccumulat

                 >> d._deferredLongTermLiabilities
                 >> d._otherLiabilities
                 >> d._nonCurrentLiabilitiesOther
                 >> d._nonCurrentLiabilitiesTotal
                 >> d._capitalLeaseObligations
                 >> d._warrants
                 >> d._redeemablePreferredStock
                 >> d._negativeGoodwill
                 >> d._capitalSurplus;
    }
}
