#ifndef STOCK_MANAGER_SERVER_ROSTER_H
#define STOCK_MANAGER_SERVER_ROSTER_H

#include <QtCore/QObject>

class Roster : public QObject
{
    Q_OBJECT
public:
    Roster(QObject* parent = nullptr);

private:
    //
};

#endif
