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

    template<typename F> Defer<F> defer(F&& f) { return Defer<F>(std::forward<F>(f)); }
    // ============================================================================================


    // ----------------------- Wire ---------------------------------------------------------------
    template <class T> class Wire {
    public:
        T& ref; bool recursive;
        Wire(T& ref, bool recursive = true) : ref(ref), recursive(recursive) {}
              T* operator->()       noexcept { return &ref; }
        const T* operator->() const noexcept { return &ref; }
    };
    template <class T> Wire       <T> io(      T& t, bool recurs = true) { return { t, recurs }; }
    // template <class T> Wire <const T> io(const T& t, bool recurs = true) { return { t, recurs }; }

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
    template <typename T> requires StreamReadableFor <T>
    QDataStream& from_stream(QDataStream& stream, bool recursive, T& d){
        if constexpr (std::is_pointer_v <T>){
            if constexpr (WireRead <std::remove_pointer_t <T>>) stream >> io(*d, recursive);
            else                                                stream >> *d;
        }
        else {
            if constexpr (WireRead <T>) stream >> io(d, recursive);
            else                        stream >> d;
        }
        return stream;
    }

    template <typename T> requires StreamWritableFor <T>
    QDataStream& to_stream(QDataStream& stream, bool recursive, const T& d){
        if constexpr (std::is_pointer_v <T>){
            if constexpr (WireWrite <std::remove_pointer_t <T>>) stream << io(*d, recursive);
            else                                                 stream << *d;
        }
        else {
            if constexpr (WireWrite <T>) stream << io(d, recursive);
            else                         stream << d;
        }
        return stream;
    }

    template <typename T> requires StreamReadableFor <T>
    QDataStream& from_stream(QDataStream& stream, T& d)
    { return from_stream <T> (stream, true, d); }

    template <typename T> requires StreamWritableFor <T>
    QDataStream& to_stream(QDataStream& stream, T& d)
    { return to_stream <T> (stream, true, d); }
    // ============================================================================================


    // ----------------------- From / To Bytes ----------------------------------------------------
    template <typename T, typename... Args> requires StreamReadableFor <T>
    T from_bytes(bool recursive, const QByteArray& data, Args&&...args){
        QDataStream stream(data);
        stream.setVersion(QDataStream::Qt_6_10);

        if constexpr (std::is_pointer_v <T>){
            using BaseT = std::remove_pointer_t <T>;
            BaseT* t = new BaseT(std::forward <Args>(args)...);
            from_stream(stream, recursive, t);
            if (stream.status() != QDataStream::Ok) { delete t; return nullptr; }
            return t;
        }
        else {
            T t(std::forward <Args> (args)...);
            from_stream(stream, recursive, t);
            return t;
        }
    }

    template <typename T> requires StreamWritableFor <T>
    QByteArray to_bytes(bool recursive, const T& d){
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_6_10);
        to_stream(stream, recursive, d);
        return data;
    }

    template <typename T, typename... Args> requires StreamReadableFor <T>
    T from_bytes(const QByteArray& data, Args&&...args)
    { return from_bytes <T, Args...> (true, data, std::forward <Args> (args)...); }

    template <typename T, typename... Args> requires StreamWritableFor <T>
    QByteArray to_bytes(const T& data) { return to_bytes <T, Args...> (true, data); }
    // ============================================================================================


    // ----------------------- List stream --------------------------------------------------------
    template <typename T, typename... Args> requires StreamReadableFor <T>
    QDataStream& list_from_stream(QDataStream& stream, bool recursive,
                                  std::vector <T>& d, Args&&...args){
        int32_t size; stream >> size;
        d.reserve(d.size() + size);
        for (int i = 0; i < size; i++){
            if constexpr (std::is_pointer_v <T>){
                using BaseT = std::remove_pointer_t <T>;
                BaseT* t = new BaseT(std::forward <Args>(args)...);
                from_stream(stream, recursive, t);
                if (stream.status() != QDataStream::Ok) delete t;
                else d.push_back(t);
            }
            else {
                T t(std::forward <Args>(args)...);
                from_stream(stream, recursive, t);
                if (stream.status() == QDataStream::Ok) d.push_back(t);
            }
        }
        return stream;
    }

    template <typename T> requires StreamWritableFor <T>
    QDataStream& list_to_stream(QDataStream& stream, bool recursive, const std::vector <T>& d){
        stream << int32_t(d.size());
        for (const auto& it : d)
            to_stream(stream, recursive, it);
        return stream;
    }

    template <typename T, typename... Args> requires StreamReadableFor <T>
    QDataStream& list_from_stream(QDataStream& stream, std::vector <T>& d, Args&&...args)
    { return list_from_stream <T, Args...> (stream, true, d, std::forward <Args> (args)...); }

    template <typename T> requires StreamWritableFor <T>
    QDataStream& list_to_stream(QDataStream& stream, const std::vector <T>& d){
        return list_to_stream(stream, true, d);
    }
    // ============================================================================================


    // ----------------------- List ---------------------------------------------------------------
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
        { return list_to_stream(s, d.recursive, d->_); }

        friend QDataStream& operator >> (QDataStream& s, Wire <List> d){
            std::vector <T> tmp;
            list_from_stream(s, d.recursive, tmp);
            if (s.status() == QDataStream::Ok)
                d->_.swap(tmp);
            return s;
        }
    };
    // ============================================================================================


    // ----------------------- list Bytes ---------------------------------------------------------
    template <typename T, typename... Args>
    std::vector <T> list_from_bytes(bool recursive, const QByteArray& data, Args&&...args){
        std::vector <T> list;
        QDataStream stream(data);
        stream.setVersion(QDataStream::Qt_6_10);
        list_from_stream <T, Args...> (stream, recursive, list, std::forward <Args>(args)...);
        return list;
    }

    template <typename T>
    QByteArray list_to_bytes(bool recursive, const std::vector <T>& d){
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_6_10);
        list_to_stream(stream, recursive, d);
        return data;
    }

    template <typename T, typename... Args>
    std::vector <T> list_from_bytes(const QByteArray& data, Args&&...args)
    { return list_from_bytes <T, Args...> (true, data, std::forward <Args> (args)...); }

    template <typename T, typename... Args>
    QByteArray list_to_bytes(const std::vector <T>& data, Args&&...args)
    { return list_to_bytes <T, Args...> (true, data, std::forward <Args> (args)...); }
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
    template<typename T>
    static inline bool nearly_equal(T a, T b, T eps) {
        if constexpr (std::is_floating_point_v<T>)
            return std::fabs(a - b) <= eps*std::max<T>(T(1), std::max(std::fabs(a), std::fabs(b)));
        else
            return a == b;
    }

    template <typename T, typename Flag> static inline std::optional <Flag>
    set_if(Trackable* ptr, T& field, const T& value, Flag flag, T eps = T{}) {
        if constexpr (std::is_floating_point_v<T>) {
            if (nearly_equal(field, value, eps)) return std::nullopt;
        } else {
            if (field == value) return std::nullopt;
        }
        field = value;
        ptr->_last_updated = QDateTime::currentDateTime();
        return flag;
    }
    // ============================================================================================
}

#endif // UTIL_FEATURES_H
