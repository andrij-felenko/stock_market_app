#ifndef UTIL_FEATURES_H
#define UTIL_FEATURES_H

#include <QIODevice>
#include <utility>

#include <QtCore/QThread>
#include <QtCore/QCoreApplication>
#include "trackable.h"
#include <type_traits>

namespace sdk {
    // ----------------------- Defer --------------------------------------------------------------
    /*! \brief A Go-like "defer" that keeps C++ scopes honest.

        Go's `defer` keyword inspired this helper: you place the cleanup right
        next to the resource and let the compiler guarantee it runs on every
        exit path. `Defer` stores a callable, executes it in the destructor, and
        can be silenced with `dismiss()` when you intentionally keep the
        resource.

        Everyday example that mirrors the Go pattern but stays in C++ RAII
        land:
        \code{.cpp}
        bool busy = true;
        auto guard = sdk::Defer([&]() { busy = false; });
        // do risky work; the flag will drop even if an exception appears
        if (all_good) guard.dismiss(); // keep the flag when you really mean it
        \endcode
    */
    template <typename F> class Defer {
        F _func;
        bool _active;
    public:
        explicit Defer(F&& f) : _func(std::forward<F>(f)), _active(true) {}
        ~Defer() { if (_active) _func(); }

        Defer(const Defer&) = delete;
        Defer& operator = (const Defer&) = delete;

        // Дозволяємо переміщення
        Defer(Defer&& other) noexcept : _func(std::move(other._func)), _active(other._active) {
            other._active = false;
        }

        Defer& operator=(Defer&& other) noexcept {
            if (this != &other) {
                _func = std::move(other._func);
                _active = other._active;
                other._active = false;
            }
            return *this;
        }

        void dismiss() noexcept { _active = false; }
    };

    /*! \brief Helper that infers the template arguments for \ref Defer.

        `sdk::defer(...)` is simply a friendlier face for \ref Defer: the same
        Go-inspired idea without writing the template type. Use it whenever a
        small, scoped promise keeps the code more readable than a manual
        try/finally.

        Common pattern for short-lived resources:
        \code{.cpp}
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) return;
        auto close = sdk::defer([&]() { file.close(); });
        // nothing else to remember—the handle will close on every exit branch
        \endcode
    */
    template<typename F> Defer<F> defer(F&& f) { return Defer<F>(std::forward<F>(f)); }
    // ============================================================================================


    // ----------------------- Wire ---------------------------------------------------------------
    /*! \brief Flags that describe what a serializer should touch.

        The bits separate core data from relationship info and update metadata
        so Syncer and Trackable can ship exactly what is needed. Кожен прапор
        має англійське пояснення та коротку україномовну підказку:

        - `WireMode::Data` — only the inner state of the object itself (цільові
          поля моделі без службових обгорток). Використовуйте для швидких
          оновлень між вузлами.
        - `WireMode::Meta` — freshness markers such as timestamps or sync
          hashes that describe *коли* файл/об'єкт змінювався останнього разу.
          Це передається на сервер, щоб звірити актуальність.
        - `WireMode::AllData` — combine the object's own data with the
          subscription/children block (див. `subs()` нижче), але без метаданих
          часу. Дає повну логічну картину в межах самого об'єкта.
        - `WireMode::All` — повний пакет: і дата, і мета, і підписки. Потрібен
          коли відправляємо на диск або робимо першу синхронізацію.
        - `WireMode::Storage` — alias for `All`; підкреслює сценарій збереження
          у файлі чи кеші.
        - `WireMode::Update` — alias for `Data`; мінімальний дельта-пакет для
          живих оновлень.
        - `WireMode::UpdateAll` — alias for `AllData`; коли окрім полів треба
          передати і вкладені Trackable-об'єкти, щоб підписки не загубилися.
        - `WireMode::SyncTime` — alias for `Meta`; потрібен коли цікавить лише
          інформація «коли змінено», без корисних даних.
    */
    enum class WireMode : uint8_t {
        All       = 0b1111'1111,
        Meta      = 0b0000'0100,
        Data      = 0b0000'0001,
        AllData   = 0b0000'0011,
        Storage   = All,
        Update    = Data,
        UpdateAll = AllData,
        SyncTime  = Meta,
    };

    /*! \brief Wrapper that says "serialize this object, but only these parts".

        `Wire` decorates an existing reference and carries a \ref WireMode that
        explains what should be written or read: metadata, data fields, or
        both. Operators behave like a pointer so regular member access keeps
        working, and the mode is available to any streaming operators you
        implement.

        How the bits map to Trackable/Syncer flow:
        - `data()` — внутрішній стан конкретного об'єкта. Це власні поля моделі
          без службових міток.
        - `subs()` — вкладені Trackable-об'єкти або пов'язані структури, які
          зберігаються всередині і теж треба передати (наприклад, дочірні списки
          інструментів). Вони йдуть разом з даними, коли потрібно повноцінне
          дерево.
        - `meta()` — мітки часу/версії, які Syncer використовує для звірки
          актуальності. Це те, що серверу треба знати, щоб вирішити, чи є зміни
          у файлі.

        Why use it?
        - The same DTOs should travel over the wire *and* rest on disk without
          two separate code paths. `Wire` lets you reuse one serializer and
          switch the mode to match the context.
        - Trackable objects rely on Syncer to ship live updates. Sending only
          `WireMode::Update` keeps those packets small; `WireMode::UpdateAll`
          додає вкладені частини через `subs()` коли треба дерево.
        - Metadata (`WireMode::SyncTime`) moves separately so peers can compare
          timestamps before asking for the heavier payload.
        - Коли сумніваєтеся, підберіть режим за сценарієм: `Update`/`UpdateAll`
          для живих оновлень, `Storage` для файлів, `SyncTime` для легких
          службових обмінів.

        Example: keep storage and live-sync in step for a Trackable-derived
        portfolio snapshot and its nested instruments:
        \code{.cpp}
        Portfolio dto;
        // Persist everything so we can restore quickly after a restart
        QFile store(path); store.open(QIODevice::WriteOnly);
        QDataStream toDisk(&store);
        toDisk << sdk::io(dto, sdk::WireMode::Storage); // data + subs + meta

        // Later, push only data changes through Syncer to a peer
        QByteArray packet;
        QDataStream live(&packet, QIODevice::WriteOnly);
        live << sdk::io(dto, sdk::WireMode::Update); // тільки поля самого об'єкта
        transport.send(packet);

        // If the peer must refresh nested Trackable items too
        QByteArray fullPacket;
        QDataStream liveAll(&fullPacket, QIODevice::WriteOnly);
        liveAll << sdk::io(dto, sdk::WireMode::UpdateAll); // додає вміст subs()
        transport.send(fullPacket);
        \endcode
    */
    template <class T> class Wire {
    public:
        bool meta() const { return (static_cast <uint8_t> (_mode) & 0b100) != 0; }
        bool subs() const { return (static_cast <uint8_t> (_mode) & 0b010) != 0; }
        bool data() const { return (static_cast <uint8_t> (_mode) & 0b001) != 0; }

        void setMode(WireMode mode) { _mode = mode; }

        T& ref;

        template <typename T2>
        Wire(T& ref, const Wire <T2>& from)         : ref(ref), _mode(from.mode()) { /* */ }
        Wire(T& ref, WireMode mode = WireMode::All) : ref(ref), _mode(mode)        { /* */ }

              T* operator->()       noexcept { return &ref; }
        const T* operator->() const noexcept { return &ref; }

        operator Wire <const std::remove_reference_t <T>> () const {
            using U = std::remove_reference_t <T>;
            Wire <const U> w(static_cast <const U&> (ref));
            w.setMode(_mode);
            return w;
        }

        WireMode mode() const { return _mode; }

    private:
        WireMode _mode;
    };

    // io section --------------------------------------------------------
    /*! \brief Convenience wrapper for constructing \ref Wire with full mode. */
    template <class T> Wire <T> io(T& t) { return Wire <T> (t); }

    /*! \brief Copy wire mode from another wrapper while changing the reference. */
    template <class T, class T2> Wire <T> io(T& t, const Wire <T2>& from)
    { return Wire <T> (t, from); }

    /*! \brief Construct a wrapper with an explicit serialization mode. */
    template <class T> Wire <T> io(T& t, WireMode mode)
    { return Wire <T> (t, mode); }
    // ===================================================================

    template<class X>
    concept WireLike = requires (std::remove_cvref_t <X>* p) { [] <class T> (Wire <T>*){}(p); };

    template <class X>
    using WireLess = std::conditional_t <WireLike <X>,
        typename std::remove_cvref_t <X>::underlying,
        std::remove_cvref_t <X>
    >;

    template<class U> concept WireWrite = requires (QDataStream& s, const U u)
    { { s << io(u, true) } -> std::same_as <QDataStream&>; };

    template<class U> concept WireRead = requires (QDataStream& s, U u)
    { { s >> io(u, true) } -> std::same_as <QDataStream&>; };
    // ============================================================================================



    // --------------------------------------------------------------------------------------------
    // ----------------------- QDataStream and std::vector ----------------------------------------
    // --------------------------------------------------------------------------------------------
    template <typename T> concept StreamReadable = requires(QDataStream& s, T& t)
    { { s >> t } -> std::same_as <QDataStream&>; };

    template <typename T> concept StreamWritable = requires(QDataStream& s, const T& t)
    { { s << t } -> std::same_as <QDataStream&>; };

    template <class T> concept StreamOrWireReadable = StreamReadable <T> || WireRead <T>;
    template <class T> concept StreamOrWireWritable = StreamWritable <T> || WireWrite<T>;

    template <typename T> concept StreamReadableFor =
            (std::is_pointer_v <T> ? StreamOrWireReadable <std::remove_pointer_t <T>>
                                   : StreamOrWireReadable <T>);

    template <typename T> concept StreamWritableFor =
            (std::is_pointer_v <T> ? StreamOrWireWritable <std::remove_pointer_t <T>>
                                   : StreamOrWireWritable <T>);


    // ----------------------- From / To Stream ---------------------------------------------------
    /*! \brief Read an object (or pointer) from a QDataStream and respect its mode.

        The helper picks the right overload for `T`: plain streaming when no
        `Wire` support is present, or `sdk::io(..., mode)` when it is. Pointer
        types are dereferenced automatically so callers do not need a separate
        branch for `T*`.
    */
    template <typename T> requires StreamReadableFor <T>
    QDataStream& from_stream(QDataStream& stream, WireMode mode, T& d){
        if constexpr (std::is_pointer_v <T>){
            if constexpr (WireRead <std::remove_pointer_t <T>>) stream >> io(*d, mode);
            else                                                stream >> *d;
        }
        else {
            if constexpr (WireRead <T>) stream >> io(d, mode);
            else                        stream >> d;
        }
        return stream;
    }

    /*! \brief Write an object (or pointer) to a QDataStream using the selected mode.

        Mirrors \ref from_stream: if `T` understands `Wire`, we call `sdk::io`
        with the provided mode; otherwise we stream the value directly. Pointer
        arguments are dereferenced for you.
    */
    template <typename T> requires StreamWritableFor <T>
    QDataStream& to_stream(QDataStream& stream, WireMode mode, const T& d){
        if constexpr (std::is_pointer_v <T>){
            if constexpr (WireWrite <std::remove_pointer_t <T>>) stream << io(*d, mode);
            else                                                 stream << *d;
        }
        else {
            if constexpr (WireWrite <T>) stream << io(d, mode);
            else                         stream << d;
        }
        return stream;
    }

    template <typename T> requires StreamReadableFor <T>
    QDataStream& from_stream(QDataStream& stream, T& d)
    { return from_stream <T> (stream, WireMode::All, d); }

    template <typename T> requires StreamWritableFor <T>
    QDataStream& to_stream(QDataStream& stream, T& d)
    { return to_stream <T> (stream, WireMode::All, d); }
    // ============================================================================================


    // ----------------------- From / To Bytes ----------------------------------------------------
    /*! \brief Deserialize any streamable type from a QByteArray.

        Builds a temporary `QDataStream`, applies the requested `WireMode`, and
        returns the value. Pointer reads allocate the object for you; ownership
        stays with the caller.
    */
    template <typename T, typename... Args> requires StreamReadableFor <T>
    T from_bytes(WireMode mode, const QByteArray& data, Args&&...args){
        QDataStream stream(data);
        stream.setVersion(QDataStream::Qt_6_10);

        if constexpr (std::is_pointer_v <T>){
            using BaseT = std::remove_pointer_t <T>;
            BaseT* t = new BaseT(std::forward <Args>(args)...);
            from_stream(stream, mode, t);
            if (stream.status() != QDataStream::Ok) { delete t; return nullptr; }
            return t;
        }
        else {
            T t(std::forward <Args> (args)...);
            from_stream(stream, mode, t);
            return t;
        }
    }

    /*! \brief Serialize any streamable type into a QByteArray.

        Keeps the Qt stream version consistent and avoids duplicating the
        `QDataStream` setup in network code or tests.
    */
    template <typename T> requires StreamWritableFor <T>
    QByteArray to_bytes(WireMode mode, const T& d){
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_6_10);
        to_stream(stream, mode, d);
        return data;
    }

    template <typename T, typename... Args> requires StreamReadableFor <T>
    T from_bytes(const QByteArray& data, Args&&...args)
    { return from_bytes <T, Args...> (WireMode::All, data, std::forward <Args> (args)...); }

    template <typename T, typename... Args> requires StreamWritableFor <T>
    QByteArray to_bytes(const T& data) { return to_bytes <T, Args...> (WireMode::All, data); }
    // ============================================================================================


    // ----------------------- List stream --------------------------------------------------------
    /*! \brief Deserialize a vector from a stream while honoring `WireMode`.

        Each item uses \ref from_stream so both plain types and `Wire`-aware
        ones work. Pointer elements join the vector only if the read succeeds
        to prevent leaking half-built values.
    */
    template <typename T, typename... Args> requires StreamReadableFor <T>
    QDataStream& list_from_stream(QDataStream& stream, WireMode mode,
                                  std::vector <T>& d, Args&&...args){
        int32_t size; stream >> size;
        d.reserve(d.size() + size);
        for (int i = 0; i < size; i++){
            if constexpr (std::is_pointer_v <T>){
                using BaseT = std::remove_pointer_t <T>;
                BaseT* t = new BaseT(std::forward <Args>(args)...);
                from_stream(stream, mode, t);
                if (stream.status() != QDataStream::Ok) delete t;
                else d.push_back(t);
            }
            else {
                T t(std::forward <Args>(args)...);
                from_stream(stream, mode, t);
                if (stream.status() == QDataStream::Ok) d.push_back(t);
            }
        }
        return stream;
    }

    /*! \brief Serialize a vector to a stream using `WireMode` for each element. */
    template <typename T> requires StreamWritableFor <T>
    QDataStream& list_to_stream(QDataStream& stream, WireMode mode, const std::vector <T>& d){
        stream << int32_t(d.size());
        for (const auto& it : d)
            to_stream(stream, mode, it);
        return stream;
    }

    template <typename T, typename... Args> requires StreamReadableFor <T>
    QDataStream& list_from_stream(QDataStream& stream, std::vector <T>& d, Args&&...args)
    { return list_from_stream <T, Args...> (stream, WireMode::All,
                                            d, std::forward <Args> (args)...); }

    template <typename T> requires StreamWritableFor <T>
    QDataStream& list_to_stream(QDataStream& stream, const std::vector <T>& d){
        return list_to_stream(stream, WireMode::All, d);
    }
    // ============================================================================================


    // ----------------------- List ---------------------------------------------------------------
    /*! \brief Thin std::vector wrapper with optional friend-only mutation.

        With `Owner = void`, this behaves like a small vector wrapper and
        exposes the underlying container through pointer-like operators. If you
        pass an `Owner` type, mutation helpers become private to that owner so
        outside code can only read the data.

        Example: expose prices as read-only to consumers while still allowing
        the owning class to append:
        \code{.cpp}
        class Prices {
            using Storage = sdk::List<double, Prices>;
        public:
            const Storage& values() const { return _values; }
            void add(double v) { _values->push_back(v); }
        private:
            Storage _values;
        };
        \endcode
    */
    template <class T, typename Owner = void>
    class List {
        std::vector <T> _;

        struct _NoOwner {};
        using FriendType = std::conditional_t <std::is_void_v <Owner>, _NoOwner, Owner>;
        friend FriendType;

        std::vector <T>* operator->() noexcept requires (not std::is_void_v <Owner>) { return &_; }
        const std::vector <T>* operator->() const noexcept requires (not std::is_void_v <Owner>)
        { return &_; }

    public:
        std::vector <T>* operator->() noexcept requires (std::is_void_v <Owner>) { return &_; }
        const std::vector <T>* operator->() const noexcept requires (std::is_void_v <Owner>)
        { return &_; }

        std::size_t size() const noexcept { return _.size(); }

        std::span       <T> range()       noexcept { return _; }
        std::span <const T> range() const noexcept { return _; }

        typename std::vector <T>::iterator begin()       noexcept { return _.begin(); }
        typename std::vector <T>::iterator end()         noexcept { return _.end(); }

        typename std::vector <T>::const_iterator begin() const noexcept { return _.begin(); }
        typename std::vector <T>::const_iterator end()   const noexcept { return _.end(); }

        friend QDataStream& operator << (QDataStream& s, Wire <const List> d)
        { return list_to_stream(s, d.mode(), d->_); }

        friend QDataStream& operator >> (QDataStream& s, Wire <List> d){
            std::vector <T> tmp;
            list_from_stream(s, d.mode(), tmp);
            if (s.status() == QDataStream::Ok)
                d->_.swap(tmp);
            return s;
        }
    };
    // ============================================================================================


    // ----------------------- list Bytes ---------------------------------------------------------
    /*! \brief Deserialize a vector from raw bytes with optional element ctor args. */
    template <typename T, typename... Args>
    std::vector <T> list_from_bytes(WireMode mode, const QByteArray& data, Args&&...args){
        std::vector <T> list;
        QDataStream stream(data);
        stream.setVersion(QDataStream::Qt_6_10);
        list_from_stream <T, Args...> (stream, mode, list, std::forward <Args>(args)...);
        return list;
    }

    /*! \brief Serialize a vector into raw bytes for transport or storage. */
    template <typename T>
    QByteArray list_to_bytes(WireMode mode, const std::vector <T>& d){
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_6_10);
        list_to_stream(stream, mode, d);
        return data;
    }

    template <typename T, typename... Args>
    std::vector <T> list_from_bytes(const QByteArray& data, Args&&...args)
    { return list_from_bytes <T, Args...> (WireMode::All, std::forward <Args> (args)...); }

    template <typename T, typename... Args>
    QByteArray list_to_bytes(const std::vector <T>& data, Args&&...args)
    { return list_to_bytes <T, Args...> (WireMode::All, data, std::forward <Args> (args)...); }
    // ============================================================================================
    // ============================================================================================



    // ------------------------ Thread do - work - later ------------------------------------------
    static void to_thread(std::function <void ()> work,
                          std::function <void ()> after = nullptr)
    {
        QThread* thread = QThread::create([=]() {
            work();
            if (after)
                QMetaObject::invokeMethod(qApp, after, Qt::QueuedConnection);
        });
        QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        thread->start();
    }
    // ============================================================================================



    // ----------------------- Set if functionality -----------------------------------------------
    /*! \brief Compare values with tolerance when dealing with floating points.

        Non-floating types fall back to regular equality. Floating values use a
        relative epsilon so we skip updates triggered only by rounding noise.
    */
    template<typename T>
    static inline bool nearly_equal(T a, T b, T eps) {
        if constexpr (std::is_floating_point_v<T>)
            return std::fabs(a - b) <= eps * std::max <T> (T(1),
                                                           std::max(std::fabs(a), std::fabs(b)));
        else
            return a == b;
    }

    /*! \brief Update a field and refresh the owner only when something truly changed.

        Commonly used inside `Trackable` DTOs to avoid spamming refresh events
        when the new value is effectively the same as the old one.
        \code{.cpp}
        std::optional<MyFlags> changed = sdk::set_if(this, price, newPrice,
                                                     MyFlags::PriceUpdated, 0.001);
        if (changed) notify(*changed);
        \endcode
        \param ptr   Pointer to the owning Trackable for timestamp refresh.
        \param field Target field to compare and potentially overwrite.
        \param value Incoming value.
        \param flag  Change flag returned when the update is applied.
        \param eps   Tolerance used for floating point comparisons.
        \return std::nullopt when unchanged, otherwise the provided flag.
    */
    template <typename T, typename T2, typename Flag> static inline std::optional <Flag>
    set_if(Trackable* ptr, T& field, const T2& value, Flag flag, T eps) {
        if constexpr (std::is_floating_point_v <T>) {
            if (nearly_equal(field, value, eps))
                return std::nullopt;
        }
        else {
            if (field == value)
                return std::nullopt;
        }
        field = value;
        ptr->refresh();
        return flag;
    }
    // ============================================================================================


    // ----------------------- Mask pack ----------------------------------------------------------
    template <uint8_t bits_count>
    class MaskPack {
        //
    };
    // ============================================================================================


    // ----------------------- Singleton inline ---------------------------------------------------
    template <typename T>
    concept SingletonLike = requires { { T::reference() } -> std::same_as<T&>; } ||
                            requires { { T::instance()  } -> std::same_as<T*>; };

    template <SingletonLike T>
    struct Singleton {
        constexpr T* operator -> () const noexcept { return ptr(); }
        constexpr T* ptr() const noexcept {
            if constexpr (requires { T::instance(); })
                return T::instance();
            else
                return &T::reference();
        }

        constexpr T& operator * () const noexcept {
            if constexpr (requires { T::instance(); })
                return *T::instance();
            else
                return T::reference();
        }
    };
    // ============================================================================================
}

#endif // UTIL_FEATURES_H
