#ifndef DATA_TRANSACTION_H
#define DATA_TRANSACTION_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data { class Transaction; }

class data::Transaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool  buy   READ buy   CONSTANT)
    Q_PROPERTY(float price READ price CONSTANT)
    Q_PROPERTY(float count READ count CONSTANT)
    Q_PROPERTY(QString   broker READ broker CONSTANT)
    Q_PROPERTY(QDateTime dtime  READ dtime  CONSTANT)
public:
    Transaction(QObject* parent = nullptr);

    bool buy() const;
    float price() const;
    float count() const;
    QString broker() const;
    QDateTime dtime() const;

signals:
    void signal_update();

private:
    bool  _buy;
    float _price;
    float _count;
    QString   _broker;
    QDateTime _dtime;

    friend QDataStream& operator << (QDataStream& s, const Transaction& d);
    friend QDataStream& operator >> (QDataStream& s,       Transaction& d);
};

#endif // DATA_TRANSACTION_H
