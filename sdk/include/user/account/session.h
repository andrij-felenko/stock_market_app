#ifndef SDK_USER_ACCOUNT_SESSION_H
#define SDK_USER_ACCOUNT_SESSION_H
/*!
\class Session
\brief Авторизація/сесія: логін, логаут, токени, терміни дії.

\details
Робота з обліковими даними (credentials), access/refresh токенами,
оновлення сесії, вихід. Може посилатися на \ref Account для ініціалізаційних дій,
але не змішує збереження профілю/реєстру з механікою входу.

\section session_usage Приклад
\code
Session sess(provider);
if (sess.login(user, pass)) {
    Account acc = sess.loadAccount();
    render(acc.ledger().portfolio());
}
\endcode
*/

#include "sdk_def.h"

class sdk::Session
{
public:
    Session() = default;

private:
    uint32_t _userid;
    QByteArray _token_access; // short token, 10 - 60 minute
    QByteArray _token_refresh; // long token,  7 - 30 days
    QDateTime _expired_short;
    QDateTime _expired_long;
    //
};

#endif // SDK_USER_ACCOUNT_SESSION_H
