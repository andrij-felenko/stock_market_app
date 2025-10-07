#ifndef SDK_INSTRUMENT_LEGAL_H
#define SDK_INSTRUMENT_LEGAL_H

#include "sdk_def.h" // IWYU pragma: keep
#include <QtCore/QDate>
#include <QtCore/QJsonObject>

class sdk::Legal
{
public:
    Legal();

    const QString& openFigi() const { return _open_figi; }
    FieldTOpt setOpenFigi(const QString& openfigi)
    { return sdk::set_if(&_track, _open_figi, openfigi, sdk::Legal_openFigi); }

    const QString& lei() const { return _lei; }
    FieldTOpt setLei(const QString& lei)
    { return sdk::set_if(&_track, _lei, lei, sdk::Legal_lei); }

    const QString& cik() const { return _cik; }
    FieldTOpt setCik(const QString& cik)
    { return sdk::set_if(&_track, _cik, cik, sdk::Legal_cik); }

    sdk::Month fiscalYearEnd() const { return _fiscal_yearend; }
    FieldTOpt setFiscalYearEnd(sdk::Month month)
    { return sdk::set_if(&_track, _fiscal_yearend, month, sdk::Legal_fiscalYearEnd); }

    const QString& ein() const { return _ein; }
    FieldTOpt setEin(const QString& ein)
    { return sdk::set_if(&_track, _ein, ein, sdk::Legal_ein); }

    const QString& employerIdNumber() const { return ein(); }
    FieldTOpt setEmployerIdNumber(const QString& ein) { return setEin(ein); }

    const QString& cusip() const { return _cusip; }
    FieldTOpt setCusip(const QString& cusip)
    { return sdk::set_if(&_track, _cusip, cusip, sdk::Legal_cusip); }

    const QDate ipo() const { return _ipo; }
    FieldTOpt setIpo(const QDate& date)
    { return sdk::set_if(&_track, _ipo, date, sdk::Legal_ipoDate); }

    sdk::HomeCategory internationalDomestic() const { return _international_domestic; }
    FieldTOpt setInternationalDomestic(sdk::HomeCategory domestic)
    { return sdk::set_if(&_track, _international_domestic, domestic,
                         sdk::Legal_internationalDomestic); }

    bool delisted() const { return _track.boolflag <0> (); }
    FieldTOpt setDelisted(bool delisted){ 
        if (this->delisted() == delisted) return std::nullopt;
        _track.setBoolFlag <0> (delisted);
        return sdk::Legal_delisted;
    }

    const QString& address() const { return _address; }
    FieldTOpt setAddress(const QString& address)
    { return sdk::set_if(&_track, _address, address, sdk::Legal_address); }

    const QJsonObject& addressJSON() const { return _address_json; }
    FieldTOpt setAddress(const QJsonObject& address)
    { return sdk::set_if(&_track, _address_json, address, sdk::Legal_address); }

private:
    QString _open_figi;
    QString _lei;
    QString _cik;
    QString _ein; // employer identification number
    QString _cusip; // унікальний код цінного паперу у США
    QDate _ipo;
    sdk::HomeCategory _international_domestic;
    sdk::Month _fiscal_yearend;
    Trackable _track; // delisted <0>
    QString _address;
    QJsonObject _address_json;

    friend QDataStream& operator << (QDataStream& s, Wire <const Legal> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Legal> d);
};

#endif // SDK_INSTRUMENT_LEGAL_H
