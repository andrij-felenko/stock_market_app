#ifndef SDK_INSTRUMENT_META_H
#define SDK_INSTRUMENT_META_H

#include "sdk.h" // IWYU pragma: keep
#include "utilities/trackable.h"
#include <QtCore/QJsonObject>
#include <QtCore/QSize>

class sdk::Meta : Trackable
{
public:
    Meta();

    const QString& sector() const { return _sector; }
    FieldTOpt setSector(const QString& sector)
    { return sdk::set_if(this, _sector, sector, sdk::Meta_sector); }

    const QString& industry() const { return _industry; }
    FieldTOpt setIndustry(const QString& industry)
    { return sdk::set_if(this, _industry, industry, sdk::Meta_industry); }

    const QString& gicSector() const { return _gic_sector; }
    FieldTOpt setGicSector(const QString& gic_sector)
    { return sdk::set_if(this, _gic_sector, gic_sector, sdk::Meta_gic_sector); }

    const QString& gicGroup() const { return _gic_group; }
    FieldTOpt setGicGroup(const QString& gic_group)
    { return sdk::set_if(this, _gic_group, gic_group, sdk::Meta_gic_group); }

    const QString& gicIndustry() const { return _gic_industry; }
    FieldTOpt setGicIndustry(const QString& gic_industry)
    { return sdk::set_if(this, _gic_industry, gic_industry, sdk::Meta_gic_industry); }

    const QString& gicSubIndustry() const { return _gic_subinddustry; }
    FieldTOpt setGicSubIndustry(const QString& industry)
    { return sdk::set_if(this, _gic_subinddustry, industry, sdk::Meta_gic_subindustry); }

    const QString& description() const { return _description; }
    FieldTOpt setDescription(const QString& description)
    { return sdk::set_if(this, _description, description, sdk::Meta_description); }

    const QJsonObject& officets() const { return _officers; }
    FieldTOpt setOfficers(const QJsonObject& officers)
    { return sdk::set_if(this, _officers, officers, sdk::Meta_officers); }

    const QString& phoneNumber() const { return _phone_number; }
    FieldTOpt setPhoneNumber(const QString& phonenumber)
    { return sdk::set_if(this, _phone_number, phonenumber, sdk::Meta_phone_number); }

    const QUrl& website() const { return _website; }
    FieldTOpt setWebsite(const QUrl& website)
    { return sdk::set_if(this, _website, website, sdk::Meta_website); }

    int fulltimeEmployees() const { return _fulltime_employees; }
    FieldTOpt setFulltimeEmployees(const int count)
    { return sdk::set_if(this, _fulltime_employees, count, sdk::Meta_fulltime_employees); }


    // ----------------------- Logo ---------------------------------------------------------------
    const QUrl& logoLink() const { return _logo_url; }
    FieldTOpt setLogoLink(const QUrl& logoLink)
    { return sdk::set_if(this, _logo_url, logoLink, sdk::Meta_logo_url); }

    const QUrl logo() const { return _logo; }
    const QByteArray& logoFull() const;

    FieldTOpt   updateLogo(const Isin& isin, const QByteArray& data);
    void          loadLogo(const Isin& isin) const;
    static QByteArray logoFull(const Isin& isin);
    // ============================================================================================

private:
    QString _sector;
    QString _industry;
    QString _gic_sector;
    QString _gic_group;
    QString _gic_industry;
    QString _gic_subinddustry;
    QString _description;
    QJsonObject _officers;
    QString _phone_number;
    QUrl _website;
    int _fulltime_employees;

    QUrl _logo_url;
    QUrl _logo;
    QSize _logo_size;
};

#endif // SDK_INSTRUMENT_META_H
