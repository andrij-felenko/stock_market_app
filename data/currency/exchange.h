#ifndef TICKER_H
#define TICKER_H

#include <QtCore/QObject>

namespace data { class Ticker; }

class data::Ticker : public QObject
{
public:
    Ticker(QObject* parent = nullptr);
};

#endif // TICKER_H
