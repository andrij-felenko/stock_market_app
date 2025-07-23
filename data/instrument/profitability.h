#ifndef DATA_TICKER_PROFITABILITY_H
#define DATA_TICKER_PROFITABILITY_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data { class Profitability; }

class data::Profitability : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float roe READ roe WRITE set_roe NOTIFY roeChanged)
    Q_PROPERTY(float roa READ roa WRITE set_roa NOTIFY roaChanged)
    Q_PROPERTY(float netincome   READ netincome   WRITE set_netincome   NOTIFY  netincomeChanged)
    Q_PROPERTY(float margin_gros READ margin_gros WRITE set_margin_gros NOTIFY marginGrosChanged)
    Q_PROPERTY(float margin_oper READ margin_oper WRITE set_margin_oper NOTIFY marginOperChanged)
public:
    Profitability(QObject* parent = nullptr);

    float roe() const;
    float roa() const;
    float margin_gros() const;
    float margin_oper() const;
    float netincome() const;

public slots:
    void set_roe(float newRoe);
    void set_roa(float newRoa);
    void set_margin_gros(float new_gross_margin);
    void set_margin_oper(float new_operating_margin);
    void set_netincome(float new_net_income);

signals:
    void roeChanged(float roe);
    void roaChanged(float roa);
    void marginGrosChanged(float gross_margin);
    void marginOperChanged(float operating_margin);
    void netincomeChanged(float netincome);

private:
    float _roe;
    float _roa;
    float _margin_gros;
    float _margin_oper;
    float _netincome;

    friend QDataStream& operator << (QDataStream& s, const Profitability& p);
    friend QDataStream& operator >> (QDataStream& s,       Profitability& p);
};

#endif
