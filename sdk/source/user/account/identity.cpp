#include "user/account/identity.h"

sdk::Identity::Identity() { /* */ }
sdk::Identity::Identity(uint32_t id) : _id(id) { /* */ }

bool sdk::Identity::valid() const { return _id != 0; }

uint32_t sdk::Identity::id() const { return _id; }

QString sdk::Identity::   username() const { return _username; }
void    sdk::Identity::setUsername(const QString& newUsername) { _username = newUsername; }

QString sdk::Identity::   email() const { return _email; }
void    sdk::Identity::setEmail(const QString& newEmail) { _email = newEmail; }


namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Identity> d){
        if (d.data()) s << d->_id << d->_hash << d->_email << d->_username;
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Identity> d){
        if (d.data()) s >> d->_id >> d->_hash >> d->_email >> d->_username;
        return s >> io(d->_track, d);
    }

    QDataStream& operator << (QDataStream& s, const Identity& d)
    { return s << Wire (d); }

    QDataStream& operator >> (QDataStream& s, Identity& d)
    { return s >> Wire (d); }
}
