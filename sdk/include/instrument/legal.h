#ifndef SDK_INSTRUMENT_LEGAL_H
#define SDK_INSTRUMENT_LEGAL_H

#include "sdk.h" // IWYU pragma: keep
#include <QtCore/QDate>
#include <QtCore/QJsonObject>

class sdk::Legal : Trackable
{
public:
    Legal();

    const QString& openFigi() const { return _open_figi; }
    FieldTOpt setOpenFigi(const QString& openfigi)
    { return sdk::set_if(this, _open_figi, openfigi, sdk::Legal_openFigi); }

    const QString& lei() const { return _lei; }
    FieldTOpt setLei(const QString& lei) { return sdk::set_if(this, _lei, lei, sdk::Legal_lei); }

    const QString& cik() const { return _cik; }
    FieldTOpt setCik(const QString& cik) { return sdk::set_if(this, _cik, cik, sdk::Legal_cik); }

    sdk::Month fiscalYearEnd() const { return _fiscal_yearend; }
    FieldTOpt setFiscalYearEnd(sdk::Month month)
    { return sdk::set_if(this, _fiscal_yearend, month, sdk::Legal_fiscalYearEnd); }

    const QString& ein() const { return _ein; }
    FieldTOpt setEin(const QString& ein) { return sdk::set_if(this, _ein, ein, sdk::Legal_ein); }

    const QString& employerIdNumber() const { return ein(); }
    FieldTOpt setEmployerIdNumber(const QString& ein) { return setEin(ein); }

    const QString& cusip() const { return _cusip; }
    FieldTOpt setCusip(const QString& cusip)
    { return sdk::set_if(this, _cusip, cusip, sdk::Legal_cusip); }

    const QDate ipo() const { return _ipo; }
    FieldTOpt setIpo(const QDate& date)
    { return sdk::set_if(this, _ipo, date, sdk::Legal_ipoDate); }

    sdk::HomeCategory internationalDomestic() const { return _internationalDomestic; }
    FieldTOpt setInternationalDomestic(sdk::HomeCategory domestic)
    { return sdk::set_if(this, _internationalDomestic, domestic,
                         sdk::Legal_internationalDomestic); }

    bool delisted() const { return _delisted; }
    FieldTOpt setDelisted(bool delisted)
    { return sdk::set_if(this, _delisted, delisted, sdk::Legal_delisted); }

    const QString& address() const { return _address; }
    FieldTOpt setAddress(const QString& address)
    { return sdk::set_if(this, _address, address, sdk::Legal_address); }

    const QJsonObject& addressJSON() const { return _address_json; }
    FieldTOpt setAddress(const QJsonObject& address)
    { return sdk::set_if(this, _address_json, address, sdk::Legal_address); }

private:
    QString _open_figi;
    QString _lei;
    QString _cik;
    sdk::Month _fiscal_yearend;
    QString _ein; // employer identification number
    QString _cusip; // унікальний код цінного паперу у США
    QDate _ipo;
    sdk::HomeCategory _internationalDomestic;
    bool _delisted;
    QString _address;
    QJsonObject _address_json;
};

#endif // SDK_INSTRUMENT_LEGAL_H
