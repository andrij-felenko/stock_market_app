#include "service/roster.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

sdk::Roster::Roster(QObject* parent) : QObject(parent)
{
    //
}

sdk::Identity* sdk::Roster::find(QString login)
{
    for (auto& it : _list)
        if (it.email() == login || it.username() == login)
            return &it;
    return nullptr;
}

void sdk::Roster::loadMeta()
{
    qDebug() << Q_FUNC_INFO;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                   "/directory/";
    QDir().mkpath(path);

    QFile file(path + "meta.edsm");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "can`t open" << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    int32_t size = 0; out >> size;
    if (size > 100) _list.reserve(size + 25000);
    else _list.reserve(size + 5);
    for (int i = 0; i < size; i++){
        _list.emplace_back(Identity());
        out >> _list.back();
    }

    file.close();
}

void sdk::Roster::saveMeta() const
{
    qDebug() << Q_FUNC_INFO;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                   "/directory/";
    QDir().mkpath(path);

    QFile file(path + "meta.edsm");
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qDebug() << "can`t open" << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    out << int32_t(_list.size());
    qDebug() << Q_FUNC_INFO << "WRITE SAVE SIZE" << int32_t(_list.size());

    int32_t index = 0;
    for (const auto& it : std::as_const(_list))
        out << it;

    file.close();
}
