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
