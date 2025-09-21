#include "user/account/profile.h"

uint32_t sdk::Profile::id() const
{
    return _id;
}

QString sdk::Profile::username() const
{
    return _username;
}

void sdk::Profile::setUsername(const QString& newUsername)
{
    _username = newUsername;
}

QString sdk::Profile::email() const
{
    return _email;
}

void sdk::Profile::setEmail(const QString& newEmail)
{
    _email = newEmail;
}

QDateTime sdk::Profile::created() const
{
    return _created;
}

void sdk::Profile::setCreated(const QDateTime& newCreated)
{
    _created = newCreated;
}



namespace sdk {
    QDataStream& operator << (QDataStream& s, const Profile& d) {
        return s << d._id << d._created << d._email << d._username;
    }

    QDataStream& operator >> (QDataStream& s, Profile& d) {
        s >> d._id >> d._created >> d._email >> d._username;
        return s;
    }
}
