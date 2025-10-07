#ifndef STOCK_MANAGER_SERVER_MARKET_H
#define STOCK_MANAGER_SERVER_MARKET_H

#include <QtCore/QObject>

class Market : public QObject
{
    Q_OBJECT
public:
    Market(QObject* parent = nullptr);

private:
    //
};

#endif
