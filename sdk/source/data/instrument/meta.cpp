#include "data/instrument/meta.h"
#include "utilities/features.h"
#include <QtCore/QIODevice>
#include <QRegularExpression>
#include <QtCore/QBuffer>
#include "data/instrument.h"

data::Meta::Meta(QObject* parent) : QObject(parent), _title(""), _type(sdk::Instype::Unknown)
{
    //
}
data::Instrument* data::Meta::instrument() const { return static_cast <Instrument*> (parent());}

data::Meta& data::Meta::operator = (const Meta& other)
{
    if (&other != this){
        _isin_code = other._isin_code;
        _isin_country = other._isin_country;
        _title = other._title;
        _type = other._type;
    }
    return *this;
}

data::Meta& data::Meta::operator = (const QByteArray& raw)
{
    QBuffer buffer;
    buffer.setData(raw);
    buffer.open(QIODevice::ReadOnly);

    QDataStream stream(&buffer);
    stream >> _isin_code >> _isin_country >> _title >> _type;
    return *this;
}

void data::Meta::set_isin(QByteArray isin)
{
    if (isin.length() != 12)
        return;

    _isin_country = sdk::country::from_string(isin.left(2));
    _isin_code = isin.right(10);
}

void data::Meta::set_title(QString title)
{
    if (_title == title)
        return;

    _title = title;
    emit titleChanged();
}

void data::Meta::set_type(sdk::Instype type)
{
    if (_type == type)
        return;

    _type = type;
    emit typeChanged();
}

sdk::Country data::Meta::isin_country() const { return _isin_country; }
QString      data::Meta::isin_full()    const { return &_isin_country + _isin_code; }
QString      data::Meta::isin_code()    const { return _isin_code; }

QByteArray data::Meta::data() const
{
    QByteArray raw;
    QDataStream stream(&raw, QIODevice::WriteOnly);
    stream << _isin_code << _isin_country << _title << _type;
    return raw;
}

QDataStream& data::Meta::save(QDataStream& s, data::ticker::SymbolList tickers) const
{
    util::list_to_stream(s, tickers);
    return s << data();
}

data::ticker::SymbolList data::Meta::load(QDataStream& s)
{
    QByteArray raw;
    data::ticker::SymbolList list = load(s, raw);

    operator = (raw);
    return list;
}

data::ticker::SymbolList data::Meta::load(QDataStream& s, QByteArray& data)
{
    data::ticker::SymbolList list;
    util::list_from_stream(s, list);
    s >> data;
    return list;
}



// void meta::Ticker::normalize() const
// {
//     QString n = name.toUpper();
//     n.remove(".");
//     n.remove(",");
//     n.replace("-", " ");
//     n.replace("_", " ");
//     n = n.simplified();

//     QStringList temp = n.split(' ', Qt::SkipEmptyParts);

//     static const QSet<QString> drop {
//         "INC","INC.","CORP","CORPORATION","CO","COMPANY","LLC","PLC","PLC.",
//         "AG","SE","NV","SA","SPA","GMBH",
//         "O","O.N","ON",
//         "VZO","VZ","VORZUG","PREF","PFD","NON","NON-VTG","NONVOTING",
//         "ADR","CDR","GDR",
//         "CLASS","CL","SHARES","ORD","ORDINARY","PREFERRED",
//         "AG", "N",
//         "A","B","C","D","E","F",
//         "SERIES","SER","SER.A","SER.B","SER.C"
//     };

//     temp.erase(std::remove_if(temp.begin(), temp.end(),
//                               [&](const QString& part){ return drop.contains(part); }),
//                temp.end());

//     _name_normalize = temp.join(' ');
// }

// const QString& meta::Ticker::name_normalize() const
// {
//     if (_name_normalize.isEmpty())
//         normalize();
//     return _name_normalize;
// }

// void meta::Ticker::clear_cache()
// {
//     _name_normalize.clear();
// }
