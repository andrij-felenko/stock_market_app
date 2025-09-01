#include "instrument/meta.h"

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
    QSize size = img.size();
    float coef = float(size.width()) / size.height();
    img = img.scaled(64 * coef, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _logo_size = coef;

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

void Identity::load_logo() const
{
    api::FileFetch::fetch_logo(static_cast <Instrument*> (parent())->primary_symbol(), _logo);
}
