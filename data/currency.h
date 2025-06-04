#ifndef CURRENCY_H
#define CURRENCY_H

#include <QtCore/QObject>

namespace data { class Currency; }

class data::Currency : public QObject
{
public:
    Currency(QObject* parent = nullptr);
};

#endif // TICKER_H
