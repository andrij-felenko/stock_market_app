#ifndef STOCK_MANAGER_SERVER_SESSION_H
#define STOCK_MANAGER_SERVER_SESSION_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <StockSDK/user/account/session.h>

class Session : public sdk::Session
{
    Q_OBJECT
public:
    Session(QObject* parent = nullptr);

    Q_INVOKABLE bool loginDevice(const QString& deviceId, const QString& login, const QString& password);
    Q_INVOKABLE bool refreshDeviceAccess(const QString& deviceId);
    Q_INVOKABLE void logoutDevice(const QString& deviceId);
    Q_INVOKABLE void logoutAllDevices(const QString& login);

    bool validateAccessFor(const QString& deviceId, const QByteArray& token) const;
    bool validateRefreshFor(const QString& deviceId, const QByteArray& token) const;
    QStringList devicesForUser(const QString& login) const;

private:
    struct DeviceSession {
        Identity identity;
        SessionTokens tokens;
    };

    bool ensureRosterAttached();
    DeviceSession* sessionFor(const QString& deviceId);
    const DeviceSession* sessionFor(const QString& deviceId) const;
    bool tokensValid(const SessionTokens& tokens, const QByteArray& token, bool refresh) const;
    void registerDevice(const QString& deviceId, const Identity& identity, const SessionTokens& tokens);
    void unregisterDevice(const QString& deviceId);

    QHash<QString, DeviceSession> _devices; // deviceId -> session
    QHash<QString, QSet<QString>> _userDevices; // username -> deviceIds
};

#endif
