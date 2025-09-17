#ifndef API_FILELOADER_H
#define API_FILELOADER_H

#include "api/api.h"
#include "instrument/isin.h"

namespace api { class FileFetch; }

class api::FileFetch : public API {
    Q_OBJECT
public:
    static FileFetch* instance();

    // Зручний статичний виклик
    static void fetch_logo(const sdk::Isin& isin, const QUrl& url) {
        StringMap k; k["url"] = url.toString();
        instance()->_request(Request::Logo, QString::fromLatin1(~isin), std::move(k));
    }

private:
    explicit FileFetch(QObject* parent = nullptr);
    using api::API::_request;

    bool _request(Request type, const QString& name, const sdk::Symbol& symbol,
                  StringMap keys = {}) override;
    void _handler_answer(Reply* reply) override;

    // Опційно: зберегти копію в кеш-файлі (AppDataLocation/logos)
    static void save_cache(const QString& tag, const QByteArray& bytes, const QString& ext = "png");
};

#endif // API_FILELOADER_H
