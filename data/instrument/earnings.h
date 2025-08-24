#ifndef DATA_INSTRUMENT_EARNINGS_H
#define DATA_INSTRUMENT_EARNINGS_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    class Instrument;
    class Earnings;
}

/*!
 * \class data::Earnings
 * \brief Результати (TTM/квартал) та темпи зростання.
 *
 * \property data::Earnings::revenue_ttm
 * Виручка за останні 12 місяців. Ключ для оцінки масштабу бізнесу та мультиплікаторів на продажі.
 *
 * \property data::Earnings::revenue_per_share_ttm
 * Виручка на акцію (TTM). Зручно для порівняння між компаніями різного розміру.
 *
 * \property data::Earnings::ebitda
 * EBITDA (TTM). Порівнюється через EV/EBITDA; допомагає бачити операційну «прибуткову масу».
 *
 * \property data::Earnings::diluted_eps_ttm
 * Розбавлений EPS (TTM).
 * Враховує потенційні конвертовані інструменти; більш «консервативний» за базовий EPS.
 *
 * \property data::Earnings::rev_growth_yoy
 * Річні темпи зростання виручки (YoY). Індикація попиту, захоплення ринку, цінової динаміки.
 *
 * \property data::Earnings::earnings_growth_yoy
 * Річні темпи зростання прибутку (YoY). Чутливі до операційного важеля й одноразових статей.
 *
 * \property data::Earnings::most_recent_quarter
 * Дата останнього звітного кварталу (MRQ). Визначає «свіжість» TTM/квартальних чисел.
 */
class data::Earnings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double revenue_ttm           READ revenue_ttm           NOTIFY revenueTtmChanged)
    Q_PROPERTY(double revenue_per_share_ttm READ revenue_per_share_ttm NOTIFY revenuePSTtmChanged)
    Q_PROPERTY(double ebitda                READ ebitda                NOTIFY ebitdaChanged)
    Q_PROPERTY(float  diluted_eps_ttm       READ diluted_eps_ttm       NOTIFY dilutedEpsTtmChanged)
    Q_PROPERTY(float  rev_growth_yoy        READ rev_growth_yoy        NOTIFY revGrowthYoyChanged)
    Q_PROPERTY(float  earnings_growth_yoy   READ earnings_growth_yoy   NOTIFY earnGrowthYoyChanged)
    Q_PROPERTY(QDate  most_recent_quarter   READ most_recent_quarter   NOTIFY mostRecentQChanged)
public:
    double revenue_ttm() const;
    double revenue_per_share_ttm() const;
    double ebitda() const;
    float diluted_eps_ttm() const;
    float rev_growth_yoy() const;
    float earnings_growth_yoy() const;
    QDate most_recent_quarter() const;

    void setRevenueTtm(double newRevenue_ttm);
    void setRevenuePerShareTtm(double newRevenue_per_share_ttm);
    void setEbitda(double newEbitda);
    void setDilutedEpsTtm(float newDiluted_eps_ttm);
    void setRevGrowthYoy(float newRev_growth_yoy);
    void setEarningsGrowthYoy(float newEarnings_growth_yoy);
    void setMostRecentQuarter(const QDate& newMost_recent_quarter);

signals:
    void revenueTtmChanged(double revenue_ttm);
    void revenuePSTtmChanged(double revenue_per_share_ttm);
    void ebitdaChanged(double ebitda);
    void dilutedEpsTtmChanged(float diluted_eps_ttm);
    void revGrowthYoyChanged(float rev_growth_yoy);
    void earnGrowthYoyChanged(float earnings_growth_yoy);
    void mostRecentQChanged(QDate most_recent_quarter);

private:
    Earnings(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    double _revenue_ttm;
    double _revenue_per_share_ttm;
    double _ebitda;
    float _diluted_eps_ttm;
    float _rev_growth_yoy;
    float _earnings_growth_yoy;
    QDate _most_recent_quarter;

    friend QDataStream& operator << (QDataStream& s, const Earnings& d);
    friend QDataStream& operator >> (QDataStream& s,       Earnings& d);
};

#endif // DATA_INSTRUMENT_EARNINGS_H
