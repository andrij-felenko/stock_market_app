#ifndef DATA_INSTRUMENT_H
#define DATA_INSTRUMENT_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "ticker.h"

namespace data { class Instrument; }

class data::Instrument : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ticker::Dividend*      dividend      READ dividend      CONSTANT)
    Q_PROPERTY(ticker::Identity*      identity      READ identity      CONSTANT)
    Q_PROPERTY(ticker::Stability*     stability     READ stability     CONSTANT)
    Q_PROPERTY(ticker::Valuation*     valuation     READ valuation     CONSTANT)
    Q_PROPERTY(ticker::Profitability* profitability READ profitability CONSTANT)
public:
    Instrument(QObject* parent = nullptr);

    ticker::Dividend* dividend() const;
    ticker::Identity* identity() const;
    ticker::Stability* stability() const;
    ticker::Valuation* valuation() const;
    ticker::Profitability* profitability() const;

    Ticker* primary_ticker() const;
    Ticker* primary_ticker_user() const;

    void save() const;
    void load();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    ticker::Dividend* _dividend = nullptr;
    ticker::Identity* _identity = nullptr;
    ticker::Stability* _stability = nullptr;
    ticker::Valuation* _valuation = nullptr;
    ticker::Profitability* _profitability = nullptr;

    std::vector <Ticker*> _tickers;
    QString _primary_ticker;

    void _update_primary_ticket();
    void _add_ticker(Ticker* ticker);

    friend QDataStream& operator << (QDataStream& s, const Instrument& d);
    friend QDataStream& operator >> (QDataStream& s,       Instrument& d);
};

#endif
