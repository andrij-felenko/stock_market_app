// #ifndef DATA_USER_INFO_H
// #define DATA_USER_INFO_H

// #include <QtCore/QObject>
// #include <QtCore/QDate>
// #include <QtCore/QUrl>
// #include <QtCore/QAbstractListModel>
// #include <QtCore/QTimer>

// namespace data { class User; }
// namespace data::user { class Info; }

// class data::user::Info : public QObject
// {
//     Q_OBJECT
//     Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
//     Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged FINAL)
//     Q_PROPERTY(bool guest READ guest NOTIFY guestChanged FINAL)
// public:
//     static Info* instance();

//     QString username() const;
//     QString email() const;
//     bool guest() const;

// public slots:
//     void setUsername(const QString& newName);
//     void setEmail(const QString& newEmail);

// signals:
//     void usernameChanged(QString name);
//     void emailChanged(QString email);
//     void guestChanged(bool guest);

// private:
//     Info(QObject* parent = nullptr);
//     Info& operator = (const Info&) = delete;
//     void clear();

//     QString _username;
//     QString _email;

//     friend QDataStream& operator << (QDataStream& s, const Info& d);
//     friend QDataStream& operator >> (QDataStream& s,       Info& d);

//     friend class data::User;
// };

// #endif
