/*!
\class Watchlist
\brief Список спостереження — тікери, за якими стежимо без намірів купівлі.

\details
Лише відображення/відстеження цін/новин/подій. Не містить жодних заявок.
Добре підходить для раннього дослідження ідей.

\section watchlist_usage Приклад
\code
Watchlist wl;
wl.add(symbol("NVDA"));
wl.add(symbol("RHM"));
for (auto s : wl) { render_quote(s); }
\endcode
*/
