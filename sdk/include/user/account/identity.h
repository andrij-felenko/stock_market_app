#ifndef SDK_USER_ACCOUNT_IDENTITY_H
#define SDK_USER_ACCOUNT_IDENTITY_H
/*!
\class Profile
\brief Базова інформація про користувача (ім’я, email, країна, налаштування).

\details
Не містить фінансових списків; відповідає лише за ідентифікаційні та UX-параметри.
Використовується \ref Account та \ref Session.
*/

#include "sdk_def.h"

class sdk::Identity
{
public:
    Identity();

    bool valid() const;

    uint32_t id() const;
    QByteArray hash() const;

    QString username() const;
    void setUsername(const QString& newUsername);

    QString email() const;
    void setEmail(const QString& newEmail);

private:
    uint32_t _id = 0;
    QString _username;
    QString _email;
    QByteArray _hash; // server side password, client side - token
    Trackable _track;

    Identity(uint32_t id);

    friend QDataStream& operator << (QDataStream& s, Wire <const Identity> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Identity> d);

    friend QDataStream& operator << (QDataStream& s, const Identity& d);
    friend QDataStream& operator >> (QDataStream& s,       Identity& d);

    friend class sdk::Session;
};

#endif // SDK_USER_ACCOUNT_IDENTITY_H
