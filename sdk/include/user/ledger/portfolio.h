#ifndef SDK_USER_LEDGER_PORTFOLIO_H
#define SDK_USER_LEDGER_PORTFOLIO_H
/*!
\class Portfolio
\brief Куплене/продане — фактичні позиції та історія виконаних ордерів.

\details
Містить записи про наявні позиції, середню ціну входу, P/L, історію виконання.
Не включає активні невиконані заявки — вони у \ref Pending.

\section portfolio_usage Приклад
\code
Portfolio pf;
pf.add(symbol("AAPL"), qty = 10, price = 180.50);
auto pl = pf.unrealizedPnL("AAPL");
\endcode
*/

#include "sdk_def.h"

class sdk::Portfolio
{
public:
    Portfolio() = default;

private:
    //
};

#endif // SDK_USER_LEDGER_PORTFOLIO_H
