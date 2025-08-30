#ifndef DATA_TICKER_STABILITY_H
#define DATA_TICKER_STABILITY_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    class Instrument;
    class Stability;
}

/*!
 * \class data::Stability
 * \brief Профіль ризику та капітальна структура (левередж, обігові кошти).
 *
 * \property data::Stability::beta
 * Бета-коефіцієнт — чутливість акції до ринкового індексу (систематичний ризик).
 * β > 1 — вища волатильність,
 * β < 1 — захисні папери. Важливий для моделі CAPM та оцінки вартості капіталу.
 *
 * \property data::Stability::debt_equity
 * Співвідношення боргу до власного капіталу (D/E).
 * Показує фінансовий левередж; високе значення підвищує чутливість до процентних ставок та циклу.
 *
 * \property data::Stability::net_debt
 * Чистий борг: Total Debt - Cash & Short-term Investments.
 * Краще відображає «борговий тягар» після врахування готівки. Використовується в EV.
 *
 * \property data::Stability::short_debt
 * Короткостроковий борг (до 1 року). Важливий для ліквідності та рефінансування.
 *
 * \property data::Stability::long_debt
 * Довгостроковий борг. Структурний левередж, залежить від інвестпрограм та політики капіталу.
 *
 * \property data::Stability::net_working_cap
 * Чистий оборотний капітал: Оборотні активи - Поточні зобов’язання.
 * Показує запас ліквідності для операційного циклу;
 * негативний NWC підвищує ризики касових розривів.
 */
class data::Stability : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float  beta            READ beta            NOTIFY betaChanged)
    Q_PROPERTY(float  debt_equity     READ debt_equity     NOTIFY debt_equityChanged)
    Q_PROPERTY(double net_debt        READ net_debt        NOTIFY netDebtChanged)
    Q_PROPERTY(double short_debt      READ short_debt      NOTIFY shortDebtChanged)
    Q_PROPERTY(double long_debt       READ long_debt       NOTIFY longDebtChanged)
    Q_PROPERTY(double net_working_cap READ net_working_cap NOTIFY netWorkingCapChanged)
public:
    float debt_equity() const;
    float beta() const;
    double net_debt() const;
    double short_debt() const;
    double long_debt() const;
    double net_working_cap() const;

public slots:
    void setDebtEquity(float new_debt_equity);
    void setBeta(float new_beta);
    void setNetDebt(double newNet_debt);
    void setShortDebt(double newShort_debt);
    void setLongDebt(double newLong_debt);
    void setNetWorkingCap(double newNet_working_cap);

signals:
    void debt_equityChanged(float debt_equity);
    void betaChanged(float beta);
    void netDebtChanged(double net_debt);
    void shortDebtChanged(double short_debt);
    void longDebtChanged(double long_debt);
    void netWorkingCapChanged(double net_working_cap);

private:
    Stability(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    float _debt_equity;
    float _beta;
    double _net_debt;
    double _short_debt;
    double _long_debt;
    double _net_working_cap;

    friend QDataStream& operator << (QDataStream& s, const Stability& d);
    friend QDataStream& operator >> (QDataStream& s,       Stability& d);
};

#endif
