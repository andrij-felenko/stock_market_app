#ifndef SDK_USER_ACCOUNT_SESSION_H
#define SDK_USER_ACCOUNT_SESSION_H
/*!
\class Session
\brief Авторизація/сесія: логін, логаут, токени, терміни дії.

\details
Робота з обліковими даними (credentials), access/refresh токенами,
оновлення сесії, вихід. Може посилатися на \ref Account для ініціалізаційних дій,
але не змішує збереження профілю/реєстру з механікою входу.

\section session_usage Приклад
\code
Session sess(provider);
if (sess.login(user, pass)) {
    Account acc = sess.loadAccount();
    render(acc.ledger().portfolio());
}
\endcode
*/

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QHash>
#include <QtCore/QStringList>
#include "sdk_def.h"
#include "service/roster.h"

class sdk::Session : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool authenticated READ authenticated NOTIFY sessionChanged)
    Q_PROPERTY(QString username READ username NOTIFY sessionChanged)
    Q_PROPERTY(QString email READ email NOTIFY sessionChanged)
    Q_PROPERTY(QDateTime accessExpiry READ accessExpiry NOTIFY tokensChanged)
    Q_PROPERTY(QDateTime refreshExpiry READ refreshExpiry NOTIFY tokensChanged)
    Q_PROPERTY(QString accessToken READ accessToken NOTIFY tokensChanged)
    Q_PROPERTY(QString refreshToken READ refreshToken NOTIFY tokensChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorChanged)
    Q_PROPERTY(QStringList rememberedUsers READ rememberedUsers NOTIFY rememberedUsersChanged)
public:
    struct SessionTokens {
        QByteArray accessToken;
        QByteArray refreshToken;
        QDateTime accessExpiry;
        QDateTime refreshExpiry;
    };

    explicit Session(QObject* parent = nullptr);

    void attachRoster(Roster* roster);

    Q_INVOKABLE bool login(const QString& login, const QString& password);
    Q_INVOKABLE bool registerAccount(const QString& username, const QString& email,
                                     const QString& password);
    Q_INVOKABLE bool refreshAccess();
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool activateRemembered(const QString& login);

    bool authenticated() const;
    QString username() const;
    QString email() const;
    QString lastError() const;

    QStringList rememberedUsers() const;

    QString accessToken() const;
    QString refreshToken() const;
    QDateTime accessExpiry() const;
    QDateTime refreshExpiry() const;

    bool validateAccessToken(const QByteArray& token) const;
    bool validateRefreshToken(const QByteArray& token) const;

signals:
    void sessionChanged();
    void tokensChanged();
    void errorChanged(QString error);
    void rememberedUsersChanged();

protected:
    SessionTokens buildTokens() const;
    void applyTokens(const SessionTokens& tokens);
    QByteArray generateToken() const;
    void clearError();
    void setError(const QString& error);
    void rebuildIndexes(const Identity& identity);
    Roster* roster() const;

private:
    void issueTokens();
    void rememberTokens(const SessionTokens& tokens);
    void loadCachedSessions();
    QString sessionStorePath() const;
    QString normalizedLogin(const QString& login) const;
    QString sanitizeLogin(const QString& login) const;

    Identity _identity;
    QByteArray _token_access; // short token, valid for corrections
    QByteArray _token_refresh; // long token, read access
    QDateTime _expired_short;
    QDateTime _expired_long;
    QString _error;
    Roster* _roster = nullptr;

    QHash<QString, SessionTokens> _cachedSessions;
};

#endif // SDK_USER_ACCOUNT_SESSION_H
