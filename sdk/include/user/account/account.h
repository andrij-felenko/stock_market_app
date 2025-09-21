#ifndef SDK_USER_ACCOUNT_ACCOUNT_H
#define SDK_USER_ACCOUNT_ACCOUNT_H
/*!
\class Account
\brief Головний контейнер користувача: об’єднує Profile + Ledger.

\details
Рівень, який бачать більшість модулів UI/логіки. Надає доступ до
профілю користувача (\ref Profile) та його реєстру списків (\ref Ledger).
Не відповідає за авторизацію — це \ref Session.

\section account_usage Приклад
\code
Account acc;
acc.profile().setEmail("user@example.org");
acc.ledger().watchlist().add("NVDA");
save(acc);
\endcode
*/


#include "sdk_def.h"
#include "profile.h"
#include "user/ledger/ledger.h"

class sdk::Account
{
public:
    Account() = default;

    Ledger ledger;
    Profile profile;

private:
    friend QDataStream& operator << (QDataStream& s, const Account& d);
    friend QDataStream& operator >> (QDataStream& s,       Account& d);
};

#endif // SDK_USER_ACCOUNT_ACCOUNT_H
