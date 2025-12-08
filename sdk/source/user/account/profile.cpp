#include "user/account/profile.h"

sdk::Profile::Profile()
{
    _created.setDateTime(QDateTime::currentDateTime());
}

sdk::Profile::Profile(const Identity identity) : Profile()
{
    //
}

QDateTime sdk::Profile::   created() const{    return _created; }
void      sdk::Profile::setCreated(const QDateTime& newCreated) { _created = newCreated; }


namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Profile> d) {
        if (d.data()) s << io(d->_identity, d);
        if (d.subs()) s << d->_created;
        s << io(d->_upd, d);
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Profile> d) {
        if (d.data()) s >> io(d->_identity, d);
        if (d.subs()) s >> d->_created;
        s >> io(d->_upd, d);
        return s >> io(d->_track, d);
    }
}
