#ifndef DATA_TICKER_QUOTES_H
#define DATA_TICKER_QUOTES_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    struct QuotesDate;
    struct QuotesTime;
    class  Quotes;
}

struct data::QuotesDate {
    QDate date;
    float open;
    float close;
    float high;
    float low;
    quint64  volume;

    friend QDataStream& operator << (QDataStream& s, const QuotesDate& q);
    friend QDataStream& operator >> (QDataStream& s,       QuotesDate& q);
};

struct data::QuotesTime {
    QTime time;
    float open;
    float close;
    float high;
    float low;
    quint64  volume;

    friend QDataStream& operator << (QDataStream& s, const QuotesTime& q);
    friend QDataStream& operator >> (QDataStream& s,       QuotesTime& q);
};

class data::Quotes : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector <QObject*> points READ points NOTIFY pointsChanged)
public:
    Quotes(QObject* parent = nullptr);
    Quotes& operator = (const Quotes& other);

    const std::vector <QuotesDate>& raw_points() const;
    QVector <QObject*> points();

    void recalculate();
    void set_data(QDate d, float open, float close, float high, float low, quint64  v);
    void set_data(QTime t, float open, float close, float high, float low, quint64  v);
    void set_intraday(QDate date);

    Q_INVOKABLE float year_max() const;
    Q_INVOKABLE float year_min() const;

    Q_INVOKABLE float current() const;
    Q_INVOKABLE bool  empty();

signals:
    void pointsChanged();

private:
    QDate _last_intraday;
    std::vector <QuotesTime> _intraday;
    std::vector <QuotesDate> _points;

    QVector <QObject*> _result;

    friend QDataStream& operator << (QDataStream& s, const Quotes& q);
    friend QDataStream& operator >> (QDataStream& s,       Quotes& q);
};

#endif
