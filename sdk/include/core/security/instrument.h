#ifndef SDK_INSTRUMENT_H
#define SDK_INSTRUMENT_H

/*!
\file finance_overview.dox
\brief Архітектура фінансового модуля (огляд + деталі)

\section overview Коротка структура

Instrument
├─ Listing = vector<Ticker> (per-ticker дані)
│  ├─ dividend
│  ├─ quotes
│  ├─ short_interest
│  ├─ corporate_action
│  └─ valuation              // перелікове, залежить від біржі/ціни
├─ Legal                     // юр. інформація: країна, форма, регулятор
├─ Meta                      // назва, опис, сектор, індустрія, регіон
└─ Finance
   ├─ QuartelData            // фасад доступу за періодами (view)
   │  ├─ Balance             // BS (періодні зрізи)
   │  ├─ CashFlow            // CF (періодні зрізи)
   │  ├─ Earnings            // точкові події (звіт/консенсус EPS)
   │  ├─ Incomes             // IS (періодні зрізи)
   │  └─ Trend               // консенсуси/ревізії/тренди
   ├─ Fundamental            // «сирі» звіти IS/BS/CF по періодах (джерело істини)
   ├─ Capital                // shares_outstanding, float, insider%, institutions%
   ├─ Estimates              // прогнози: EPS next, Revenue next, Target price, Rating...
   └─ ~ Derived              // ОКРЕМИЙ КЛАС ВІДСУТНІЙ: похідні функції живуть прямо у Finance

\section shortnotes Короткі пояснення

- \b Listing – все, що залежить від конкретного лістингу/біржі (ціни, дохідність, мультиплікатори).
- \b Legal/Meta – статичні атрибути емітента (company-level).
- \b Fundamental – збереження \e періодних звітів (IS/BS/CF) як канонічного джерела.
- \b QuartelData – зручний вью поверх Fundamental без дублювання даних.
- \b Capital – структура капіталу; використовується у розрахунках EPS, BVPS, NetDebt тощо.
- \b Estimates – сторонні прогнози/консенсуси для майбутніх періодів.
- \b Derived (у вигляді методів Finance) – TTM/MRQ/NetDebt/різні «per share» метрики рахуються на льоту.

\section details Детальна система роботи

\subsection ownership Володіння й межі відповідальності
- \b Company-level (спільно для всіх тикерів): Legal, Meta, Fundamental, Capital, Estimates.
- \b Per-ticker: Listing::valuation, quotes, dividend, short_interest, corporate_action.
- \b QuartelData – легкий фасад, що віддає посилання/копії значень із Fundamental; \e не зберігає свій стан.

\subsection periods Періодизація (Quartel/Year)
- Ключ періоду: \c (year, Quartel) або \c date (наприклад, \c 2025-06-30).
- \c filing_date зберігається для «актуальності» (коли саме подано).
- Колекції періодів у Fundamental мають \c ensure-доступ: \c cashFlow(year,q), \c incomes(year,q), \c balance(year,q).
- Принцип: \b один масштаб і валюта на \e серію (наприклад, CF-quarterly у мільйонах JPY).

\subsection storage Збереження чисел (пам’ять і точність)
- Внутрішній тип більшості сум: \b int32_t з \b серійним масштабом (fixed-point).
  Приклад: для виручки \c REV_SCALE = 1e6 (у мільйонах); для EPS \c EPS_SCALE = 1e3 (у тисячних).
- Публічні гетери повертають \b double/\b float (інлайн конвертери множать/ділять на \c scale).
- Лічильники (кількість аналітиків/ревізій): \b uint16_t.
- \note Масштаб серії не змінюється між періодами, щоб не «стрибали» значення.

\subsection currency Масштабування за валютою
- Для тоталів (Revenue/CF/BS) використовується \c sdk::currency::scale(cur) → {1e4, 1e5, 1e6, 1e7…} залежно від номіналу валюти.
- Для «на акцію» (EPS/DPS) – окремий \c EPS_SCALE (найчастіше 1e3).
- \warning Не накладайте «валютний мінор-юнит» (центи) на тотали – це швидко переповнить \c int32_t.

\subsection consistency Узгодження та контрольні рівняння
- \b Cash Flow:
  \code
  CFO + CFI + CFF + FX  ≈  cashEnd − cashBegin
  changeInCash          ≈  cashEnd − cashBegin
  \endcode
- \b Income Statement:
  \code
  grossProfit         = revenue − costOfRevenue
  operatingIncome     ≈ grossProfit − (R&D + SG&A + otherOpEx)
  incomeBeforeTax     ≈ operatingIncome + otherIncomeExpenseNet + netInterest
  netIncome           ≈ incomeBeforeTax − incomeTaxExpense
  \endcode
- \b Balance Sheet:
  \code
  workingCapital = currentAssets − currentLiabilities
  ΔworkingCapital = WC_t − WC_(t−1)
  \endcode
- \note Поля, які постачальник дає як «control total», можна зберігати для звірок, але в алгоритмах покладатися на складники.

\subsection derived Похідні показники (живуть у Finance як методи)
- \b EPS TTM: сумування \c netIncome_to_common чотирьох останніх кварталів / середня \c diluted_shares TTM.
- \b Sales per Share (TTM): \c revenue_TTM / \c diluted_shares_TTM.
- \b BVPS MRQ: \c (total_equity − preferred_equity) / \c diluted_shares_MRQ.
- \b NetDebt: \c total_debt − cash_and_equivalents (MRQ).
- \b Маржі: \c gross/operating/net margins = показник / revenue.
- \note Усі TTM/MRQ беруться із Fundamental/Capital та конвертуються масштабами серій; кешувати за потребою.

\subsection trend Тренди та консенсуси (Trend / Earnings / Estimates)
- \b Trend: зберігає EPS-тренд-рівні (current, 7d/30d/60d/90d ago), ревізії (up/down за 7/30 днів), консенсуси по EPS/Revenue.
- \b Earnings: подія звіту (дата, сесія \c before_market/after_market, epsActual/epsEstimate); \c epsDifference і \c surprisePercent — похідні.
- \b Estimates: майбутні періоди (EPS/Revenue next Q/Y, target price, rating), джерело – сторонні провайдери.

\subsection import Імпорт даних і мапінг
- Парсер провайдера пише у Fundamental/Capital/Estimates, застосовуючи:
  - вибір \c scale за валютою для тоталів;
  - \c EPS_SCALE для «на акцію»;
  - уніфікацію назв (наприклад, \c reconciledDepreciation → \c depreciation).
- Для «дивних» полів (на кшталт \c otherOperatingExpenses, які в деяких фідах включають COGS) – позначка \c {CAUTION} та виключення з формул.

\subsection facade QuartelData як фасад
- Надає короткі маршрути доступу: \c quartelData.cashFlow(year,q).operatingTotal().
- Під капотом бере значення з Fundamental; \b не дублює зберігання.
- \see Fundamental для повних наборів полів.

\subsection perf Продуктивність і пам’ять
- Внутрішній \c int32_t + scale мінімізує RAM та покращує кеш-локальність.
- Конвертери \c to/from інлайн, коштують кілька інструкцій (дешевше за промах кешу).
- При сумуванні кількох \c int32_t використовуйте \c int64_t акумулятори (щоб уникнути переповнення до конвертації в \c double).

\subsection invariants Інваріанти класів (приклади)
- \b CashFlow: \c cashBegin + changeInCash + fxEffect ≈ cashEnd.
- \b Incomes: \c grossProfit = revenue − costOfRevenue; \c EBITDA = operatingIncome + D&A.
- \b Balance: \c totalAssets ≈ totalLiabilities + totalEquity.
- Порушення інваріантів – тригер для діагностики імпорту.

\subsection examples Приклади використання

\subsubsection ex_access Доступ до періоду
\code
auto& cf = finance.fundamental().cashFlowEnsure(2025, Quartel::Q2);
auto  cfo = cf.operatingTotal();  // double/float у валютних одиницях серії
\endcode

\subsubsection ex_ttm Розрахунок TTM без дублювання
\code
double ttm_revenue = 0.0;
for (auto yq : last4Quarters()) {
    ttm_revenue += finance.fundamental().incomes(yq.year, yq.q).totalRevenue();
}
double sales_ps_ttm = ttm_revenue / finance.capital().dilutedSharesTTM();
\endcode

\subsection naming Стиль імен та масштабів
- Поля всередині класів: \c snake_case з префіксом області (\c _operating_total, \c _investing_capital_spending).
- Гетери/сетери: розбірливі \c camelCase без згадки «CashFlow/Income» у назвах, бо це контекст класу.
- Масштаби: \c EPS_SCALE для EPS, \c currency::scale() для тоталів; обидва документуються у коментарях полів.

\subsection safety Поради з безпеки даних
- Не використовувати «магічні нулі» для «немає значення» – користуйтесь \c Trackable/optional-логікою.
- Конвертери \c from* повинні насичувати до меж \c INT32_MIN/INT32_MAX.
- При зміні \c scale у майбутніх версіях – міграція даних обов’язкова.

*/

#include "sdk_def.h"
#include "core/security/data.h"
#include "core/isin.h"

namespace api { class Eod; }

class sdk::Instrument
{
public:
    Instrument(const Isin& isin, uint16_t index);

    Instrument(Instrument&&) noexcept;

    Data* const data() const;
    Data* create();
    void release();
    bool has_data() const;

    bool save() const;
    bool load();

    sdk::Instype type() const { return _type; }
    const Isin&   isin() const { return _isin; }
    const QString& name() const { return _name; }
    sdk::Country country() const;

    std::vector <sdk::Symbol> tickers() const;
    bool contains(const sdk::Symbol& symbol) const;

    Data* operator->() const noexcept { return data(); }

private:
    Instrument(uint16_t index); // only for Market
    Instrument(const Instrument&) = delete;
    Instrument& operator = (const Instrument&) = delete;
    Instrument& operator = (Instrument&&) noexcept = delete;

    union {
        std::vector <sdk::Symbol>* _tickers;
        Data* _data = nullptr;
    };
    void unload();

    QString _name;
    sdk::Instype _type;
    Isin _isin;

    Trackable _track;
    uint16_t _index = 0;
    std::atomic <uint32_t> _usages = 0; // 0 - bit is union flag

    void findBetterName(const QString& str);
    Ticker* findTicker(const Symbol& tag){
        if (has_data())
            for (auto& it : _data->tickers)
                if (it.symbol() == tag)
                    return &it;
        return nullptr;
    }

    Instrument& operator ++ () noexcept;
    Instrument& operator -- () noexcept;

    friend QDataStream& operator << (QDataStream& s, Wire <const Instrument> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Instrument> d);

    friend class sdk::Market;

    void _sortTickers();
    Ticker* _addTicker(const sdk::Symbol& symbol);
};

#endif // SDK_INSTRUMENT_H
