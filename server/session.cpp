#include "session.h"
#include "nexus.h"

#include <QtCore/QDateTime>

Session::Session(QObject* parent) : sdk::Session(parent)
{
    attachRoster(DB->roster());
}

bool Session::ensureRosterAttached()
{
    if (roster() == nullptr) {
        setError(tr("Roster is not attached"));
        return false;
    }
    return true;
}

Session::DeviceSession* Session::sessionFor(const QString& deviceId)
{
    auto it = _devices.find(deviceId);
    return it == _devices.end() ? nullptr : &it.value();
}

const Session::DeviceSession* Session::sessionFor(const QString& deviceId) const
{
    auto it = _devices.constFind(deviceId);
    return it == _devices.cend() ? nullptr : &it.value();
}

bool Session::tokensValid(const SessionTokens& tokens, const QByteArray& token, bool refresh) const
{
    const QDateTime& expiry = refresh ? tokens.refreshExpiry : tokens.accessExpiry;
    const QByteArray& expected = refresh ? tokens.refreshToken : tokens.accessToken;
    return !expiry.isNull() && expiry > QDateTime::currentDateTimeUtc() && expected == token;
}

void Session::registerDevice(const QString& deviceId, const Identity& identity, const SessionTokens& tokens)
{
    _devices.insert(deviceId, {identity, tokens});
    _userDevices[identity.username()].insert(deviceId);
}

void Session::unregisterDevice(const QString& deviceId)
{
    auto it = _devices.find(deviceId);
    if (it == _devices.end())
        return;
    const QString username = it->identity.username();
    _devices.erase(it);
    auto userIt = _userDevices.find(username);
    if (userIt != _userDevices.end()) {
        userIt->remove(deviceId);
        if (userIt->isEmpty())
            _userDevices.erase(userIt);
    }
}

bool Session::loginDevice(const QString& deviceId, const QString& login, const QString& password)
{
    if (deviceId.isEmpty()) {
        setError(tr("Device id is required"));
        return false;
    }

    if (!ensureRosterAttached())
        return false;

    Identity identity;
    QString error;
    if (!roster()->authenticate(login, password, &identity, &error)) {
        setError(error);
        return false;
    }

    registerDevice(deviceId, identity, buildTokens());
    clearError();
    return true;
}

bool Session::refreshDeviceAccess(const QString& deviceId)
{
    auto device = sessionFor(deviceId);
    if (device == nullptr) {
        setError(tr("Unknown device session"));
        return false;
    }

    if (!tokensValid(device->tokens, device->tokens.refreshToken, true)) {
        setError(tr("Refresh token is invalid or expired"));
        return false;
    }

    device->tokens = buildTokens();
    clearError();
    return true;
}

void Session::logoutDevice(const QString& deviceId)
{
    unregisterDevice(deviceId);
}

void Session::logoutAllDevices(const QString& login)
{
    const auto devices = _userDevices.value(login);
    for (const auto& deviceId : devices)
        _devices.remove(deviceId);
    _userDevices.remove(login);
}

bool Session::validateAccessFor(const QString& deviceId, const QByteArray& token) const
{
    const auto* device = sessionFor(deviceId);
    return device != nullptr && tokensValid(device->tokens, token, false);
}

bool Session::validateRefreshFor(const QString& deviceId, const QByteArray& token) const
{
    const auto* device = sessionFor(deviceId);
    return device != nullptr && tokensValid(device->tokens, token, true);
}

QStringList Session::devicesForUser(const QString& login) const
{
    return _userDevices.value(login).values();
}
