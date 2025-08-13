#ifndef API_FILELOADER_H
#define API_FILELOADER_H

#include "api/api.h"

namespace api { class FileFetch; }

class api::FileFetch : public API {
    Q_OBJECT
public:
    static FileFetch* instance();

    // Зручний статичний виклик
    static void fetch_logo(QString tag, const QUrl& url) {
        StringMap k; k["url"] = url.toString();
        instance()->_send(Request::Text, std::move(tag), std::move(k));
    }

private:
    explicit FileFetch(QObject* parent = nullptr);

    bool _request(Request type, QString name, StringMap keys = {}) override;
    void _handler_answer(Request type, QByteArray data,
                         QString name, bool stream = false) override;

    // Опційно: зберегти копію в кеш-файлі (AppDataLocation/logos)
    static void save_cache(const QString& tag, const QByteArray& bytes, const QString& ext = "png");
};

#endif // API_FILELOADER_H
