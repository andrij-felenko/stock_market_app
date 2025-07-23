#ifndef DATA_USER_H
#define DATA_USER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

namespace data { class User; }

class data::User : public QObject
{
    Q_OBJECT
public:
    static User* instance();

private:
    User(QObject* parent = nullptr);
    User& operator = (const User&) = delete;
};

#endif
