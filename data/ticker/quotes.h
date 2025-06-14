#ifndef DATA_TICKER_QUOTES_H
#define DATA_TICKER_QUOTES_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data::ticker {
    struct QuotesDate;
    struct QuotesTime;
    class  Quotes;
}

struct data::ticker::QuotesDate {
    QDate date;
    float open;
    float close;
    float high;
    float low;
    uint64_t volume;
};

struct data::ticker::QuotesTime {
    QTime date;
    float open;
    float close;
    float high;
    float low;
    uint64_t volume;
};

class data::ticker::Quotes : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector <QObject*> points READ points NOTIFY pointsChanged)
public:
    Quotes(QObject* parent = nullptr);

    const QVector <QuotesDate>& raw_points() const;
    QVector <QObject*> points();

    void recalculate();
    void set_data(QDate d, float open, float close, float high, float low, uint64_t v);

signals:
    void pointsChanged();

private:
    QVector <QuotesDate> _points;
    QVector <QObject*> _result;

    friend QDataStream& operator << (QDataStream& s, const Quotes& q);
    friend QDataStream& operator >> (QDataStream& s,       Quotes& q);
};

#endif
