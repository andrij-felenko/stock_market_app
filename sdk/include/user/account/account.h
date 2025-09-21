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
