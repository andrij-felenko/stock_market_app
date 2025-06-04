#include "eod.h"
#include <QDebug>

EodApiClient::EodApiClient(QObject* parent) : QObject(parent)
{
    set_api_key("683ebb8bc59b60.11043967");
}

void EodApiClient::set_api_key(const QString& key) {
    m_api_key = key;
}

void EodApiClient::fetch_ticker_data(const QString& ticker) {
    QUrl url("https://eodhd.com/api/eod/" + ticker);
    QUrlQuery query;
    query.addQueryItem("api_token", m_api_key);
    query.addQueryItem("fmt", "json");
    query.addQueryItem("from", "2025-03-12");
    query.addQueryItem("to", "2025-04-24");
    url.setQuery(query);
    // QUrl url("https://eodhd.com/api/fundamentals/" + ticker);
    // QUrlQuery query;
    // query.addQueryItem("api_token", m_api_key);
    // query.addQueryItem("fmt", "json");
    // url.setQuery(query);
    qDebug() << url;

    QNetworkRequest request(url);

    QNetworkReply* reply = m_manager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit error_occurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonDocument doc = QJsonDocument::fromJson(response);
        qDebug() << response;
        if (!doc.isObject()) {
            emit error_occurred("Invalid JSON received");
            qDebug() << "error" << reply->readAll();
            reply->deleteLater();
            return;
        }

        emit data_ready(doc.object());
        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::readyRead, this, [=, this]() {
        QByteArray response = reply->readAll();
        qDebug() << response;
    });
}
