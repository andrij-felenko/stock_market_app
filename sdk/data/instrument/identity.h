#ifndef DATA_TICKER_IDENTIFY_H
#define DATA_TICKER_IDENTIFY_H

#include "data/geo/geo.h"
#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QSize>

namespace data {
    class Instrument;
    class Identity;
}

/*!
 * \class data::Identity
 * \brief Опис емітента: бізнес, географія, галузь, публічна інформація.
 *
 * \property data::Identity::title
 * Юридична/ринкова назва компанії. Використовується для пошуку та відображення.
 *
 * \property data::Identity::descript
 * Короткий опис бізнесу: продукти/послуги, ринки, структура. Допомагає контекстуалізувати метрики.
 *
 * \property data::Identity::country
 * Країна базування/реєстрації. Важлива для валютного/регуляторного ризику та премії за країну.
 *
 * \property data::Identity::sector
 * Сектор економіки (GICS/ICB). Дозволяє порівнювати мультиплікатори усередині однорідних груп.
 *
 * \property data::Identity::industry
 * Індустрія/підгалузь. Точніший класифікатор для peer-компарів.
 *
 * \property data::Identity::headquart
 * Штаб-квартира (місто/адреса). Дає додатковий геоконтекст (податки, кадри, логістика).
 *
 * \property data::Identity::isin
 * Міжнародний ідентифікатор цінного паперу. Потрібен для крос-платформенного зв’язування даних.
 *
 * \property data::Identity::ipo
 * Дата IPO. Визначає «вік» публічної історії, релевантність історичних мультиплікаторів.
 *
 * \property data::Identity::url
 * Офіційний сайт емітента. Джерело звітів, прес-релізів, IR-матеріалів.
 *
 * \property data::Identity::logo / data::Identity::logo_url / data::Identity::logo_size
 * Візуальна айдентика бренду (локальний кеш/віддалений URL/відносний розмір).
 * Для UX ідентифікації тікерів.
 */
class data::Identity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString descript  READ descript    NOTIFY  descriptChanged)
    Q_PROPERTY(QString country   READ country_str NOTIFY   countryChanged)
    Q_PROPERTY(QString sector    READ sector      NOTIFY    sectorChanged)
    Q_PROPERTY(QString industry  READ industry    NOTIFY  industryChanged)
    Q_PROPERTY(QString headquart READ headquart   NOTIFY headquartChanged)
    Q_PROPERTY(QDate ipo         READ ipo         NOTIFY  ipoChanged FINAL)
    Q_PROPERTY(QUrl  url         READ url         NOTIFY  urlChanged FINAL)
    Q_PROPERTY(QUrl logo         READ logo        NOTIFY logoChanged FINAL)
    Q_PROPERTY(QUrl logo_url     READ logo_url    NOTIFY logoChanged FINAL)
    Q_PROPERTY(float logo_size   READ logo_size   NOTIFY logoChanged FINAL)
public:
    QString title() const;
    QString descript() const;
    QString sector() const;
    geo::Country country() const;
    QString country_str() const;
    QString exchange() const;
    QString industry() const;
    QString headquart() const;
    QString isin() const;
    QDate ipo()     const;
    QUrl logo()     const;
    QUrl logo_url() const;
    QUrl url()      const;
    float logo_size() const;

    uint8_t filledCapacity() const;
    void setLogoBytes(QByteArray data);
    void setTitle(const QString& new_title);
    void setDescrip(const QString& new_descript);
    void setSector(const QString& new_sector);
    void setCountry(const QString& new_country);
    void setCountry(geo::Country c);
    void setIndustry(const QString& new_industry);
    void setHeadquart(const QString& new_headquart);
    void setIsin(const QString& new_isin);
    void setIpo(const QDate& new_ipo);
    void setLogo(const QUrl& new_logo);
    void setUrl(const QUrl& new_url);

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
    Identity(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    QString _title;
    geo::Country _country;
    QString _descript;
    QString _sector;
    QString _industry;
    QString _headquart;
    QString _isin;
    QDate _ipo;
    QUrl _logo;
    QUrl _url;
    QByteArray _logo_bytes;
    QUrl _logo_cache;
    float _logo_size;

    void cache_logo();
    void load_logo() const;

    friend QDataStream& operator << (QDataStream& s, const Identity& i);
    friend QDataStream& operator >> (QDataStream& s,       Identity& i);
};

#endif
