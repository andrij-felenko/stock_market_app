#ifndef SDK_INSTRUMENT_BALANCE_H
#define SDK_INSTRUMENT_BALANCE_H

#include "sdk_def.h" // IWYU pragma: keep

class sdk::Balance : public Trackable
{
public:
    Balance() = default;

    /** @brief Дата подачі звіту для періоду (рік+квартал). */
    QDate filingDate() const;
    FieldTOpt setFilingDate(const QDate& date);

    /** @brief Загальні активи на кінець періоду. */
    double totalAssets() const;
    FieldTOpt setTotalAssets(double v);

    /** @brief Загальні зобов’язання на кінець періоду. */
    double totalLiabilities() const;
    FieldTOpt setTotalLiabilities(double v);

    /** @brief Власний капітал акціонерів на кінець періоду. */
    double totalShareholdersEquity() const;
    FieldTOpt setTotalShareholdersEquity(double v);

    /** @brief Контрольний підсумок пасивів і капіталу (звітний тотал). */
    double liabilitiesAndShareholdersEquity() const;
    FieldTOpt setLiabilitiesAndShareholdersEquity(double v);

    /** @brief Грошові кошти. */
    double cash() const;
    FieldTOpt setCash(double v);

    /** @brief Гроші та їх еквіваленти. */
    double cashAndCashEquivalents() const;
    FieldTOpt setCashAndCashEquivalents(double v);

    /** @brief Короткострокові інвестиції. */
    double shortTermInvestments() const;
    FieldTOpt setShortTermInvestments(double v);

    /** @brief Чиста дебіторська заборгованість. */
    double netReceivables() const;
    FieldTOpt setNetReceivables(double v);

    /** @brief Запаси. */
    double inventory() const;
    FieldTOpt setInventory(double v);

    /** @brief Інші оборотні активи. */
    double otherCurrentAssets() const;
    FieldTOpt setOtherCurrentAssets(double v);

    /** @brief Оборотні активи — підсумок. */
    double totalCurrentAssets() const;
    FieldTOpt setTotalCurrentAssets(double v);

    /** @brief Довгострокові інвестиції. */
    double longTermInvestments() const;
    FieldTOpt setLongTermInvestments(double v);

    /** @brief Основні засоби (PP&E), агрегат. */
    double propertyPlantAndEquipment() const;
    FieldTOpt setPropertyPlantAndEquipment(double v);

    /** @brief Основні засоби (валові, до зносу). */
    double propertyPlantAndEquipmentGross() const;
    FieldTOpt setPropertyPlantAndEquipmentGross(double v);

    /** @brief Основні засоби (чисті, після зносу). */
    double propertyPlantAndEquipmentNet() const;
    FieldTOpt setPropertyPlantAndEquipmentNet(double v);

    /** @brief Накопичена амортизація. */
    double accumulatedDepreciation() const;
    FieldTOpt setAccumulatedDepreciation(double v);

    /** @brief Нематеріальні активи. */
    double intangibleAssets() const;
    FieldTOpt setIntangibleAssets(double v);

    /** @brief Гудвіл. */
    double goodwill() const;
    FieldTOpt setGoodwill(double v);

    /** @brief Інші необоротні активи (агрегат). */
    double otherNonCurrentAssets() const;
    FieldTOpt setOtherNonCurrentAssets(double v);

    /** @brief Інші статті необоротних активів (окремий рядок). */
    double nonCurrentAssetsOther() const;
    FieldTOpt setNonCurrentAssetsOther(double v);

    /** @brief Необоротні активи — підсумок. */
    double nonCurrentAssetsTotal() const;
    FieldTOpt setNonCurrentAssetsTotal(double v);

    /** @brief Відкладені довгострокові витрати (активи). */
    double deferredLongTermAssetCharges() const;
    FieldTOpt setDeferredLongTermAssetCharges(double v);

    /** @brief Накопичена амортизація НМА. */
    double accumulatedAmortization() const;
    FieldTOpt setAccumulatedAmortization(double v);

    /** @brief Чисті матеріальні активи. */
    double netTangibleAssets() const;
    FieldTOpt setNetTangibleAssets(double v);

    /** @brief Кредиторська заборгованість. */
    double accountsPayable() const;
    FieldTOpt setAccountsPayable(double v);

    /** @brief Інші поточні зобов’язання. */
    double otherCurrentLiabilities() const;
    FieldTOpt setOtherCurrentLiabilities(double v);

    /** @brief Поточні зобов’язання — підсумок. */
    double totalCurrentLiabilities() const;
    FieldTOpt setTotalCurrentLiabilities(double v);

    /** @brief Відкладена виручка (поточна). */
    double currentDeferredRevenue() const;
    FieldTOpt setCurrentDeferredRevenue(double v);

    /** @brief Короткостроковий борг. */
    double shortTermDebt() const;
    FieldTOpt setShortTermDebt(double v);

    /** @brief Поточна частина довгострокового боргу. */
    double currentPortionOfLongTermDebt() const;
    FieldTOpt setCurrentPortionOfLongTermDebt(double v);

    /** @brief Довгостроковий борг. */
    double longTermDebt() const;
    FieldTOpt setLongTermDebt(double v);

    /** @brief Довгостроковий борг — підсумок. */
    double longTermDebtTotal() const;
    FieldTOpt setLongTermDebtTotal(double v);

    /** @brief Загальний борг (короткий + довгий). */
    double totalDebt() const;
    FieldTOpt setTotalDebt(double v);

    /** @brief Звичайний капітал (Common Stock). */
    double commonStock() const;
    FieldTOpt setCommonStock(double v);

    /** @brief Статутний/акціонерний капітал (Capital Stock). */
    double capitalStock() const;
    FieldTOpt setCapitalStock(double v);

    /** @brief Нерозподілений прибуток. */
    double retainedEarnings() const;
    FieldTOpt setRetainedEarnings(double v);

    /** @brief Інші компоненти власного капіталу. */
    double otherShareholdersEquity() const;
    FieldTOpt setOtherShareholdersEquity(double v);

    /** @brief Частка common stock у власному капіталі. */
    double commonStockTotalEquity() const;
    FieldTOpt setCommonStockTotalEquity(double v);

    /** @brief Частка preferred stock у власному капіталі. */
    double preferredStockTotalEquity() const;
    FieldTOpt setPreferredStockTotalEquity(double v);

    /** @brief Внесок нерозподіленого прибутку до власного капіталу. */
    double retainedEarningsTotalEquity() const;
    FieldTOpt setRetainedEarningsTotalEquity(double v);

    /** @brief Викуплені власні акції (treasury). */
    double treasuryStock() const;
    FieldTOpt setTreasuryStock(double v);

    /** @brief Додатково сплачений капітал (APIC). */
    double additionalPaidInCapital() const;
    FieldTOpt setAdditionalPaidInCapital(double v);

    /** @brief Постійний капітал. */
    double totalPermanentEquity() const;
    FieldTOpt setTotalPermanentEquity(double v);

    /** @brief Неконтрольна частка (міноритарії). */
    double noncontrollingInterest() const;
    FieldTOpt setNoncontrollingInterest(double v);

    /** @brief Викупні неконтрольні частки. */
    double redeemableNoncontrolInterests() const;
    FieldTOpt setRedeemableNoncontrolInterests(double v);

    /** @brief Накопичений інший сукупний дохід (AOCI). */
    double otherComprehensIncomeAccumulat() const;
    FieldTOpt setOtherComprehensIncomeAccumulat(double v);

    /** @brief Відкладені довгострокові зобов’язання. */
    double deferredLongTermLiabilities() const;
    FieldTOpt setDeferredLongTermLiabilities(double v);

    /** @brief Інші зобов’язання. */
    double otherLiabilities() const;
    FieldTOpt setOtherLiabilities(double v);

    /** @brief Інші необоротні зобов’язання. */
    double nonCurrentLiabilitiesOther() const;
    FieldTOpt setNonCurrentLiabilitiesOther(double v);

    /** @brief Необоротні зобов’язання — підсумок. */
    double nonCurrentLiabilitiesTotal() const;
    FieldTOpt setNonCurrentLiabilitiesTotal(double v);

    /** @brief Зобов’язання з фінансової оренди. */
    double capitalLeaseObligations() const;
    FieldTOpt setCapitalLeaseObligations(double v);

    /** @brief Варранти. */
    double warrants() const;
    FieldTOpt setWarrants(double v);

    /** @brief Викупні привілейовані акції. */
    double redeemablePreferredStock() const;
    FieldTOpt setRedeemablePreferredStock(double v);

    /** @brief Негативний гудвіл. */
    double negativeGoodwill() const;
    FieldTOpt setNegativeGoodwill(double v);

    /** @brief Додатковий капітал (surplus). */
    double capitalSurplus() const;
    FieldTOpt setCapitalSurplus(double v);

    /** @brief Гроші + короткострокові інвестиції. Рахується на льоту. */
    double cashAndShortTermInvestments() const;

    /** @brief Чистий борг = Загальний борг − Гроші та їх еквіваленти.
     *  Рахується на льоту. */
    double netDebt() const;

    /** @brief Чистий оборотний капітал = Оборотні активи − Поточні зобов’язання.
     *  Рахується на льоту. */
    double netWorkingCapital() const;

    /** @brief
     *  Інвестований капітал = Загальний борг + Власний капітал − Гроші та їх еквіваленти. */
    double netInvestedCapital() const;

private:
    double  to (int32_t d) const;
    int32_t from(double d) const;

    std::optional <FieldType> set_if(int32_t& field, const double& value);

    Currency _currency = Currency::None;
    QDate _filing_date = QDate();

    int32_t _total_assets = 0;
    int32_t _total_liabilities = 0;
    int32_t _total_shareholders_equity = 0;
    int32_t _liabilities_and_shareholders_equity = 0;

    int32_t _cash = 0;
    int32_t _cash_and_cash_equivalents = 0;
    int32_t _short_term_investments = 0;

    int32_t _net_receivables = 0;
    int32_t _inventory = 0;
    int32_t _other_current_assets = 0;
    int32_t _total_current_assets = 0;

    int32_t _long_term_investments = 0;
    int32_t _property_plant_and_equipment = 0;
    int32_t _property_plant_and_equipment_gross = 0;
    int32_t _property_plant_and_equipment_net = 0;
    int32_t _accumulated_depreciation = 0;
    int32_t _intangible_assets = 0;
    int32_t _goodwill = 0;

    int32_t _other_non_current_assets = 0;
    int32_t _non_current_assets_other = 0;
    int32_t _non_current_assets_total = 0;
    int32_t _deferred_long_term_asset_charges = 0;
    int32_t _accumulated_amortization = 0;
    int32_t _net_tangible_assets = 0;

    int32_t _accounts_payable = 0;
    int32_t _other_current_liabilities = 0;
    int32_t _total_current_liabilities = 0;
    int32_t _current_deferred_revenue = 0;

    int32_t _short_term_debt = 0;
    int32_t _current_portion_of_long_term_debt = 0;
    int32_t _long_term_debt = 0;
    int32_t _long_term_debt_total = 0;
    int32_t _total_debt = 0;

    int32_t _common_stock = 0;
    int32_t _capital_stock = 0;
    int32_t _retained_earnings = 0;
    int32_t _other_shareholders_equity = 0;
    int32_t _common_stock_total_equity = 0;
    int32_t _preferred_stock_total_equity = 0;
    int32_t _retained_earnings_total_equity = 0;
    int32_t _treasury_stock = 0;
    int32_t _additional_paid_in_capital = 0;
    int32_t _total_permanent_equity = 0;
    int32_t _noncontrolling_interest = 0;
    int32_t _redeemable_noncontrol_interests = 0;
    int32_t _other_comprehens_income_accumulat = 0;

    int32_t _deferred_long_term_liabilities = 0;
    int32_t _other_liabilities = 0;
    int32_t _non_current_liabilities_other = 0;
    int32_t _non_current_liabilities_total = 0;
    int32_t _capital_lease_obligations = 0;
    int32_t _warrants = 0;
    int32_t _redeemable_preferred_stock = 0;
    int32_t _negative_goodwill = 0;
    int32_t _capital_surplus = 0;

    friend class QuartelData;
    friend QDataStream& operator << (QDataStream& s, Wire <const Balance> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Balance> d);
};

#endif // SDK_INSTRUMENT_BALANCE_H
