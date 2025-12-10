#ifndef STOCK_MANAGER_SERVER_ROSTER_H
#define STOCK_MANAGER_SERVER_ROSTER_H

#include <QtCore/QObject>
#include <StockSDK/service/roster.h>

class Roster : public sdk::Roster
{
    Q_OBJECT
public:
    Roster(QObject* parent = nullptr);
};

#endif
