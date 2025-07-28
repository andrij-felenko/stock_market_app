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

QString data::user::Info::username() const { return _username.isEmpty() ? "guest" : _username; }
void data::user::Info::setUsername(const QString& newName)
{
    if (_username == newName)
        return;
    _username = newName;
    emit usernameChanged(_username);
    emit guestChanged(guest());
}

QString data::user::Info::email() const { return _email; }
void data::user::Info::setEmail(const QString& newEmail)
{
    if (_email == newEmail)
        return;
    _email = newEmail;
    emit emailChanged(_email);
}

bool data::user::Info::guest() const { return _username.isEmpty(); }

namespace data::user {
    QDataStream& operator << (QDataStream& s, const Info& d){
        return s << d._email << d._username;
    }

    QDataStream& operator >> (QDataStream& s, Info& d){
        return s >> d._email >> d._username;
    }
}
