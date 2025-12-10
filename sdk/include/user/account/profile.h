#ifndef SDK_USER_ACCOUNT_PROFILE_H
#define SDK_USER_ACCOUNT_PROFILE_H
/*!
\class Profile
\brief Базова інформація про користувача (ім’я, email, країна, налаштування).

\details
Не містить фінансових списків; відповідає лише за ідентифікаційні та UX-параметри.
Використовується \ref Account та \ref Session.
*/

#include "sdk_def.h"
#include "identity.h"

class sdk::Profile
{
public:
    Profile();
    Profile(const Identity identity);

    Identity identity;

    Identity identityData() const;
    void setIdentity(const Identity& id);

    QDateTime created() const;
    void   setCreated(const QDateTime& newCreated);

private:
    Identity _identity;
    DTime _created;
    DTime _upd;
    Trackable _track;

    friend QDataStream& operator << (QDataStream& s, Wire <const Profile> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Profile> d);

    friend class sdk::Session;
};

#endif // SDK_USER_ACCOUNT_PROFILE_H
