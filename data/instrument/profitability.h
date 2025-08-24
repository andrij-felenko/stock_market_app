#ifndef DATA_TICKER_PROFITABILITY_H
#define DATA_TICKER_PROFITABILITY_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    class Instrument;
    class Profitability;
}

/*!
 * \class data::Profitability
 * \brief Рентабельність, ефективність використання капіталу та динаміка результатів.
 *
 * \property data::Profitability::roe
 * ROE — прибутковість власного капіталу: Net Income / Equity. Показує віддачу на кошти акціонерів.
 * Високий ROE може бути наслідком як ефективності, так і високого левереджу.
 *
 * \property data::Profitability::roa
 * ROA — прибутковість активів: Net Income / Total Assets.
 * Вимірює ефективність бізнес-моделі незалежно від структури капіталу.
 * Нижчий за ROE природно через левередж.
 *
 * \property data::Profitability::margin_gros
 * Валовий маржинальний рівень: (Revenue - COGS) / Revenue. Відображає цінову сила/собівартість.
 * Сильні бренди/уникальні технології — зазвичай високий gross margin.
 *
 * \property data::Profitability::margin_oper
 * Операційна маржа: Operating Income / Revenue.
 * Показує ефективність операцій після витрат на збут/адмін/R&D.
 *
 * \property data::Profitability::net_income
 * Чистий прибуток (Net Income). Базова «дна» P&L,
 * включає ефекти податків, відсотків, одноразових статей.
 *
 * \property data::Profitability::revenue_ttm
 * Виручка за останні 12 міс. Масштаб бізнесу. База для P/S, EV/Sales, маржинальності TTM.
 *
 * \property data::Profitability::gross_profit_ttm
 * Валова прибутковість у грошах (TTM).
 * Корисна для оцінки «валої» рентабельності в абсолюті (напр., для покриття OPEX).
 *
 * \property data::Profitability::revenue_per_share_ttm
 * Виручка на акцію (TTM). Нормалізує масштаб під капіталізацію;
 * зручно для порівняння на акцію між компаніями.
 *
 * \property data::Profitability::profit_margin
 * Чиста маржа: Net Income / Revenue. Узагальнює вплив операцій/податків/відсотків.
 * Висока маржа — цінова сила або низькі витрати.
 *
 * \property data::Profitability::operating_margin_ttm
 * Operating Margin (TTM). Сгладжує сезонність. Дає «середній» рівень операційної ефективності.
 *
 * \property data::Profitability::eps_ttm
 * EPS (TTM) — прибуток на акцію за 12 міс. Базовий показник для P/E,
 * динаміки прибутковості для акціонерів.
 *
 * \property data::Profitability::eps_estimate_year
 * Оцінка EPS на поточний рік (консенсус). Дає уявлення про очікування ринку;
 * джерело для Forward P/E.
 *
 * \property data::Profitability::eps_estimate_next_year
 * Оцінка EPS на наступний рік. Важлива для темпів зростання та оцінки «на перспективу».
 *
 * \property data::Profitability::peg_ratio
 * PEG = (P/E) / Growth. Інтерпретують як «ціна за одиницю зростання». PEG ≈ 1 умовно «адекватно»,
 * <1 — потенційно недооцінено (якщо якість прибутку добра).
 *
 * \property data::Profitability::target_price
 * Консенсус-ціль від аналітиків. Синтез прогнозів по зростанню/ризиках.
 * Варто сприймати як довідковий орієнтир, не гарантію.
 *
 * \property data::Profitability::most_recent_quarter
 * Дата останньої звітної квартальної публікації (MRQ).
 * Ключ для контексту будь-яких «MRQ/TTM» метрик.
 *
 * \property data::Profitability::q_rev_growth_yoy
 * Річний ріст квартальної виручки (YoY). Показує динаміку попиту/масштабування.
 * Чутливий до сезонності.
 *
 * \property data::Profitability::q_eps_growth_yoy
 * Річний ріст квартального EPS (YoY).
 * Інкапсулює одноразові ефекти; інтерпретація потребує якості earnings.
 *
 * \property data::Profitability::ebitda
 * EBITDA — прибуток до відсотків, податків, амортизації.
 * Оцінює операційну грошеподібність бізнесу, база для EV/EBITDA,
 * але ігнорує капітальні витрати (CapEx).
 */
class data::Profitability : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float roe         READ roe         NOTIFY roeChanged)
    Q_PROPERTY(float roa         READ roa         NOTIFY roaChanged)
    Q_PROPERTY(float net_income  READ net_income  NOTIFY  netincomeChanged)
    Q_PROPERTY(float margin_gros READ margin_gros NOTIFY marginGrosChanged)
    Q_PROPERTY(float margin_oper READ margin_oper NOTIFY marginOperChanged)
    Q_PROPERTY(double revenue_ttm           READ revenue_ttm           NOTIFY revenueTtmChanged)
    Q_PROPERTY(double gross_profit_ttm      READ gross_profit_ttm      NOTIFY grosProfitTtmChanged)
    Q_PROPERTY(float  revenue_per_share_ttm READ revenue_per_share_ttm NOTIFY revenuePSTtmChanged)
    Q_PROPERTY(float  profit_margin         READ profit_margin         NOTIFY profitMarginChanged)
    Q_PROPERTY(float  operating_margin_ttm  READ operating_margin_ttm  NOTIFY operMargTtmChanged)
    Q_PROPERTY(float  eps_ttm               READ eps_ttm               NOTIFY epsTtmChanged)
    Q_PROPERTY(float  eps_estimate_year     READ eps_estimate_year     NOTIFY epsEstYearChanged)
    Q_PROPERTY(float  eps_estimate_nextyear READ eps_estimate_nextyear NOTIFY epsEstNYChanged)
    Q_PROPERTY(float  peg_ratio             READ peg_ratio             NOTIFY pegRatioChanged)
    Q_PROPERTY(float  target_price          READ target_price          NOTIFY targetPriceChanged)
    Q_PROPERTY(QDate  most_recent_quarter   READ most_recent_quarter   NOTIFY mostRecentQChanged)
    Q_PROPERTY(float  q_rev_growth_yoy      READ q_rev_growth_yoy      NOTIFY qRevGrowYoyChanged)
    Q_PROPERTY(float  q_eps_growth_yoy      READ q_eps_growth_yoy      NOTIFY qEpsGrowYoyChanged)
    Q_PROPERTY(double ebitda                READ ebitda                NOTIFY ebitdaChanged)
public:
    float roe() const;
    float roa() const;
    float margin_gros() const;
    float margin_oper() const;
    float net_income() const;
    double revenue_ttm() const;
    double gross_profit_ttm() const;
    float revenue_per_share_ttm() const;
    float profit_margin() const;
    float operating_margin_ttm() const;
    float eps_ttm() const;
    float eps_estimate_year() const;
    float eps_estimate_nextyear() const;
    float peg_ratio() const;
    float target_price() const;
    QDate most_recent_quarter() const;
    float q_rev_growth_yoy() const;
    float q_eps_growth_yoy() const;
    double ebitda() const;

    void setRoe(float newRoe);
    void setRoa(float newRoa);
    void setMarginGros(float new_gross_margin);
    void setMarginOper(float new_operating_margin);
    void setNetIncome(float new_net_income);
    void setRevenueTtm(double newRevenue_ttm);
    void setGrossProfitTtm(double newGross_profit_ttm);
    void setRevenuePerShareTtm(float newRevenue_per_share_ttm);
    void setProfitMargin(float newProfit_margin);
    void setOperatingMarginTtm(float newOperating_margin_ttm);
    void setEpsTtm(float newEps_ttm);
    void setEpsEstimateYear(float newEps_estimate_year);
    void setEpsEstimateNextYear(float newEps_estimate_next_year);
    void setPegRatio(float newPeg_ratio);
    void setTargetPrice(float newTarget_price);
    void setMostRecentQuarter(const QDate& newMost_recent_quarter);
    void setQrevGrowthYoy(float newQ_rev_growth_yoy);
    void setQepsGrowthYoy(float newQ_eps_growth_yoy);
    void setEbitda(double newEbitda);

signals:
    void roeChanged(float roe);
    void roaChanged(float roa);
    void marginGrosChanged(float gross_margin);
    void marginOperChanged(float operating_margin);
    void netincomeChanged(float netincome);
    void revenueTtmChanged(double revenue_ttm);
    void grosProfitTtmChanged(double gross_profit_ttm);
    void revenuePSTtmChanged(float revenue_per_share_ttm);
    void profitMarginChanged(float profit_margin);
    void operMargTtmChanged(float operating_margin_ttm);
    void epsTtmChanged(float eps_ttm);
    void epsEstYearChanged(float eps_estimate_year);
    void epsEstNYChanged(float eps_estimate_next_year);
    void pegRatioChanged(float peg_ratio);
    void targetPriceChanged(float target_price);
    void mostRecentQChanged(QDate most_recent_quarter);
    void qRevGrowYoyChanged(float q_rev_growth_yoy);
    void qEpsGrowYoyChanged(float q_eps_growth_yoy);
    void ebitdaChanged(double ebitda);

private:
    Profitability(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    float _roe;
    float _roa;
    float _margin_gros;
    float _margin_oper;
    float _netincome;
    double _revenue_ttm;
    double _gross_profit_ttm;
    float _revenue_per_share_ttm;
    float _profit_margin;
    float _operating_margin_ttm;
    float _eps_ttm;
    float _eps_estimate_year;
    float _eps_estimate_next_year;
    float _peg_ratio;
    float _target_price;
    QDate _most_recent_quarter;
    float _q_rev_growth_yoy;
    float _q_eps_growth_yoy;
    double _ebitda;

    friend QDataStream& operator << (QDataStream& s, const Profitability& p);
    friend QDataStream& operator >> (QDataStream& s,       Profitability& p);
};

#endif
