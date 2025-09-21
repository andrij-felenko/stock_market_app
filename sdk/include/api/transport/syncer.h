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


class sdk::api::Syncer : public QObject {
    Q_OBJECT
public:
    // explicit Syncer(QUrl url, QObject* parent = nullptr);

    struct Date {
        QDateTime dt;
        sdk::DataType type;
    };
};

#endif // SDK_API_TRANSPORT_SYNCER_H
