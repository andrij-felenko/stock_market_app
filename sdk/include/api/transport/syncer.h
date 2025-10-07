#ifndef SDK_API_TRANSPORT_SYNCER_H
#define SDK_API_TRANSPORT_SYNCER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <deque>

#include "call.h"
// #include "settings/network.h" // IWYU pragma: keep
#include "core/symbol.h"

/*!
 * \brief The sdk::api::Syncer class
 * Є пару видів як саме йде робота з-до пам'ятті
 * 1. зберігаємо весб інструмент на диску
 * 2. треба переслати весь інструмент, отже шлемо його в bytearray
 * 3. треба взяти тільки кусок які ми змінили і надіслати саме його
 * 4. завантажити весб документ з файлової системи
 * 5. зберегти кусок оновленого документу
 * 6. зберегти весь документ
 *
 * 1, 4 - з рекурсією
 * 3, 5 - без рекурсії точно
 * 2, 6 - скоріше всього теж з рекурсією, бо треба вводити тртій параметр тоді
 */

class sdk::api::Syncer : public QObject {
    Q_OBJECT
public:
    enum class Reason {
        UpdateAccount,
        CheckUpdates,
        UpdateListOf,
        UpdateQuart,
        UpdateTicker,
        UpdateLedger,
    };

    // explicit Syncer(QUrl url, QObject* parent = nullptr);

    struct Date {
        DTime dt;
        sdk::DataType type;
    };
};

#endif // SDK_API_TRANSPORT_SYNCER_H
