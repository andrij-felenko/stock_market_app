#ifndef API_EOD_H
#define API_EOD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>

class EodApiClient : public QObject {
    Q_OBJECT

public:
    explicit EodApiClient(QObject* parent = nullptr);
    void set_api_key(const QString& key);
    void fetch_ticker_data(const QString& ticker);

signals:
    void data_ready(const QJsonObject& data);
    void error_occurred(const QString& message);

private:
    QNetworkAccessManager m_manager;
    QString m_api_key;
};

#endif
