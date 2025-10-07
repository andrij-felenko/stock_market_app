#ifndef SDK_INSTRUMENT_INCOMES_H
#define SDK_INSTRUMENT_INCOMES_H

#include "sdk_def.h"

class sdk::Incomes
{
public:
    Incomes() = default;

    /// \brief Дата подання звіту емітентом/регулятором (для таймлайнів/актуальності).
    QDate filingDate() const;
    FieldTOpt setFilingDate(const QDate& v);

    /// \brief Виручка за період до будь-яких витрат.
    int64_t totalRevenue() const;
    FieldTOpt setTotalRevenue(int64_t v);

    /// \brief Собівартість/прямі витрати на виробництво/надання послуг.
    int64_t costOfRevenue() const;
    FieldTOpt setCostOfRevenue(int64_t v);

    /// \brief Валовий прибуток = виручка − собівартість (як дає провайдер або для звірки).
    int64_t grossProfit() const;

    /// \brief Витрати на дослідження та розробки (R&D).
    int64_t researchDevelopment() const;
    FieldTOpt setResearchDevelopment(int64_t v);

    /// \brief Витрати на збут, загальногосподарські та адміністративні (SG&A).
    int64_t sellingGeneralAdmin() const;
    FieldTOpt setSellingGeneralAdmin(int64_t v);

    /// \brief Витрати на маркетинг і продажі (якщо виділяються окремо).
    int64_t sellingMarketingExpens() const;
    FieldTOpt setSellingMarketingExpens(int64_t v);

    /// \brief Інші операційні витрати, не включені до SG&A/R&D.
    int64_t otherOperatingExpenses() const;
    FieldTOpt setOtherOperatingExpenses(int64_t v);

    /// \brief Сукупні операційні витрати (контрольна сума провайдера).
    int64_t totalOperatingExpenses() const;

    /// \brief Операційний прибуток (EBIT) — результат від основної діяльності.
    int64_t operatingIncome() const;

    /// \brief Амортизація та знос (Depreciation & Amortization, D&A).
    int64_t deprAmortization() const;
    FieldTOpt setDeprAmortization(int64_t v);

    /// \brief Узгоджена амортизація (альтернативний рядок провайдера до D&A).
    int64_t reconciledDepreciation() const;
    FieldTOpt setReconciledDepreciation(int64_t v);

    /// \brief Чисті відсоткові доходи/витрати (нетто), якщо даються одним числом.
    int64_t netInterestIncome() const;

    /// \brief Відсоткові доходи (якщо виділяються окремо).
    int64_t interestIncome() const;
    FieldTOpt setInterestIncome(int64_t v);

    /// \brief Відсоткові витрати (якщо виділяються окремо).
    int64_t interestExpense() const;
    FieldTOpt setInterestExpense(int64_t v);

    /// \brief Інші неопераційні доходи/витрати, нетто.
    int64_t otherIncomeExpenseNet() const;
    FieldTOpt setOtherIncomeExpenseNet(int64_t v);

    /// \brief Прибуток до оподаткування (EBT).
    int64_t incomeBeforeTax() const;

    /// \brief Витрати з податку на прибуток за період.
    int64_t incomeTaxExpense() const;
    FieldTOpt setIncomeTaxExpense(int64_t v);

    /// \brief Чистий прибуток від безперервної діяльності (без discontinued).
    int64_t netIncomeContOps() const;
    FieldTOpt setNetIncomeContOps(int64_t v);

    /// \brief Результат від припиненої діяльності (якщо є).
    int64_t discontinuedOps() const;
    FieldTOpt setDiscontinuedOps(int64_t v);

    /// \brief Частка міноритарних акціонерів (якщо подається).
    int64_t minorityInterest() const;
    FieldTOpt setMinorityInterest(int64_t v);

    /// \brief Чистий прибуток за період (канонічний NI).
    int64_t netIncome() const;
    FieldTOpt setNetIncome(int64_t v);

    /// \brief Чистий прибуток, що належить звичайним акціям (для EPS).
    int64_t netIncomeToCommon() const;

    /// \brief Коригування за преференційними акціями/іншими привілейованими статтями.
    int64_t preferredAdjustments() const;
    FieldTOpt setPreferredAdjustments(int64_t v);

    /// \brief Ефекти змін облікової політики/нарахувань, які впливають на показники періоду.
    int64_t accountingEffects() const;
    FieldTOpt setAccountingEffects(int64_t v);

private:
    Trackable _track;
    sdk::Currency _currency;
    friend class QuartelData;

    QDate _filing_date;

    // перетворення масштабу: внутрішнє зберігання int32, зовнішній інтерфейс int64
    int64_t to  (int32_t d) const;
    int32_t from(int64_t d) const;

    std::optional<sdk::FieldType> set_if(int32_t& field, const int64_t& value);

    int32_t _total_revenue              = 0;
    int32_t _cost_of_revenue            = 0;

    int32_t _research_development       = 0;
    int32_t _selling_general_admin      = 0;
    int32_t _selling_marketing_expenses = 0;
    int32_t _other_operating_expenses   = 0;

    int32_t _depr_amortization          = 0; // D&A
    int32_t _reconciled_depreciation    = 0;

    int32_t _interest_income            = 0;
    int32_t _interest_expense           = 0;
    int32_t _other_income_expense_net   = 0;

    int32_t _income_tax_expense         = 0;

    int32_t _net_income_cont_ops        = 0; // ?????
    int32_t _discontinued_ops           = 0;
    int32_t _minority_interest          = 0;

    int32_t _net_income                 = 0;
    int32_t _preferred_adj              = 0;
    int32_t _acctg_effects              = 0;

    friend QDataStream& operator << (QDataStream& s, Wire <const Incomes> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Incomes> d);
};

#endif // SDK_INSTRUMENT_INCOMES_H
