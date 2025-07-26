#ifndef DATA_USER_H
#define DATA_USER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "instrument.h"
#include "user/stock.h"
#include "user/info.h"

namespace data { class User; }
namespace model {
    class AssetList;
    class InstrumentList;
}

class data::User : public user::Info
{
    Q_OBJECT
public:
    static User* instance();

    std::vector <data::Instrument*>& favorite_list();
    std::vector <data::Stock*>&         asset_list();

private:
    User(QObject* parent = nullptr);
    User& operator = (const User&) = delete;

    std::vector <data::Instrument*> _favorite_list;
    std::vector <data::Stock*>         _asset_list;

    friend class model::AssetList;
    friend class model::InstrumentList;
};

namespace data { User* account(); }

#endif
