/*!
\class Targets
\brief Цілі/плани купівлі без створення заявки.

\details
Аналітичний список «що купити і чому»: теза, цільова ціна, дедлайн, умови входу.
Не створює ордер автоматично (на відміну від \ref Pending).

\section targets_usage Приклад
\code
Targets tg;
tg.add(symbol("TTE"),
       "дивіденди+енергоцикл",   // thesis
       70.0,                     // target price
       QDate::currentDate().addMonths(3)); // deadline
\endcode

*/
