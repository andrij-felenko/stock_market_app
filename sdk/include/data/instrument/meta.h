#ifndef DATA_META_H
#define DATA_META_H

#include "geo/geo.h"
#include "data/instrument/symbol.h"
#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include <QtCore/QDataStream>

namespace api {
    class Eod;
}

namespace data {
    class Meta;
    class Instrument;
}

class data::Meta : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString isin  READ isin_str NOTIFY  isinChanged)
    Q_PROPERTY(QString title READ title    NOTIFY titleChanged)
    Q_PROPERTY(QString type  READ type_str NOTIFY  typeChanged)
public:
    Meta& operator = (const Meta& other);
    Meta& operator = (const QByteArray& raw);

    QByteArray data() const;
    QDataStream& save(QDataStream& s, data::ticker::SymbolList tickers) const;
    data::ticker::SymbolList load(QDataStream& s);
    static data::ticker::SymbolList load(QDataStream& s, QByteArray& data);

    QString isin_str() const { return isin_full(); }
    QString title() const { return _title; }
    QString type_str() const { return sdk::instype::to_string(_type); }
    sdk::Instype type() const { return _type; }

    void set_title(QString title);
    void set_type(sdk::Instype type);
    void set_isin(QByteArray isin);
    sdk::Country isin_country() const;
    QString isin_code() const;
    QString isin_full() const;

signals:
    void  isinChanged();
    void titleChanged();
    void  typeChanged();

private:
    explicit Meta(QObject* parent = nullptr);
    friend class Instrument;
    friend class Market;
    friend class api::Eod;
    Instrument* instrument() const;

    sdk::Country _isin_country;
    QByteArray _isin_code;

    QString _title;
    sdk::Instype _type;
};

/*
    "Code": "1T9", T //
    "Exchange": "F", T //
    "Name": "Tarkett", I //
    "Country": "Germany", - //
    "Currency": "EUR", T - //
    "Type": "Common Stock", I //
    "Isin": "FR0`004`188`670" I //
 * */

#endif
