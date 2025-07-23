#include "info.h"
#include <QtCore/QCoreApplication>

data::user::Info* data::user::Info::instance()
{
    static Info* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Info(qApp);
    }
    return _instance;
}

data::user::Info::Info(QObject* parent) : QObject(parent)
{
    //
}
