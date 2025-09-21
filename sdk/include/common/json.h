#ifndef SDK_COMMON_JSON_H
#define SDK_COMMON_JSON_H

#include <QtCore/QString>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

namespace json {
    std::optional <QString> string(const QJsonObject& obj, const char* key);

    std::optional <bool> boolean(const QJsonObject& obj, const char* key);

    std::optional <QDate> date(const QJsonObject& obj, const char* key,
                               const char* fmt = "yyyy-MM-dd");

    std::optional <QJsonObject> object(const QJsonObject& obj, const char* key);

    std::optional <int64_t> integer(const QJsonObject& obj, const char* key);

    std::optional <double> real(const QJsonObject& obj, const char* key);
}

#endif // SDK_COMMON_JSON_H
