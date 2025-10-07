#ifndef STOCK_MANAGER_SERVER_SESSION_H
#define STOCK_MANAGER_SERVER_SESSION_H

#include <QtCore/QObject>

class Session : public QObject
{
    Q_OBJECT
public:
    Session(QObject* parent = nullptr);

private:
    //
};

#endif
