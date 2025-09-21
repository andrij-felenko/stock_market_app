/*!
\class Screener
\brief Правила/умови моніторингу (ціна, метрики, патерни).

\details
Налаштовувані сценарії «якщо-умова-then-повідомити»: price in range, EPS change, патерни,
розташування в каналі для періодичних тікерів тощо. Ширше, ніж «alerts».

\section screener_usage Приклад
\code
Screener sc;
sc.addRule(symbol("AAPL"), Rule::priceBetween(150, 160));
sc.addRule(symbol("TXN"),  Rule::epsGrowthYoY(> 0.10));
auto hits = sc.evaluate(market_feed);
\endcode
*/
