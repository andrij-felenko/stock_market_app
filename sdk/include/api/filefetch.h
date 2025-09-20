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
    static void fetchLogo(const sdk::Isin& isin, const QUrl& url) {
        StringMap k; k["url"] = url.toString();
        instance()->request(Request::Logo, QString::fromLatin1(~isin), std::move(k));
    }

private:
    explicit FileFetch(QObject* parent = nullptr);
    using api::API::request;

    bool request(Request type, const QString& name, const sdk::Symbol& symbol,
                 StringMap keys = {}) override;
    void handlerAnswer(Reply* reply) override;
};

#endif // API_FILELOADER_H
