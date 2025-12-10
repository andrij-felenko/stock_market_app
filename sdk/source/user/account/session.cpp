#include "user/account/session.h"

#include <QtCore/QRandomGenerator>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QDebug>

sdk::Session::Session(QObject* parent) : QObject(parent)
{
}

sdk::Session::SessionTokens sdk::Session::buildTokens() const
{
    SessionTokens tokens;
    tokens.accessToken  = generateToken();
    tokens.refreshToken = generateToken();
    tokens.accessExpiry = QDateTime::currentDateTimeUtc().addSecs(30 * 60); // 30 minutes
    tokens.refreshExpiry = QDateTime::currentDateTimeUtc().addDays(90);     // 90 days
    return tokens;
}

void sdk::Session::attachRoster(Roster* roster)
{
    _roster = roster;
    loadCachedSessions();
}

Roster* sdk::Session::roster() const
{
    return _roster;
}

void sdk::Session::clearError()
{
    if (!_error.isEmpty()) {
        _error.clear();
        emit errorChanged(_error);
    }
}

void sdk::Session::setError(const QString& error)
{
    if (_error == error)
        return;
    _error = error;
    emit errorChanged(_error);
}

QByteArray sdk::Session::generateToken() const
{
    QByteArray bytes(32, Qt::Uninitialized);
    QRandomGenerator::global()->generate(reinterpret_cast<quint32*>(bytes.data()), bytes.size() / 4);
    return bytes.toBase64();
}

void sdk::Session::applyTokens(const SessionTokens& tokens)
{
    _token_access  = tokens.accessToken;
    _token_refresh = tokens.refreshToken;
    _expired_short = tokens.accessExpiry;
    _expired_long  = tokens.refreshExpiry;
    emit tokensChanged();
}

void sdk::Session::issueTokens()
{
    auto tokens = buildTokens();
    applyTokens(tokens);
    rememberTokens(tokens);
}

bool sdk::Session::login(const QString& login, const QString& password)
{
    if (_roster == nullptr) {
        setError(tr("Roster is not attached"));
        return false;
    }

    Identity identity;
    if (!_roster->authenticate(login, password, &identity, &_error)) {
        emit errorChanged(_error);
        return false;
    }

    rebuildIndexes(identity);
    clearError();
    emit sessionChanged();
    issueTokens();
    return true;
}

bool sdk::Session::registerAccount(const QString& username, const QString& email,
                                   const QString& password)
{
    if (_roster == nullptr) {
        setError(tr("Roster is not attached"));
        return false;
    }

    const Identity identity = _roster->registerAccount(username, email, password, &_error);
    if (!identity.valid()) {
        emit errorChanged(_error);
        return false;
    }

    rebuildIndexes(identity);
    clearError();
    emit sessionChanged();
    issueTokens();
    return true;
}

bool sdk::Session::refreshAccess()
{
    if (!validateRefreshToken(_token_refresh)) {
        setError(tr("Refresh token is invalid or expired"));
        return false;
    }

    issueTokens();
    clearError();
    return true;
}

void sdk::Session::logout()
{
    _identity = Identity();
    _token_access.clear();
    _token_refresh.clear();
    _expired_short = QDateTime();
    _expired_long = QDateTime();
    emit sessionChanged();
    emit tokensChanged();
}

QStringList sdk::Session::rememberedUsers() const
{
    return _cachedSessions.keys();
}

bool sdk::Session::authenticated() const
{
    return _identity.valid() && validateAccessToken(_token_access);
}

QString sdk::Session::username() const { return _identity.username(); }
QString sdk::Session::email() const { return _identity.email(); }
QString sdk::Session::lastError() const { return _error; }

QString sdk::Session::accessToken() const { return QString::fromUtf8(_token_access); }
QString sdk::Session::refreshToken() const { return QString::fromUtf8(_token_refresh); }
QDateTime sdk::Session::accessExpiry() const { return _expired_short; }
QDateTime sdk::Session::refreshExpiry() const { return _expired_long; }

bool sdk::Session::validateAccessToken(const QByteArray& token) const
{
    return !_expired_short.isNull() && _expired_short > QDateTime::currentDateTimeUtc() &&
           token == _token_access;
}

bool sdk::Session::validateRefreshToken(const QByteArray& token) const
{
    return !_expired_long.isNull() && _expired_long > QDateTime::currentDateTimeUtc() &&
           token == _token_refresh;
}

void sdk::Session::rebuildIndexes(const Identity& identity)
{
    _identity = identity;
}

void sdk::Session::rememberTokens(const SessionTokens& tokens)
{
    const QString key = normalizedLogin(!_identity.username().isEmpty() ? _identity.username()
                                      : _identity.email());
    if (key.isEmpty())
        return;

    _cachedSessions.insert(key, tokens);

    const QString path = sessionStorePath();
    const QString filename = path + sanitizeLogin(key) + ".session";
    QFile file(filename);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        qWarning() << "Unable to persist session" << filename << file.errorString();
        emit rememberedUsersChanged();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);
    out << key << tokens.accessToken << tokens.refreshToken << tokens.accessExpiry << tokens.refreshExpiry;
    emit rememberedUsersChanged();
}

QString sdk::Session::sessionStorePath() const
{
    QString base;
    if (_roster)
        base = _roster->storageRoot();
    if (base.isEmpty())
        base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/roster/";
    QDir dir(base + "sessions/");
    dir.mkpath(".");
    return dir.path() + "/";
}

QString sdk::Session::sanitizeLogin(const QString& login) const
{
    QString cleaned = login;
    QString safe;
    safe.reserve(cleaned.size());
    for (QChar c : cleaned) {
        safe.append(c.isLetterOrNumber() ? c : '_');
    }
    if (safe.isEmpty())
        safe = "user";
    return safe;
}

QString sdk::Session::normalizedLogin(const QString& login) const
{
    return login.trimmed().toLower();
}

void sdk::Session::loadCachedSessions()
{
    const QString path = sessionStorePath();
    QDir dir(path);
    const auto files = dir.entryList({"*.session"}, QDir::Files);
    for (const auto& file : files) {
        QFile f(dir.filePath(file));
        if (!f.open(QIODevice::ReadOnly))
            continue;

        QDataStream in(&f);
        in.setVersion(QDataStream::Qt_6_10);

        SessionTokens tokens;
        QString key;
        in >> key >> tokens.accessToken >> tokens.refreshToken >> tokens.accessExpiry >> tokens.refreshExpiry;
        if (key.isEmpty())
            continue;
        _cachedSessions.insert(normalizedLogin(key), tokens);
    }

    if (!_cachedSessions.isEmpty())
        emit rememberedUsersChanged();
}

bool sdk::Session::activateRemembered(const QString& login)
{
    const QString key = normalizedLogin(login);
    if (!_cachedSessions.contains(key)) {
        setError(tr("No saved session for %1").arg(login));
        return false;
    }

    if (_roster == nullptr) {
        setError(tr("Roster is not attached"));
        return false;
    }

    Identity* id = _roster->find(login);
    if (id == nullptr) {
        setError(tr("Account not found"));
        return false;
    }

    const auto tokens = _cachedSessions.value(key);
    if (tokens.refreshExpiry <= QDateTime::currentDateTimeUtc()) {
        setError(tr("Saved session expired"));
        return false;
    }

    rebuildIndexes(*id);
    applyTokens(tokens);
    emit sessionChanged();
    clearError();

    issueTokens(); // rotate tokens to extend validity
    return true;
}
