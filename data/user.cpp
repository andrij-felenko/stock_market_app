#include "user.h"
#include <QtCore/QCoreApplication>

data::User* data::User::instance()
{
    static User* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new User(qApp);
    }
    return _instance;
}

data::User::User(QObject* parent) : QObject(parent)
{
    //
}
