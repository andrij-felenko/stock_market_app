#ifndef SDK_USER_LEDGER_PENDING_H
#define SDK_USER_LEDGER_PENDING_H
/*!
\class Pending
\brief Активні, ще не виконані заявки (очікування виконання у брокера).

\details
Відображає стан заявок типу limit/stop/stop-limit/OCO тощо, поки вони не виконані або скасовані.
Виконані переходять до \ref Portfolio (у вигляді позицій/транзакцій).

\section pending_usage Приклад
\code
Pending pd;
pd.placeLimitBuy(symbol("AAPL"), qty=5, limit=165.00);
pd.cancel(symbol("AAPL")); // якщо потрібно
\endcode
*/

#include "sdk_def.h"

class sdk::Pending
{
public:
    Pending() = default;

private:
    //
};

#endif // SDK_USER_LEDGER_PENDING_H
