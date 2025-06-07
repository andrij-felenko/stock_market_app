#ifndef DATA_TICKER_STABILITY_H
#define DATA_TICKER_STABILITY_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data::ticker { class Stability; }

class data::ticker::Stability : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float debt_equity READ debt_equity WRITE set_debt_equity NOTIFY debt_equityChanged)
    Q_PROPERTY(float revenue     READ revenue     WRITE set_revenue     NOTIFY revenueChanged)
    Q_PROPERTY(float beta        READ beta        WRITE set_beta        NOTIFY betaChanged)
public:
    Stability(QObject* parent = nullptr);

    float debt_equity() const;
    float revenue() const;
    float beta() const;

public slots:
    void set_debt_equity(float new_debt_equity);
    void set_revenue(float new_revenue);
    void set_beta(float new_beta);

signals:
    void debt_equityChanged(float debt_equity);
    void revenueChanged(float revenue);
    void betaChanged(float beta);

private:
    float _debt_equity;
    float _revenue;
    float _beta;

    friend QDataStream& operator << (QDataStream& s, const Stability& d);
    friend QDataStream& operator >> (QDataStream& s,       Stability& d);
};

#endif
