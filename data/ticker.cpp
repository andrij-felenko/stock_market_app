#include "ticker.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

using namespace data::ticker;

data::Ticker::Ticker(QObject* parent) : QObject(parent)
{
    _quotes = new ticker::Quotes(this);
    _dividend = new ticker::Dividend(this);
    _identity = new ticker::Identity(this);
    _stability = new ticker::Stability(this);
    _valuation = new ticker::Valuation(this);
    _profitability = new ticker::Profitability(this);
}

// tdsm - ticker data stock manager
void data::Ticker::save() const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(basePath);
    QString filename = basePath + "/" + _identity->ticker() + ".tdsm";

    QFile file(filename);
    qDebug() << "file" << file.fileName();
    if (!file.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);
    out << *this;
    file.close();
    qDebug() << "Save to: " << filename;
}

void data::Ticker::load()
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filename = basePath + "/" + _identity->ticker() + ".tdsm";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);
    in >> *this;
    file.close();
}

Quotes*        data::Ticker::quotes()        const { return _quotes; }
Dividend*      data::Ticker::dividend()      const { return _dividend; }
Identity*      data::Ticker::identity()      const { return _identity; }
Stability*     data::Ticker::stability()     const { return _stability; }
Valuation*     data::Ticker::valuation()     const { return _valuation; }
Profitability* data::Ticker::profitability() const { return _profitability; }

namespace data {
    QDataStream& operator<<(QDataStream& s, const Ticker& d) {
        return s << *d._dividend << *d._identity  << *d._profitability
                 << *d._quotes   << *d._stability << *d._valuation;
    }

    QDataStream& operator>>(QDataStream& s, Ticker& d) {
        return s >> *d._dividend >> *d._identity  >> *d._profitability
                 >> *d._quotes   >> *d._stability >> *d._valuation;
    }
}
