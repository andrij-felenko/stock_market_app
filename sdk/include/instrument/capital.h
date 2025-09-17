#ifndef SDK_INSTRUMENT_CAPITAL_H
#define SDK_INSTRUMENT_CAPITAL_H

#include "sdk.h" // IWYU pragma: keep

class sdk::Capital : Trackable
{
public:
    Capital();

    uint64_t sharesOutstanding() const { return _shares_outstanding; }
    FieldTOpt setSharesOutstanding(int64_t value)
    { return sdk::set_if(this, _shares_outstanding, value, sdk::Cap_shares_outstand); }

    uint64_t sharesFloat() const { return _shares_float; }
    FieldTOpt setSharesFloat(int64_t value)
    { return sdk::set_if(this, _shares_float, value, sdk::Cap_shares_float); }

    double percentOfInsiders() const { return _percent_of_insiders; }
    FieldTOpt setPercentOfInsiders(double perc)
    { return sdk::set_if(this, _percent_of_insiders, perc, sdk::Cap_percent_of_insiders, 1e-12); }

    double percentOfInstitution() const { return _percent_institution; }
    FieldTOpt setPercentOfInsitution(double perc)
    { return sdk::set_if(this, _percent_institution, perc, sdk::Cap_percent_institution, 1e-12); }

    int64_t outstandShare(uint16_t year, sdk::Quartel quart = sdk::Quartel::Annual){
        for (const auto& it : _outstand_shares)
            if (it.year == year && it.quartel == quart)
                return it.shares;
        return -1;
    }

    int16_t outstandShare(QDate date)
    { return outstandShare(date.year(), sdk::quartel::from_month(date.month())); }

    FieldTOpt setOutstandShare(uint64_t shares, uint16_t year,
                               sdk::Quartel quart = sdk::Quartel::Annual){
        for (auto& it : _outstand_shares)
            if (it.year == year && quart == it.quartel){
                if (shares == it.shares)
                    return std::nullopt;
                it.shares = shares;
                _last_updated = QDateTime::currentDateTime();
                return sdk::Cap_outstand_shares;
            }
        _outstand_shares.emplace_back(year, quart, shares);
        _last_updated = QDateTime::currentDateTime();
        return sdk::Cap_outstand_shares;
    }
    FieldTOpt setOutstandShare(uint64_t shares, QDate date)
    { return setOutstandShare(shares, date.year(), sdk::quartel::from_month(date.month())); }

private:
    int64_t _shares_outstanding = -1;
    int64_t _shares_float = -1;
    double _percent_of_insiders = 0.0;
    double _percent_institution = 0.0;

    struct SharesPoint {
        uint16_t year;
        sdk::Quartel quartel;
        int64_t shares;

        bool operator == (const SharesPoint& o) const noexcept
        { return year == o.year && quartel == o.quartel && shares == o.shares; }
    };
    static inline bool lessByYearQuartel(const SharesPoint& a,
                                         const SharesPoint& b) {
        if (a.year != b.year) return a.year < b.year;
        return +a.quartel < +b.quartel;
    }
    std::vector <SharesPoint> _outstand_shares;

    friend QDataStream& operator << (QDataStream& s, const Capital& d);
    friend QDataStream& operator >> (QDataStream& s,       Capital& d);

    friend QDataStream& operator << (QDataStream& s, const SharesPoint& d);
    friend QDataStream& operator >> (QDataStream& s,       SharesPoint& d);
};

/**
 * @brief Власники компанії: інституції та фонди.
 * Зберігаємо списки записів із датами, підтримуємо відбір за датою,
 * топ-N, сумарні частки та індекси концентрації.
 */
/**
 * @brief Запис про власника (інституція або фонд).
 * percentOfShares — частка від усіх акцій компанії (0..1).
 * percentOfAssets — частка цього активу у портфелі фонду (0..1).
 * currentShares — поточна кількість акцій у власності.
 * changeShares — зміна кількості акцій за період (може бути від'ємною).
 * changeFraction — зміна частки в частках (0..1), позитивна/негативна.
 */
// struct OwnershipRecord {
//     QString holderName;     // "Vanguard Group Inc"
//     QDate   reportDate;     // "2025-06-30"
//     double  percentOfShares; // частка від outstanding: 0..1  (див. нормалізацію нижче)
//     double  percentOfAssets; // частка цього активу в портфелі фонду: 0..1 (за наявності)
//     quint64 currentShares;   // 162544006
//     qint64  changeShares;    // 1461575 (може бути від’ємним)
//     double  changeFraction;  // 0..1 (перетворити з %)
// };


/**
@brief Журнал угод інсайдерів та публічних осіб.
Ми зберігаємо дату угоди, тип операції, кількість акцій та ціну (якщо відома).
Це допомагає бачити покупки/продажі керівництва та інших пов'язаних осіб.
*/
// TODO: базова модель однієї транзакції
// struct InsiderTransactionRecord {
//     QDate      reportDate;     // "date" — дата публікації/репортингу
//     QDate      transactionDate;// "transactionDate" — фактична дата угоди
//     QString    ownerName;      // "ownerName"
//     std::optional<QString> ownerCik; // "ownerCik"
//     enum class TransactionCode { // "transactionCode" (SEC Form 4)
//         A, B, C, D, F, G, I, K, M, P, S, U, W, X, Unknown
//     } code;
//     qint64     sharesAmount;   // "transactionAmount" (шт.), null → 0/optional
//     double     transactionPrice;// "transactionPrice" (за акцію), може бути NaN
//     enum class AcqDisp { Acquired, Disposed, Unknown } acqDisp; // "A"/"D"
//     std::optional<qint64> postTransactionShares; // "postTransactionAmount"
//     std::optional<QUrl>   secLink;               // "secLink" (може бути порожнім)

//     // опційно: sourceType = { SecForm4, PoliticianDisclosure, Unknown }
// };

#endif // SDK_INSTRUMENT_CAPITAL_H
