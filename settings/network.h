#ifndef SETTINGS_NETWORK_H
#define SETTINGS_NETWORK_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

namespace settings {
    class Network;
    Network* network();
}

class settings::Network : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key_alpha_vantage READ key_av WRITE setAlphaVantageKey NOTIFY key_avChanged)
    Q_PROPERTY(QString key_eod READ key_eod WRITE setEodKey NOTIFY key_eodChanged)
    Q_PROPERTY(QString key_finnhub READ key_fh WRITE setFinnHubKey NOTIFY key_fhChanged)
    Q_PROPERTY(QString key_open_ai READ key_oai WRITE setOpenaiKey NOTIFY key_openaiChanged)
    Q_PROPERTY(QString key_twelve_data READ key_td WRITE setTwelveDataKey NOTIFY key_tdChanged)
    Q_PROPERTY(QString key_marketstack READ key_ms WRITE setMarketStackKey NOTIFY key_msChanged)
public:
    static Network* instance();

    QString key_av()  const;
    QString key_eod() const;
    QString key_fh()  const;
    QString key_oai() const;
    QString key_td()  const;
    QString key_ms() const;

public slots:
    void setAlphaVantageKey(const QString& new_key);
    void setEodKey         (const QString& new_key);
    void setFinnHubKey     (const QString& new_key);
    void setOpenaiKey      (const QString& new_key);
    void setTwelveDataKey  (const QString& new_key);
    void setMarketStackKey (const QString& new_key);

signals:
    void key_avChanged    (QString key);
    void key_eodChanged   (QString key);
    void key_fhChanged    (QString key);
    void key_openaiChanged(QString key);
    void key_tdChanged    (QString key);
    void key_msChanged    (QString key);

private:
    Network(QObject* parent = nullptr);
    Network& operator = (const Network&) = delete;
};

#endif // SETTINGS_NETWORK_H
