#ifndef SDK_INSTRUMENT_CASHFLOW_H
#define SDK_INSTRUMENT_CASHFLOW_H

#include "sdk.h" // IWYU pragma: keep

class sdk::CashFlow : Trackable
{
public:
    CashFlow() = default;

    /** @brief Дата подачі звіту для періоду (рік+квартал). */
    QDate filingDate() const;
    FieldTOpt setFilingDate(const QDate& date);

    /**
     * \brief Сукупний грошовий потік від фінансової діяльності за період.
     *
     * За можливості обчислюється як сума компонентів фінансування:
     * (net_borrowings + equity_issuance + buyback_net + dividends_paid + financing_other)
     * у спільному масштабі та валюті серії. Якщо один чи більше компонентів недоступні
     * (відсутні у вихідних даних), допускається використання збереженого значення, як його
     * надав постачальник. Знак та трактування потоків відповідають джерелу. Для контролю
     * консистентності може порівнюватися з
     * cashEnd() − cashBegin() − operatingTotal() − investingTotal() − fxEffectOnCash().
     */
    [[nodiscard]] int32_t financingTotal() const;

    /**
     * \brief Чиста зміна грошових коштів за період.
     *
     * Переважний спосіб — сума трьох потоків діяльності з урахуванням FX:
     * operatingTotal() + investingTotal() + financingTotal() + fxEffectOnCash().
     * Альтернатива — різниця між кінцевим і початковим сальдо: cashEnd() − cashBegin().
     * Значення повинно узгоджуватися з обома підходами; у разі розбіжностей рекомендовано
     * діагностувати причину вхідних даних (відсутні/подвійно враховані статті).
     */
    [[nodiscard]] int32_t changeInCash() const;

    /**
     * \brief Сальдо грошових коштів на початок періоду.
     *
     * Для безперервного ряду дорівнює cashEnd() попереднього періоду в тій самій валюті
     * та масштабі. Для першого періоду ряду береться із джерела даних або з балансу на
     * початок. Може слугувати контрольним значенням у звірці: cashBegin() + changeInCash()
     * ≈ cashEnd() (з урахуванням FX-ефекту, якщо він виділяється окремо).
     */
    [[nodiscard]] int32_t cashBegin() const;

    /**
     * \brief Сальдо грошових коштів та їх еквівалентів на кінець періоду.
     *
     * Зазвичай відповідає рядку “Cash and Cash Equivalents” у балансі на дату періоду.
     * Використовується як опорна величина для звірки тоталів Cash Flow та розрахунку
     * changeInCash(). Має бути в однаковому масштабі та валюті з іншими полями серії.
     */
    [[nodiscard]] int32_t cashEnd() const;

    /**
     * \brief Вплив змін валютних курсів на грошові кошти за період.
     *
     * Обчислюється як залишок у рівнянні узгодження:
     * fx = cashEnd() − cashBegin() − (operatingTotal() + investingTotal() + financingTotal()).
     * Якщо постачальник надає явне значення, воно може використовуватися напряму. Поле
     * пояснює різницю між бухгалтерськими потоками діяльності та фактичним рухом коштів
     * через переоцінку/перерахунок валюти.
     */
    [[nodiscard]] int32_t fxEffectOnCash() const;

    /**
     * \brief Зміна грошових коштів та їх еквівалентів за період.
     *
     * У більшості джерел дублює changeInCash(). Якщо у вашій моделі відстежуються
     * окремо «cash» та «cash equivalents», величина може відображати саме зміну частки
     * «еквівалентів» або сукупну зміну «cash & equivalents». Рекомендовано тримати як
     * похідне від cashEnd() − cashBegin(), якщо джерело не визначає інше явно.
     */
    [[nodiscard]] int32_t cashEquivalentsChange() const;

    /**
     * \brief Сукупна зміна оборотного капіталу за період.
     *
     * Розраховується як різниця між чистим оборотним капіталом на кінець та початок:
     * (currentAssets − currentLiabilities)_t − (currentAssets − currentLiabilities)_{t−1}.
     * Слугує ключовою корекцією в непрямому методі формування операційного потоку і
     * пояснює розбіжність між чистим прибутком та фактичним грошовим потоком. Має
     * обчислюватися у тій самій валюті та масштабі, що й інші статті періоду.
     */
    [[nodiscard]] int32_t workingCapitalChange() const;

    /// \brief Чисті інвестиції за період.
    /// Простими словами: скільки грошей пішло/прийшло через купівлю або продаж інвестактивів
    /// (купівлі зазвичай «мінус», продажі — «плюс»; знак як у джерела).
    int64_t investingNetsInvestments() const;
    FieldTOpt setInverstinNetsInvestment(int64_t v);

    /// \brief Вплив зміни зобов’язань на реальні гроші від операцій.
    /// Якщо кредиторка зросла — компанія поки не заплатила готівкою (часто «плюс»);
    /// якщо зменшилась — гроші пішли (часто «мінус»).
    int64_t operatingChangeLiabilities() const;
    FieldTOpt setOperatingChangeLiabilities(int64_t v);

    /// \brief Підсумок грошових потоків інвестдіяльності за період.
    /// Показує сумарно, скільки кешу пішло/прийшло через інвестиції, CapEx та інші інвест-операції.
    int64_t investingTotal() const;
    FieldTOpt setInvestingTotal(int64_t v);

    /// \brief Інші коригування в операційній діяльності.
    /// «Мішечок» дрібних правок, які допомагають зблизити прибуток на папері з реальним грошовим потоком.
    int64_t operatingOtherAdjustments() const;
    FieldTOpt setOperatingOtherAdjustments(int64_t v);

    /// \brief Гроші, отримані від випуску нових акцій.
    /// Компанія продала нові акції — отримала кеш (зазвичай «плюс»).
    int64_t financingEquityIssuance() const;
    FieldTOpt setFinancingEquityIssuance(int64_t v);

    /// \brief Інші інвестиційні потоки.
    /// Рідкі/дрібні інвест-операції, що не підпали під окремі статті (не CapEx і не «investments»).
    int64_t investingOther() const;
    FieldTOpt setInvestingOther(int64_t v);

    /// \brief Виплачені дивіденди грошима.
    /// Це виплати акціонерам і зазвичай означають відтік кешу («мінус»).
    int64_t financingDividendsPaid() const;
    FieldTOpt setFinancingDividendsPaid(int64_t v);

    /// \brief Вплив зміни запасів на операційний кеш.
    /// Більше запасів — гроші «зависли на складі» (часто «мінус»); менше — гроші вивільнились («плюс»).
    int64_t operatingChangeInventory() const;
    FieldTOpt setOperatingChangeInventory(int64_t v);

    /// \brief Чистий ефект операцій з власними акціями (викуп/продаж).
    /// Викуп зазвичай забирає кеш («мінус»), продаж — приносить («плюс»).
    int64_t financingBuybackNet() const;
    FieldTOpt setFinancingBuybackNet(int64_t v);

    /// \brief Інші фінансові грошові потоки.
    /// Комісії, дрібні погашення/залучення тощо — все, що не виділено окремими рядками.
    int64_t financingOther() const;
    FieldTOpt setFinancingOther(int64_t v);

    /// \brief Узагальнена поправка від чистого прибутку до операційного кеш-потоку.
    /// Постачальник дає одним числом «дрібні» не грошові/вирівнювальні ефекти.
    int64_t operatingNetIncomeAdjust() const;
    FieldTOpt setOperatingNetIncomeAdjust(int64_t v);

    /// \brief Капітальні видатки (CapEx).
    /// Великі покупки для розвитку (обладнання, будівлі) — зазвичай відтік готівки («мінус»).
    int64_t investingCapitalSpending() const;
    FieldTOpt setInvestingCapitalSpending(int64_t v);

    /// \brief Вплив зміни дебіторської заборгованості на кеш.
    /// Більше дебіторки — гроші ще не надійшли («мінус»); менше — кошти зайшли («плюс»).
    int64_t operatingChangeReceivables() const;
    FieldTOpt setOperatingChangeReceivables(int64_t v);

    /// \brief Інші операційні грошові потоки.
    /// Дрібні статті, які все одно рухають реальні гроші в межах операцій.
    int64_t operatingOther() const;
    FieldTOpt setOperatingOther(int64_t v);

    /// \brief Інші не грошові коригування в операціях.
    /// Те, що впливає на прибуток, але не рухає кеш (резерви, переоцінки тощо).
    int64_t operatingNoncashOther() const;
    FieldTOpt setOperatingNoncashOther(int64_t v);

    /// \brief Вільний грошовий потік (FCF).
    /// Скільки кешу лишається після необхідних інвестицій — «вільні» гроші бізнесу.
    int64_t freeCash() const;
    FieldTOpt setFreeCash(int64_t v);

    /// \brief Чисті запозичення.
    /// Скільки нового боргу взяли мінус що погасили: плюс — залучили кеш, мінус — віддали кеш.
    int64_t financingNetBorrowings() const;
    FieldTOpt setFinancingNetBorrowings(int64_t v);

    /// \brief Чистий прибуток за період.
    /// Базова величина з P&L, від якої рухаємося до реального операційного кешу (CFO).
    int64_t netIncome() const;
    FieldTOpt setNetIncome(int64_t v);

    /// \brief Операційний грошовий потік (CFO).
    /// Гроші від основної діяльності без інвестицій і фінансування — «серцевина» генерування кешу.
    int64_t operatingTotal() const;
    FieldTOpt setOperatingTotal(int64_t v);

    /// \brief Амортизація.
    /// Витрата в прибутку без руху готівки; у CFO її додають назад.
    int64_t depreciation() const;
    FieldTOpt setDepreciation(int64_t v);

    /// \brief Винагороди акціями (SBC).
    /// Також «не кеш»: зменшують прибуток, але гроші не виходять, тому додаються назад у CFO.
    int64_t stockBasedCompensation() const;
    FieldTOpt setStockBasedCompensation(int64_t v);


private:
    sdk::Currency _currency;
    friend class QuartelData;

    QDate _filing_date;

    int64_t  to (int32_t d) const;
    int32_t from(int64_t d) const;

    int32_t _investing_net_investments;
    int32_t _operating_change_liabilities;
    int32_t _investing_total;
    int32_t _operating_other_adjustments;
    int32_t _financing_equity_issuance;
    int32_t _investing_other;
    int32_t _financing_dividends_paid;
    int32_t _operating_change_inventory;
    int32_t _financing_buyback_net;
    int32_t _financing_other;
    int32_t _operating_net_income_adjust;
    int32_t _investing_capital_spending;
    int32_t _operating_change_receivables;
    int32_t _operating_other;
    int32_t _operating_noncash_other;
    int32_t _free_cash;
    int32_t _financing_net_borrowings;
    int32_t _net_income;
    int32_t _operating_total;
    int32_t _depreciation;
    int32_t _stock_based_compensation;
};

#endif // SDK_INSTRUMENT_CASHFLOW_H
