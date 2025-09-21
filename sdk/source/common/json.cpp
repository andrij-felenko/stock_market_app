#include "common/json.h"

std::optional <QString> json::string(const QJsonObject& obj, const char* key)
{
    auto it = obj.find(key);
    if (it == obj.end() || !it->isString()) return std::nullopt;
    const QString v = it->toString().trimmed();
    return v.isEmpty() ? std::nullopt : std::optional<QString>(v);
};

std::optional <bool> json::boolean(const QJsonObject& obj, const char* key)
{
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    if (it->isBool()) return it->toBool();
    if (it->isString()) {
        const auto v = it->toString().trimmed().toLower();
        if (v == "true" || v == "1")  return true;
        if (v == "false"|| v == "0")  return false;
    }
    return std::nullopt;
};

std::optional <QDate> json::date(const QJsonObject& obj, const char* key, const char* fmt)
{
    auto it = obj.find(key);
    if (it == obj.end() || !it->isString()) return std::nullopt;
    const QDate dt = QDate::fromString(it->toString(), fmt);
    return dt.isValid() ? std::optional<QDate>(dt) : std::nullopt;
};

std::optional <QJsonObject> json::object(const QJsonObject& obj, const char* key)
{
    auto it = obj.find(key);
    return (it != obj.end() && it->isObject()) ? std::optional <QJsonObject>(it->toObject())
                                               : std::nullopt;
};

std::optional <int64_t> json::integer(const QJsonObject& obj, const char* key)
{
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    if (it->isDouble()) return it->toInteger();
    if (it->isString()) {
        bool ok = false;
        int v = it->toString().remove(',').toLongLong(&ok);
        if (ok) return v;
    }
    return std::nullopt;
};

std::optional <double> json::real(const QJsonObject& obj, const char* key)
{
    auto it = obj.find(key);
    if (it == obj.end()) return std::nullopt;
    if (it->isDouble()) return it->toDouble();
    if (it->isString()) {
        bool ok = false;
        double d = it->toString().toDouble(&ok);
        if (ok) return d;
    }
    return std::nullopt;
};
