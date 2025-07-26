#ifndef DATA_TICKER_IDENTIFY_H
#define DATA_TICKER_IDENTIFY_H

#include "data/currency/name.h"
#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data { class Identity; }

class data::Identity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title     READ title     WRITE set_title     NOTIFY     titleChanged)
    Q_PROPERTY(QString descript  READ descript  WRITE set_descrip   NOTIFY  descriptChanged)
    Q_PROPERTY(QString country   READ country   WRITE set_country   NOTIFY   countryChanged)
    Q_PROPERTY(QString sector    READ sector    WRITE set_sector    NOTIFY    sectorChanged)
    Q_PROPERTY(QString industry  READ industry  WRITE set_industry  NOTIFY  industryChanged)
    Q_PROPERTY(QString headquart READ headquart WRITE set_headquart NOTIFY headquartChanged)
    Q_PROPERTY(QString isin      READ isin      WRITE set_isin      NOTIFY      isinChanged)
    Q_PROPERTY(QDate ipo READ ipo  WRITE set_ipo  NOTIFY  ipoChanged FINAL)
    Q_PROPERTY(QUrl  url READ url  WRITE set_url  NOTIFY  urlChanged FINAL)
    Q_PROPERTY(QUrl logo READ logo WRITE set_logo NOTIFY logoChanged FINAL)
public:
    Identity(QObject* parent = nullptr);

    QString title() const;
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
    void set_title(const QString& new_title);
    void set_descrip(const QString& new_descript);
    void set_sector(const QString& new_sector);
    void set_country(const QString& new_country);
    void set_industry(const QString& new_industry);
    void set_headquart(const QString& new_headquart);
    void set_isin(const QString& new_isin);
    void set_ipo(const QDate& new_ipo);
    void set_logo(const QUrl& new_logo);
    void set_url(const QUrl& new_url);

signals:
    void titleChanged(QString title);
    void tickerChanged(QString ticker);
    void descriptChanged(QString descript);
    void sectorChanged(QString sector);
    void countryChanged(QString country);
    void industryChanged(QString industry);
    void headquartChanged(QString headquart);
    void isinChanged(QString isin);
    void ipoChanged(QDate ipo);
    void logoChanged(QUrl logo);
    void urlChanged(QUrl url);

private:
    QString _title;
    QString _country;
    QString _descript;
    QString _sector;
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
