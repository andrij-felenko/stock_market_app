#include "user.h"
#include <QtCore/QCoreApplication>

data::User* data::account() { return User::instance(); }
data::User* data::User::instance()
{
    static User* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new User(qApp);
    }
    return _instance;
}

std::vector<data::Instrument*>& data::User::favorite_list() { return _favorite_list; }
std::vector<data::Stock*>&      data::User::   asset_list() { return    _asset_list; }

data::User::User(QObject* parent) : data::user::Info(parent)
{
    //
}
