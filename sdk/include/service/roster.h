#ifndef SDK_SERVICE_ROSTER_H
#define SDK_SERVICE_ROSTER_H

#include <QtCore/QObject>
#include "sdk_def.h"
#include "user/account/identity.h"

class SDK;

class sdk::Roster : public QObject
{
    Q_OBJECT
public:
    Roster(QObject* parent = nullptr);

    Identity* find(QString login);

    void saveMeta() const;

private:
    uint16_t _last_index = 0;

    std::vector <Identity> _list;

    void loadMeta();
    friend class ::SDK;
};

#endif // SDK_SERVICE_ROSTER_H
