#ifndef DATA_USER_INFO_H
#define DATA_USER_INFO_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

namespace data { class User; }
namespace data::user { class Info; }

class data::user::Info : public QObject
{
    Q_OBJECT
public:
    static Info* instance();

private:
    Info(QObject* parent = nullptr);
    Info& operator = (const Info&) = delete;

    friend class data::User;
};

#endif
