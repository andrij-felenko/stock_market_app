/*!
\file ledger_overview.dox
\brief Огляд користувацьких сутностей обліку

\section intro Коротко
Цей модуль описує користувацькі списки та контейнери верхнього рівня:

- \ref Portfolio — куплене/продане (фактичні угоди/позиції).
- \ref Watchlist — спостереження (цікаві тікери без дій).
- \ref Pending — активні, ще не виконані заявки.
- \ref Targets — цілі/плани купівлі без створення заявки.
- \ref Screener — правила/умови моніторингу (ціна/метрики/патерни).

Контейнери:
- \ref Ledger — тримає всі списки вище.
- \ref Profile — інформація про користувача.
- \ref Account — об’єднує \ref Profile + \ref Ledger.
- \ref Session — логін/логаут, токени, керування сесією.

\section naming Неймінг
Назви підібрані так, щоб були і професійними для трейдерів, і зрозумілими для клієнтів:
Portfolio, Watchlist, Pending, Targets, Screener, Ledger, Profile, Account, Session.
*/


/*!
\class Ledger
\brief Реєстр користувача: тримає Portfolio, Watchlist, Pending, Targets, Screener.

\details
Єдиний вхід для роботи з «активами та намірами» користувача.
Інкапсулює збереження, завантаження та узгодження даних між списками.

\section ledger_usage Приклад
\code
Ledger ldg;
ldg.watchlist().add("RHM");
ldg.targets().add("AIR", "цикл авіаперевезень", 180.0, QDate::currentDate().addMonths(6));
ldg.pending().placeLimitBuy("AAPL", 5, 165.0);
ldg.portfolio().syncFromBroker(broker_feed);
auto alerts = ldg.screener().evaluate(market_feed);
\endcode
*/
