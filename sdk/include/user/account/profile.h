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

class sdk::Profile
{
public:
    Profile();

    uint32_t id() const;

    QString username() const;
    void setUsername(const QString& newUsername);

    QString email() const;
    void setEmail(const QString& newEmail);

    QDateTime created() const;
    void setCreated(const QDateTime& newCreated);

private:
    uint32_t _id;
    QString _username;
    QString _email;
    QDateTime _created;

    friend QDataStream& operator << (QDataStream& s, const Profile& d);
    friend QDataStream& operator >> (QDataStream& s,       Profile& d);
};

#endif // SDK_USER_ACCOUNT_PROFILE_H
