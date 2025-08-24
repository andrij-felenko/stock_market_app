#ifndef DATA_TICKER_VALUATION_H
#define DATA_TICKER_VALUATION_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    class Instrument;
    class Valuation;
}

/*!
 * \class data::Valuation
 * \brief Блок ринкової оцінки компанії: капіталізація, мультиплікатори (P/E, P/B, P/S),
 * EV та похідні.
 *
 * \property data::Valuation::shares_outstanding
 * Кількість акцій в обігу (Outstanding). Використовується для розрахунку ринкової капіталізації
 * (Market Cap = Shares Outstanding × Поточна ціна) та показників «на акцію». Зміни можливі через
 * buyback/емісії. Високий free float покращує ліквідність.
 *
 * \property data::Valuation::book_value_ps_mrq
 * Балансова вартість на акцію (BVPS) за останній звітний квартал (MRQ). Приблизно дорівнює
 * власному капіталу / акції. Застосовується в P/B. Важливо розуміти, що BVPS гірше відображає
 * нематеріальні активи (R&D, бренд).
 *
 * \property data::Valuation::market_capitalization
 * Ринкова капіталізація. Оцінка ринку власного капіталу компанії: ціна × кількість акцій.
 * Не враховує борг/готівку (тому для злиттів/поглинань частіше дивляться EV).
 *
 * \property data::Valuation::enterprise_value
 * Enterprise Value (EV) — «вартість підприємства» для інвестора з урахуванням структури капіталу:
 * EV = Market Cap + Net Debt (або: Total Debt + Minority Interest + Preferred - Cash).
 * Краще підходить для порівнянь між компаніями з різним левереджем.
 *
 * \property data::Valuation::earnings_per_share_ttm
 * Прибуток на акцію за останні 12 міс. (EPS TTM). Базовий компонент для P/E. Негативний EPS робить
 * P/E неінтерпретованим; у циклічних галузях волатильний.
 *
 * \property data::Valuation::price_to_earnings_ttm
 * P/E (Trailing) — ціна до прибутку за TTM: Price / EPS_TTM. Швидкий індикатор «скільки ринок
 * платить за 1 одиницю прибутку». Працює гірше при нуль/негативному прибутку або різкій фазі циклу
 *
 * \property data::Valuation::price_to_earnings_fwd
 * Forward P/E — ціна до очікуваного прибутку (на наступні 12 міс.).
 * Покладається на прогнози аналітиків;
 * чутливий до переглядів консенсусу.
 *
 * \property data::Valuation::price_to_book_mrq
 * P/B — ціна до балансової вартості (MRQ).
 * Має сенс у капіталомістких бізнесах (банки, страхування).
 * У «легких» бізнесах (софт) часто занижує реальну цінність.
 *
 * \property data::Valuation::price_to_sales_ttm
 * P/S — ціна до виручки (TTM). Корисний, коли прибутку немає або він спотворений.
 * Порівнює ціну до масштабу бізнесу, але ігнорує маржинальність.
 *
 * \property data::Valuation::ev_to_sales_ttm
 * EV/Sales (TTM) — EV до виручки. Краще за P/S, бо враховує борг/готівку.
 * Застосовується для «ранніх» компаній або циклічних періодів зі слабким прибутком.
 *
 * \property data::Valuation::ev_to_ebitda
 * EV/EBITDA — один із ключових мультиплікаторів порівняльної оцінки (M&A, телекоми, промисловість)
 * Менше чутливий до структури капіталу/податків,
 * але EBITDA ігнорує капзатрати та зміни в оборотному капіталі.
 */
class data::Valuation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint64_t shares_outstanding   READ shares_outstanding     NOTIFY shares_outChanged)
    Q_PROPERTY(double book_value_ps_mrq      READ book_value_ps_mrq      NOTIFY bvps_mrqChanged)
    Q_PROPERTY(double market_capitalization  READ market_capitalization  NOTIFY market_capChanged)
    Q_PROPERTY(double enterprise_value       READ enterprise_value       NOTIFY evChanged)
    Q_PROPERTY(double earnings_per_share_ttm READ earnings_per_share_ttm NOTIFY eps_ttmChanged)

    Q_PROPERTY(float price_to_earnings_ttm READ price_to_earnings_ttm NOTIFY pe_ttmChanged)
    Q_PROPERTY(float price_to_earnings_fwd READ price_to_earnings_fwd NOTIFY pe_fwdChanged)
    Q_PROPERTY(float price_to_book_mrq     READ price_to_book_mrq     NOTIFY pb_mrqChanged)
    Q_PROPERTY(float price_to_sales_ttm    READ price_to_sales_ttm    NOTIFY ps_ttmChanged)
    Q_PROPERTY(float ev_to_sales_ttm       READ ev_to_sales_ttm       NOTIFY ev_sales_ttmChanged)
    Q_PROPERTY(float ev_to_ebitda          READ ev_to_ebitda          NOTIFY ev_ebitdaChanged)
public:
    uint64_t shares_outstanding() const;
    double book_value_ps_mrq() const;
    double market_capitalization() const;
    double enterprise_value() const;
    double earnings_per_share_ttm() const;
    float price_to_earnings_ttm() const;
    float price_to_earnings_fwd() const;
    float price_to_book_mrq() const;
    float price_to_sales_ttm() const;
    float ev_to_sales_ttm() const;
    float ev_to_ebitda() const;

    void setSharesOutstanding(uint64_t v);
    void setBookValuePsMrq(double v);
    void setMarketCapitalization(double v);
    void setEnterpriseValue(double v);
    void setEarningsPerShareTtm(double v);
    void setPriceToEarningsTtm(float v);
    void setPriceToEarningsFwd(float v);
    void setPriceToBookMrq(float v);
    void setPriceToSalesTtm(float v);
    void setEvToSalesTtm(float v);
    void setEvToEbitda(float v);

signals:
    void shares_outChanged(uint64_t shares_outstanding);
    void bvps_mrqChanged(double book_value_ps_mrq);
    void market_capChanged(double market_capitalization);
    void evChanged(double enterprise_value);
    void eps_ttmChanged(double earnings_per_share_ttm);
    void pe_ttmChanged(float price_to_earnings_ttm);
    void pe_fwdChanged(float price_to_earnings_fwd);
    void pb_mrqChanged(float price_to_book_mrq);
    void ps_ttmChanged(float price_to_sales_ttm);
    void ev_sales_ttmChanged(float ev_to_sales_ttm);
    void ev_ebitdaChanged(float ev_to_ebitda);

private:
    Valuation(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    uint64_t _shares_outstanding;
    double _book_value_ps_mrq;
    double _market_capitalization;
    double _enterprise_value;
    double _earnings_per_share_ttm;
    float _price_to_earnings_ttm;
    float _price_to_earnings_fwd;
    float _price_to_book_mrq;
    float _price_to_sales_ttm;
    float _ev_to_sales_ttm;
    float _ev_to_ebitda;

    friend QDataStream& operator << (QDataStream& s, const Valuation& d);
    friend QDataStream& operator >> (QDataStream& s,       Valuation& d);
};

#endif
