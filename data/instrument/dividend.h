#ifndef DATA_TICKER_DIVIDEND_H
#define DATA_TICKER_DIVIDEND_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QDataStream>

namespace data {
    class Dividend;

    struct DividendList {
        QDate date;
        float amount;
    };
}

/*!
 * \class data::Dividend
 * \brief Політика виплат акціонерам та графік дивідендів.
 *
 * \property data::Dividend::yield
 * Дивідендна дохідність: Річний дивіденд на акцію / Поточна ціна. Орієнтир грошового повернення
 * інвестору «тут і зараз»; змінюється з ціною.
 *
 * \property data::Dividend::per_share
 * Річний дивіденд на акцію (екстрапольований або оголошений).
 * База для yield; інколи компанії мають нерівномірні виплати (квартал/півріччя/спецдивіденди).
 *
 * \property data::Dividend::pay_ratio
 * Payout ratio — частка прибутку, що виплачується дивідендами: Дивіденди / Net Income.
 * Високий рівень може обмежувати інвестиції в зростання;
 * дуже низький — сигнал до реінвестування або відсутності стабільного cash flow.
 *
 * \property data::Dividend::next_date
 * Дата найближчої ключової події (зазвичай Ex-Dividend Date).
 * Для права на дивіденд інвестор має володіти акцією до дати «ex-».
 *
 * \property data::Dividend::prev_date
 * Дата останньої виплати (Dividend/Pay Date).
 * Дає історичний контекст стабільності/регулярності виплат.
 *
 * \note \c set_history()
 * Дозволяє накопичувати історію дат/сум для аналізу стабільності дивідендного потоку.
 */
class data::Dividend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float yield     READ yield     WRITE set_yield     NOTIFY     yieldChanged)
    Q_PROPERTY(float per_share READ per_share WRITE set_per_share NOTIFY per_shareChanged)
    Q_PROPERTY(float pay_ratio READ pay_ratio WRITE set_pay_ratio NOTIFY pay_ratioChanged)
    Q_PROPERTY(QDate next_date READ next_date WRITE set_next_date NOTIFY next_dateChanged)
    Q_PROPERTY(QDate prev_date READ prev_date WRITE set_prev_date NOTIFY prev_dateChanged)
public:
    Dividend(QObject* parent = nullptr);

    float yield()     const;
    float per_share() const;
    float pay_ratio() const;
    QDate next_date() const;
    QDate prev_date() const;

    void set_history(QDate date, float f);

public slots:
    void set_yield    (float new_yield);
    void set_per_share(float new_per_share);
    void set_pay_ratio(float new_payout_ratio);
    void set_next_date(const QDate& new_next_date);
    void set_prev_date(const QDate& new_prev_date);

signals:
    void     yieldChanged(float yield);
    void per_shareChanged(float per_share);
    void pay_ratioChanged(float payout_ratio);
    void next_dateChanged(QDate next_date);
    void prev_dateChanged(QDate prev_date);

private:
    float _yield = 0;
    float _per_share = 0;
    float _pay_ratio = 0;
    QDate _next_date;
    QDate _prev_date;

    std::vector <DividendList> _history;

    friend QDataStream& operator << (QDataStream& s, const Dividend& d);
    friend QDataStream& operator >> (QDataStream& s,       Dividend& d);
};

#endif
