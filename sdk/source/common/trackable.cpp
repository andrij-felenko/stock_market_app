#include "common/trackable.h"
#include "common/features.h"

bool sdk::Trackable::islock() const { return _.boolflag <0>(); }
void sdk::Trackable::  lock() { _.setBoolFlag <0> (true); }
void sdk::Trackable::unlock() { _.setBoolFlag <0> (false); }

void sdk::Trackable::refresh() { refresh(QDateTime::currentDateTime()); }
void sdk::Trackable::refresh(const QDateTime& dt) { _ = dt; }

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire<const Trackable> d) { return s << io(d->_, d); }
    QDataStream& operator >> (QDataStream& s, Wire<      Trackable> d) { return s >> io(d->_, d); }
}
