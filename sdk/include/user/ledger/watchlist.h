#ifndef SDK_USER_LEDGER_WATCHLIST_H
#define SDK_USER_LEDGER_WATCHLIST_H
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

#include "sdk_def.h"
#include "core/symbol.h"

class sdk::WatchList
{
public:
    WatchList(QString name = "");

private:
    QString _name;
    SymbolList _list;

    friend QDataStream& operator << (QDataStream& s, const WatchList& d);
    friend QDataStream& operator >> (QDataStream& s,       WatchList& d);
};

#endif // SDK_USER_LEDGER_WATCHLIST_H
