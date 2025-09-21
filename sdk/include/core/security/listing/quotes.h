#ifndef SDK_INSTRUMENT_QUOTES_H
#define SDK_INSTRUMENT_QUOTES_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include "sdk_def.h"
#include "common/features.h"

struct sdk::QuotesDate {
    QDate date;
    float open;
    float close;
    float high;
    float low;
    quint64  volume;

    friend QDataStream& operator << (QDataStream& s, const QuotesDate& q);
    friend QDataStream& operator >> (QDataStream& s,       QuotesDate& q);
};

struct sdk::QuotesTime {
    QTime time;
    float open;
    float close;
    float high;
    float low;
    quint64  volume;

    friend QDataStream& operator << (QDataStream& s, const QuotesTime& q);
    friend QDataStream& operator >> (QDataStream& s,       QuotesTime& q);
};

/*!
 * \class data::Quotes
 * \brief Цінові ряди (щоденні та внутрішньоденні) для побудови графіків/індикаторів.
 *
 * \property data::Quotes::points
 * Нормалізований набір точок (QObjects) для QML-графіків: дата/час, open/high/low/close, обсяг.
 * Служить візуалізації (свічки/лінія), пошуку екстремумів (year_min/year_max) та поточної ціни.
 *
 * \note \c set_intraday() переключає джерело на внутрішньоденні дані за обраною датою.
 */
class sdk::Quotes : Trackable
{
public:
    Quotes();
    Quotes& operator = (const Quotes& other);

    const std::vector <QuotesDate>& raw_points() const;

    void setData(QDate d, float open, float close, float high, float low, quint64  v);
    void setData(QTime t, float open, float close, float high, float low, quint64  v);
    void setIntraday(QDate date);

    // TODO 52WeekHigh — максимум ціни за 52 тижні (розрахунок із історії котирувань).
    double fiftyTwoWeekHigh() const;

    // TODO 52WeekLow — мінімум ціни за 52 тижні (розрахунок із історії котирувань).
    double fiftyTwoWeekLow() const;

    // TODO 50DayMA — середня ціна за останні ~50 торгових днів.
    double movingAverage50Day() const;

    // TODO 200DayMA — середня ціна за останні ~200 торгових днів.
    double movingAverage200Day() const;

    float yearMax() const;
    float yearMin() const;

    float current() const;
    bool  empty();

    double beta() const { return _beta; }
    FieldTOpt setBeta(double value)
    { return sdk::set_if(this, _beta, value, sdk::Quotes_beta); }

private:
    double _beta = 0.0;
    QDate _last_intraday;
    sdk::List <QuotesTime, Quotes> _intraday;
    sdk::List <QuotesDate, Quotes> _points;

    friend QDataStream& operator << (QDataStream& s, const Quotes& q);
    friend QDataStream& operator >> (QDataStream& s,       Quotes& q);
};

#endif // SDK_INSTRUMENT_QUOTES_H
