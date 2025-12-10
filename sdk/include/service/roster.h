#ifndef SDK_SERVICE_ROSTER_H
#define SDK_SERVICE_ROSTER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>
#include <QtCore/QDataStream>
#include <vector>
#include "sdk_def.h"
#include "user/account/account.h"

class SDK;

class sdk::Roster : public QObject
{
    Q_OBJECT
public:
    Roster(QObject* parent = nullptr);

    Q_INVOKABLE Identity* find(QString login);

    Q_INVOKABLE QStringList knownUsers() const;

    Q_INVOKABLE bool usernameTaken(const QString& username) const;
    Q_INVOKABLE bool emailTaken(const QString& email) const;

    Identity registerAccount(const QString& username, const QString& email,
                             const QString& password, QString* error = nullptr);
    bool authenticate(const QString& login, const QString& password, Identity* target,
                      QString* error = nullptr) const;

    Account* accountFor(const Identity& identity);
    const Account* accountFor(const Identity& identity) const;

    QString storageRoot() const;
    void saveMeta() const;

private:
    uint16_t _last_index = 0;

    std::vector <Account> _accounts;
    QHash<QString, int> _loginIndex;

    void appendAccount(Account&& account);
    QString sanitizeLogin(const QString& login) const;
    QString accountFileName(const Identity& identity) const;
    QString accountFilePath(const Identity& identity) const;
    bool loadAccountFile(const QString& path);
    bool loadLegacyMeta(QDataStream& stream);
    void saveAccountFile(const Account& account) const;

    void loadMeta();
    friend class ::SDK;
};

#endif // SDK_SERVICE_ROSTER_H
