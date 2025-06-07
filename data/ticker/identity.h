#ifndef DATA_TICKER_IDENTIFY_H
#define DATA_TICKER_IDENTIFY_H

#include "data/currency/name.h"
#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data::ticker { class Identity; }

class data::ticker::Identity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(currency::Tag currency READ currency WRITE set_currency NOTIFY currencyChanged)
    Q_PROPERTY(QString   currency_str READ currency_str                NOTIFY currencyChanged)
    Q_PROPERTY(QString title     READ title     WRITE set_title     NOTIFY     titleChanged)
    Q_PROPERTY(QString ticker    READ ticker    WRITE set_ticker    NOTIFY    tickerChanged)
    Q_PROPERTY(QString descript  READ descript  WRITE set_descrip   NOTIFY  descriptChanged)
    Q_PROPERTY(QString country   READ country   WRITE set_country   NOTIFY   countryChanged)
    Q_PROPERTY(QString exchange  READ exchange  WRITE set_exchange  NOTIFY  exchangeChanged)
    Q_PROPERTY(QString sector    READ sector    WRITE set_sector    NOTIFY    sectorChanged)
    Q_PROPERTY(QString industry  READ industry  WRITE set_industry  NOTIFY  industryChanged)
    Q_PROPERTY(QString headquart READ headquart WRITE set_headquart NOTIFY headquartChanged)
    Q_PROPERTY(QString isin      READ isin      WRITE set_isin      NOTIFY      isinChanged)
    Q_PROPERTY(QDate ipo READ ipo  WRITE set_ipo  NOTIFY  ipoChanged FINAL)
    Q_PROPERTY(QUrl  url READ url  WRITE set_url  NOTIFY  urlChanged FINAL)
    Q_PROPERTY(QUrl logo READ logo WRITE set_logo NOTIFY logoChanged FINAL)
public:
    Identity(QObject* parent = nullptr);

    currency::Tag currency() const;
    QString currency_str() const;
    QString title() const;
    QString ticker() const;
    QString descript() const;
    QString sector() const;
    QString country() const;
    QString exchange() const;
    QString industry() const;
    QString headquart() const;
    QString isin() const;
    QDate ipo() const;
    QUrl logo() const;
    QUrl url() const;

public slots:
    void set_currency(const currency::Tag& new_currency);
    void set_title(const QString& new_title);
    void set_ticker(const QString& new_ticker);
    void set_descrip(const QString& new_descript);
    void set_sector(const QString& new_sector);
    void set_country(const QString& new_country);
    void set_exchange(const QString& new_exchange);
    void set_industry(const QString& new_industry);
    void set_headquart(const QString& new_headquart);
    void set_isin(const QString& new_isin);
    void set_ipo(const QDate& new_ipo);
    void set_logo(const QUrl& new_logo);
    void set_url(const QUrl& new_url);

signals:
    void currencyChanged(currency::Tag currency);
    void titleChanged(QString title);
    void tickerChanged(QString ticker);
    void descriptChanged(QString descript);
    void sectorChanged(QString sector);
    void countryChanged(QString country);
    void exchangeChanged(QString exchange);
    void industryChanged(QString industry);
    void headquartChanged(QString headquart);
    void isinChanged(QString isin);
    void ipoChanged(QDate ipo);
    void logoChanged(QUrl logo);
    void urlChanged(QUrl url);

private:
    currency::Tag _currency;
    QString _title;
    QString _ticker;
    QString _descript;
    QString _sector;
    QString _country;
    QString _exchange;
    QString _industry;
    QString _headquart;
    QString _isin;
    QDate _ipo;
    QUrl _logo;
    QUrl _url;

    friend QDataStream& operator << (QDataStream& s, const Identity& i);
    friend QDataStream& operator >> (QDataStream& s,       Identity& i);
};

#endif
