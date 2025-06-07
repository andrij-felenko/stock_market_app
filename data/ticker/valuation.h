#ifndef DATA_TICKER_VALUATION_H
#define DATA_TICKER_VALUATION_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data::ticker { class Valuation; }

class data::ticker::Valuation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float eps         READ eps         WRITE set_eps         NOTIFY         epsChanged)
    Q_PROPERTY(float pe_ratio    READ pe_ratio    WRITE set_pe_ratio    NOTIFY    pe_ratioChanged)
    Q_PROPERTY(float pb_ratio    READ pb_ratio    WRITE set_pb_ratio    NOTIFY    pb_ratioChanged)
    Q_PROPERTY(float book_value  READ book_value  WRITE set_book_value  NOTIFY  book_valueChanged)
    Q_PROPERTY(float market_cap  READ market_cap  WRITE set_market_cap  NOTIFY  market_capChanged)
    Q_PROPERTY(float share_count READ share_count WRITE set_share_count NOTIFY share_countChanged)
public:
    Valuation(QObject* parent = nullptr);

    float eps() const;
    float pe_ratio() const;
    float pb_ratio() const;
    float book_value() const;
    float market_cap() const;
    float share_count() const;

public slots:
    void set_eps(float new_eps);
    void set_pe_ratio(float new_pe_ratio);
    void set_pb_ratio(float new_pb_ratio);
    void set_book_value(float new_book_value);
    void set_market_cap(float new_market_cap);
    void set_share_count(float new_share_count);

signals:
    void epsChanged(float eps);
    void pe_ratioChanged(float pe_ratio);
    void pb_ratioChanged(float pb_ratio);
    void book_valueChanged(float book_value);
    void market_capChanged(float market_cap);
    void share_countChanged(float share_count);

private:
    float _eps;
    float _pe_ratio;
    float _pb_ratio;
    float _book_value;
    float _market_cap;
    float _share_count;

    friend QDataStream& operator << (QDataStream& s, const Valuation& d);
    friend QDataStream& operator >> (QDataStream& s,       Valuation& d);
};

#endif
