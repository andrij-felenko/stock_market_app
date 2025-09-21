#ifndef SDK_SERVICES_FINDER_H
#define SDK_SERVICES_FINDER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "core/security/instrument.h"
#include "core/security/listing/ticker.h"


class sdk::Finder final
{
public:
    Finder(const sdk::Symbol& symbol);

    sdk::Symbol symbol() const;
    Ticker* ticker = nullptr;
    Instrument* instrument = nullptr;

    Ticker* operator->() const noexcept;
    bool found() const;
    bool exist() const;

    bool ensure();

private:
    sdk::Symbol _symbol;
};

#endif // SDK_SERVICES_FINDER_H
