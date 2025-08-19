#include "identity.h"
#include "api/filefetch.h"
#include <QtGui/QImageReader>
#include <QtCore/QBuffer>
#include "../instrument.h"

using namespace data;

data::Identity::Identity(QObject* parent) : QObject(parent)
{
    connect(this, &Identity::logoChanged, this, &Identity::cache_logo);
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

QString Identity::    country() const { return  _country; }
void    Identity::set_country(const QString& new_country)
{
    if (_country == new_country)
        return;
    _country = new_country;
    emit countryChanged(_country);
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

QUrl Identity::logo_url() const { return _logo; }
QUrl Identity::logo() const
{
    if (! _logo_cache.isEmpty())
        return _logo_cache;

    return _logo;
}

void Identity::cache_logo()
{
    if (_logo.isEmpty())
        return;

    QByteArray data = _logo_bytes;
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer);
    QByteArray fmt = reader.format();

    QImage img = reader.read();
    buffer.close();

    if (img.isNull()) {
        load_logo();
        return;
    }

    // Масштабуємо до 64×64 зберігаючи пропорції
    img = img.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Кодуємо в потрібний формат
    QByteArray scaledData;
    QBuffer outBuffer(&scaledData);
    outBuffer.open(QIODevice::WriteOnly);
    img.save(&outBuffer, fmt.isEmpty() ? "PNG" : fmt.constData());
    outBuffer.close();

    const QString mime = fmt.isEmpty() ? QStringLiteral("png") : QString::fromLatin1(fmt);
    const QString base64 = QString::fromLatin1(scaledData.toBase64());

    // QML Image чудово розуміє data: URL
    QUrl temp = QStringLiteral("data:image/") + mime + QStringLiteral(";base64,") + base64;
    if (temp == _logo_cache)
        return;

    _logo_cache = temp;
    emit logoChanged(logo());
}

void Identity::set_logo(const QUrl& new_logo)
{
    if (_logo == new_logo)
        return;

    _logo = new_logo;
    load_logo();
    emit logoChanged(_logo);
}

void Identity::set_logo_bytes(QByteArray data)
{
    _logo_bytes = data;
    emit logoChanged(logo());
}

QUrl Identity::    url() const { return _url; }
void Identity::set_url(const QUrl& new_url)
{
    if (_url == new_url)
        return;
    _url = new_url;
    emit urlChanged(_url);
}

void Identity::load_logo() const
{
    api::FileFetch::fetch_logo(static_cast <Instrument*> (parent())->primary_symbol(), _logo);
}

uint8_t Identity::filled_capacity() const
{
    uint8_t filled = 0, count = 0;
    if (count++; not     _title.isEmpty()) filled++;
    if (count++; not   _country.isEmpty()) filled++;
    if (count++; not  _descript.isEmpty()) filled++;
    if (count++; not    _sector.isEmpty()) filled++;
    if (count++; not  _industry.isEmpty()) filled++;
    if (count++; not _headquart.isEmpty()) filled++;
    if (count++; not      _isin.isEmpty()) filled++;

    if (count++; not  _ipo.isNull())  filled++;
    if (count++; not _logo.isEmpty()) filled++;
    if (count++; not  _url.isEmpty()) filled++;

    return filled * 100 / count;
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Identity& i) {
        s << i._title    << i._descript  << i._sector
          << i._industry << i._headquart << i._isin
          << i._ipo      << i._logo      << i._url
          << i._country  << i._logo_bytes;
        qDebug() << i._country << Q_FUNC_INFO;
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Identity& i) {
        s >> i._title    >> i._descript  >> i._sector
          >> i._industry >> i._headquart >> i._isin
          >> i._ipo      >> i._logo      >> i._url
          >> i._country  >> i._logo_bytes;
        i.cache_logo();
        qDebug() << i._country << Q_FUNC_INFO;
        return s;
    }
}
