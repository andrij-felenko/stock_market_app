#ifndef DATA_INSTRUMENT_BALANCE_H
#define DATA_INSTRUMENT_BALANCE_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    class Balance;
    class Instrument;
}

/*!
 * \class data::Balance
 * \brief Ключові балансові агрегати для оцінки стійкості капіталу.
 *
 * \property data::Balance::total_debt
 * Загальна заборгованість (коротка + довга). База для показників левереджу (D/E, Net Debt/EBITDA),
 * оцінки відсоткових витрат та ризиків рефінансування.
 *
 * \property data::Balance::cash_sti
 * Готівка та короткострокові інвестиції (Cash & STI).
 * Ліквідна «подушка» для операцій та боргових виплат;
 * зменшує чистий борг, входить у розрахунок EV.
 *
 * \property data::Balance::equity
 * Власний капітал (Book Equity). База для ROE, P/B; відображає накопичений результат та додатковий
 * капітал акціонерів.
 */
class data::Balance : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double total_debt READ total_debt NOTIFY totalDebtChanged)
    Q_PROPERTY(double cash_sti   READ cash_sti   NOTIFY cashStiChanged) // cash & short-term invest
    Q_PROPERTY(double equity     READ equity     NOTIFY equityChanged)
public:
    double total_debt() const;
    double cash_sti() const;
    double equity() const;

    void setTotalDebt(double newTotal_debt);
    void setCashSti(double newCash_sti);
    void setEquity(double newEquity);

signals:
    void totalDebtChanged(double total_debt);
    void cashStiChanged(double cash_sti);
    void equityChanged(double equity);

private:
    Balance(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    double _total_debt;
    double _cash_sti;
    double _equity;

    friend QDataStream& operator << (QDataStream& s, const Balance& d);
    friend QDataStream& operator >> (QDataStream& s,       Balance& d);
};

#endif // DATA_INSTRUMENT_BALANCE_H
