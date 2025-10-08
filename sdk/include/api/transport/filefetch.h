#ifndef SDK_API_TRANSPORT_FILEFETCH_H
#define SDK_API_TRANSPORT_FILEFETCH_H

#include "api/transport/provider.h"
#include "core/isin.h"

class sdk::api::FileFetch : public Provider {
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
    using api::Provider::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& symbol,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;
};

namespace sdk { constexpr inline Singleton <api::FileFetch> apiFileFetch {}; }

#endif // SDK_API_TRANSPORT_FILEFETCH_H
