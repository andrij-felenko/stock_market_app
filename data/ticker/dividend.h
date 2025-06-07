#ifndef DATA_TICKER_DIVIDEND_H
#define DATA_TICKER_DIVIDEND_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QDataStream>

namespace data::ticker { class Dividend; }

class data::ticker::Dividend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float yield     READ yield     WRITE set_yield     NOTIFY     yieldChanged)
    Q_PROPERTY(float per_share READ per_share WRITE set_per_share NOTIFY per_shareChanged)
    Q_PROPERTY(float pay_ratio READ pay_ratio WRITE set_pay_ratio NOTIFY pay_ratioChanged)
    Q_PROPERTY(QDate next_date READ next_date WRITE set_next_date NOTIFY next_dateChanged)
    Q_PROPERTY(QDate prev_date READ prev_date WRITE set_prev_date NOTIFY prev_dateChanged)
public:
    Dividend(QObject* parent = nullptr);

    float yield()     const;
    float per_share() const;
    float pay_ratio() const;
    QDate next_date() const;
    QDate prev_date() const;

public slots:
    void set_yield    (float new_yield);
    void set_per_share(float new_per_share);
    void set_pay_ratio(float new_payout_ratio);
    void set_next_date(const QDate& new_next_date);
    void set_prev_date(const QDate& new_prev_date);

signals:
    void     yieldChanged(float yield);
    void per_shareChanged(float per_share);
    void pay_ratioChanged(float payout_ratio);
    void next_dateChanged(QDate next_date);
    void prev_dateChanged(QDate prev_date);

private:
    float _yield = 0;
    float _per_share = 0;
    float _pay_ratio = 0;
    QDate _next_date;
    QDate _prev_date;

    std::vector <QDate> _history;

    friend QDataStream& operator << (QDataStream& s, const Dividend& d);
    friend QDataStream& operator >> (QDataStream& s,       Dividend& d);
};

#endif
