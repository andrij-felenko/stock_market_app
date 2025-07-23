#include "identity.h"

using namespace data;

data::Identity::Identity(QObject* parent) : QObject(parent)
{
    //
}

QString Identity::    title() const { return _title; }
void    Identity::set_title(const QString& new_title)
{
    if (_title == new_title)
        return;
    _title = new_title;
    emit titleChanged(_title);
}

QString Identity::    descript() const { return _descript; }
void    Identity::set_descrip(const QString& new_descript)
{
    if (_descript == new_descript)
        return;
    _descript = new_descript;
    emit descriptChanged(_descript);
}

QString Identity::    sector() const { return _sector; }
void    Identity::set_sector(const QString& new_sector)
{
    if (_sector == new_sector)
        return;
    _sector = new_sector;
    emit sectorChanged(_sector);
}

QString Identity::    industry() const { return _industry; }
void    Identity::set_industry(const QString& new_industry)
{
    if (_industry == new_industry)
        return;
    _industry = new_industry;
    emit industryChanged(_industry);
}

QString Identity::    headquart() const { return _headquart; }
void    Identity::set_headquart(const QString& new_headquart)
{
    if (_headquart == new_headquart)
        return;
    _headquart = new_headquart;
    emit headquartChanged(_headquart);
}

QString Identity::    isin() const { return _isin; }
void    Identity::set_isin(const QString& new_isin)
{
    if (_isin == new_isin)
        return;
    _isin = new_isin;
    emit isinChanged(_isin);
}

QDate Identity::    ipo() const { return _ipo; }
void  Identity::set_ipo(const QDate& new_ipo)
{
    if (_ipo == new_ipo)
        return;
    _ipo = new_ipo;
    emit ipoChanged(_ipo);
}

QUrl Identity::    logo() const { return _logo; }
void Identity::set_logo(const QUrl& new_logo)
{
    if (_logo == new_logo)
        return;
    _logo = new_logo;
    emit logoChanged(_logo);
}

QUrl Identity::    url() const { return _url; }
void Identity::set_url(const QUrl& new_url)
{
    if (_url == new_url)
        return;
    _url = new_url;
    emit urlChanged(_url);
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Identity& i) {
        s << i._title    << i._descript  << i._sector
          << i._industry << i._headquart << i._isin
          << i._ipo      << i._logo      << i._url;
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Identity& i) {
        s >> i._title    >> i._descript  >> i._sector
          >> i._industry >> i._headquart >> i._isin
          >> i._ipo      >> i._logo      >> i._url;
        return s;
    }
}
