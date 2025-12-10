#include "service/roster.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QRegularExpression>
#include <QtCore/QStandardPaths>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QDebug>

namespace {
    QByteArray hashPassword(const QString& password, const QString& salt)
    {
        auto data = QString("%1:%2").arg(password, salt).toUtf8();
        return QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    }

    bool looksLikeEmail(const QString& email)
    {
        static const QRegularExpression re("^[^@\\s]+@[^@\\s]+\\.[^@\\s]+$");
        return re.match(email).hasMatch();
    }
}

sdk::Roster::Roster(QObject* parent) : QObject(parent)
{
    //
}

QString sdk::Roster::storageRoot() const
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/roster/";
    QDir dir(path);
    dir.mkpath(".");
    return path;
}

QString sdk::Roster::sanitizeLogin(const QString& login) const
{
    QString cleaned = login.trimmed().toLower();
    QString safe;
    safe.reserve(cleaned.size());
    for (QChar c : cleaned) {
        safe.append(c.isLetterOrNumber() ? c : '_');
    }
    if (safe.isEmpty())
        safe = "user";
    return safe;
}

QString sdk::Roster::accountFileName(const Identity& identity) const
{
    const QString baseLogin = identity.username().isEmpty() ? identity.email() : identity.username();
    return sanitizeLogin(baseLogin) + ".account";
}

QString sdk::Roster::accountFilePath(const Identity& identity) const
{
    return storageRoot() + accountFileName(identity);
}

void sdk::Roster::appendAccount(Account&& account)
{
    const int newIndex = static_cast<int>(_accounts.size());
    _accounts.push_back(std::move(account));
    const auto& id = _accounts.back().profile.identity;
    _loginIndex.insert(id.username().toLower(), newIndex);
    _loginIndex.insert(id.email().toLower(), newIndex);
    if (id.id() > _last_index)
        _last_index = id.id();
}

void sdk::Roster::saveAccountFile(const Account& account) const
{
    const QString path = accountFilePath(account.profile.identity);
    QFile file(path);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        qWarning() << "Roster: can't save account" << path << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);
    out << account;
}

bool sdk::Roster::loadAccountFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Roster: can't open account file" << path << file.errorString();
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_10);

    Account account;
    in >> account;

    if (!account.profile.identity.valid())
        return false;

    appendAccount(std::move(account));
    return true;
}

bool sdk::Roster::loadLegacyMeta(QDataStream& stream)
{
    int32_t size = 0; stream >> size;
    if (size <= 0)
        return false;

    if (size > 100) _accounts.reserve(size + 25000);
    else _accounts.reserve(size + 5);

    for (int i = 0; i < size; i++){
        Account account;
        stream >> account;
        if (!account.profile.identity.valid())
            continue;

        appendAccount(std::move(account));
    }
    return !_accounts.empty();
}

sdk::Identity* sdk::Roster::find(QString login)
{
    login = login.trimmed().toLower();
    if (_loginIndex.contains(login))
        return &_accounts[_loginIndex.value(login)].profile.identity;

    for (auto& account : _accounts) {
        if (account.profile.identity.email().compare(login, Qt::CaseInsensitive) == 0 ||
            account.profile.identity.username().compare(login, Qt::CaseInsensitive) == 0)
            return &account.profile.identity;
    }
    return nullptr;
}

bool sdk::Roster::usernameTaken(const QString& username) const
{
    const QString normalized = username.trimmed();
    for (const auto& account : _accounts)
        if (account.profile.identity.username().compare(normalized, Qt::CaseInsensitive) == 0)
            return true;
    return false;
}

bool sdk::Roster::emailTaken(const QString& email) const
{
    const QString normalized = email.trimmed();
    for (const auto& account : _accounts)
        if (account.profile.identity.email().compare(normalized, Qt::CaseInsensitive) == 0)
            return true;
    return false;
}

sdk::Identity sdk::Roster::registerAccount(const QString& username, const QString& email,
                                           const QString& password, QString* error)
{
    QString trimmedName = username.trimmed();
    QString trimmedMail = email.trimmed();

    if (trimmedName.isEmpty()) {
        if (error) *error = tr("Username is required");
        return {};
    }

    if (!looksLikeEmail(trimmedMail)) {
        if (error) *error = tr("Email looks invalid");
        return {};
    }

    if (password.size() < 8) {
        if (error) *error = tr("Password must be at least 8 characters");
        return {};
    }

    if (usernameTaken(trimmedName)) {
        if (error) *error = tr("Username already exists");
        return {};
    }

    if (emailTaken(trimmedMail)) {
        if (error) *error = tr("Email already exists");
        return {};
    }

    Identity identity(++_last_index);
    identity.setUsername(trimmedName);
    identity.setEmail(trimmedMail);
    identity.setHash(hashPassword(password, trimmedMail));

    Account account;
    account.profile.setIdentity(identity);
    appendAccount(std::move(account));
    saveAccountFile(_accounts.back());
    saveMeta();

    if (error) error->clear();
    return identity;
}

bool sdk::Roster::authenticate(const QString& login, const QString& password, Identity* target,
                               QString* error) const
{
    const QString trimmedLogin = login.trimmed().toLower();
    int index = _loginIndex.value(trimmedLogin, -1);

    if (index < 0) {
        for (int i = 0; i < _accounts.size(); ++i) {
            const auto& id = _accounts.at(i).profile.identity;
            if (id.username().compare(trimmedLogin, Qt::CaseInsensitive) == 0 ||
                id.email().compare(trimmedLogin, Qt::CaseInsensitive) == 0) {
                index = i;
                break;
            }
        }
    }
    if (index < 0) {
        if (error) *error = tr("Account not found");
        return false;
    }

    const auto& account = _accounts.at(index);
    const auto candidate = hashPassword(password, account.profile.identity.email());
    if (!account.profile.identity.matchesHash(candidate)) {
        if (error) *error = tr("Invalid credentials");
        return false;
    }

    if (target)
        *target = account.profile.identity;
    if (error) error->clear();
    return true;
}

sdk::Account* sdk::Roster::accountFor(const Identity& identity)
{
    auto it = _loginIndex.constFind(identity.email().toLower());
    if (it != _loginIndex.constEnd())
        return &_accounts[it.value()];
    it = _loginIndex.constFind(identity.username().toLower());
    if (it != _loginIndex.constEnd())
        return &_accounts[it.value()];
    for (auto& account : _accounts)
        if (account.profile.identity.id() == identity.id())
            return &account;
    return nullptr;
}

const sdk::Account* sdk::Roster::accountFor(const Identity& identity) const
{
    return const_cast<Roster*>(this)->accountFor(identity);
}

void sdk::Roster::loadMeta()
{
    qDebug() << Q_FUNC_INFO;
    QString path = storageRoot();
    QDir dir(path);
    auto files = dir.entryList({"*.account"}, QDir::Files);
    for (const auto& file : files)
        loadAccountFile(dir.filePath(file));
    const bool alreadyLoaded = !_accounts.empty();

    QFile file(path + "meta.edsm");
    if (!file.exists()) {
        const QString legacyPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                   "/directory/";
        QFile legacyFile(legacyPath + "meta.edsm");
        if (legacyFile.exists()) {
            file.setFileName(legacyFile.fileName());
            path = legacyPath;
            dir.setPath(path);
            files = dir.entryList({"*.account"}, QDir::Files);
            for (const auto& f : files)
                loadAccountFile(dir.filePath(f));
        }
    }

    if (!file.open(QIODevice::ReadOnly)){
        if (_accounts.empty())
            qDebug() << "Roster meta not found, starting new.";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    quint32 magic = 0;
    bool legacyLoaded = false;
    out >> magic;
    if (magic == 0x52535452) { // 'RSTR'
        quint16 version = 0; out >> version;
        quint16 storedIndex = 0; out >> storedIndex;
        if (storedIndex > _last_index)
            _last_index = storedIndex;

        int32_t size = 0; out >> size;
        if (!alreadyLoaded) {
            for (int i = 0; i < size; ++i) {
                QString filename; out >> filename;
                loadAccountFile(path + filename);
            }
        }
    } else {
        _last_index = static_cast<uint16_t>(magic);
        if (!loadLegacyMeta(out))
            qDebug() << "Legacy roster meta is empty";
        else
            legacyLoaded = true;
    }

    file.close();

    if (legacyLoaded)
        saveMeta();
}

void sdk::Roster::saveMeta() const
{
    qDebug() << Q_FUNC_INFO;
    const QString path = storageRoot();
    QDir dir(path);
    dir.mkpath(".");

    for (const auto& account : _accounts)
        saveAccountFile(account);

    QFile file(path + "meta.edsm");
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qDebug() << "can`t open" << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    out << quint32(0x52535452); // RSTR
    out << quint16(1);
    out << _last_index;
    out << int32_t(_accounts.size());

    for (const auto& it : std::as_const(_accounts))
        out << accountFileName(it.profile.identity);

    file.close();
}

QStringList sdk::Roster::knownUsers() const
{
    QStringList users;
    for (const auto& account : _accounts)
        users << (account.profile.identity.username().isEmpty()
                    ? account.profile.identity.email()
                    : account.profile.identity.username());
    return users;
}
