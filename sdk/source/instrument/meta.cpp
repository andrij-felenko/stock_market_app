#include "instrument/meta.h"
#include <QtCore/QBuffer>
#include <QtGui/QImageReader>
#include "instrument/isin.h"
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include "api/filefetch.h"

FieldTOpt sdk::Meta::updateLogo(const Isin& isin, const QByteArray& data)
{
    if (data.isEmpty() || not isin.valid())
        return std::nullopt;

    QBuffer buffer;
    buffer.setData(data);
    buffer.open(QIODevice::ReadOnly);
    QImageReader reader(&buffer);
    QByteArray fmt = reader.format();

    QImage img = reader.read();
    buffer.close();

    if (img.isNull())
        return std::nullopt;

    // save full file into
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
               "/tickers_images/" + QString::fromLatin1(~isin));
    if (file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        file.write(data);
        file.close();
    }

    // Масштабуємо до 64×64 зберігаючи пропорції
    QSize size = img.size();
    float coef = float(size.width()) / size.height();
    img = img.scaled(64 * coef, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _logo_size = QSize(64 * coef, 64);

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
    if (temp == _logo)
        return std::nullopt;

    return set_if(this, _logo, temp, sdk::Meta_logo);
}

void sdk::Meta::loadLogo(const Isin& isin) const
{
    api::FileFetch::fetch_logo(~isin, _logo_url);
}

QByteArray sdk::Meta::logoFull(const Isin& isin)
{
    QByteArray ret;
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
               "/tickers_images/" + QString::fromLatin1(~isin));
    if (file.open(QIODevice::ReadOnly)){
        ret = file.readAll();
        file.close();
    }
    return ret;
}
