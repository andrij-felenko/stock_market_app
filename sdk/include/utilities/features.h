#ifndef UTIL_FEATURES_H
#define UTIL_FEATURES_H

#include <utility>

#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

namespace util {
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


    // ----------------------- QDataStream and std::vector ----------------------------------------
    template <typename T>
    concept DataStreamReadable = requires(QDataStream& s, T& t)
    { { s >> t } -> std::same_as <QDataStream&>; };

    template <typename T, typename... Args>
    requires (std::is_pointer_v <T> ? DataStreamReadable <std::remove_pointer_t <T>>
                                    : DataStreamReadable <T>)
    QDataStream& list_from_stream(QDataStream& s, std::vector <T>& d, Args&&...args){
        int32_t size; s >> size;
        d.reserve(d.size() + size);
        for (int i = 0; i < size; i++){
            if constexpr (std::is_pointer_v <T>){
                using BaseT = std::remove_pointer_t <T>;
                BaseT* t = new BaseT(std::forward <Args>(args)...);
                s >> *t;
                d.push_back(t);
            }
            else {
                T t(std::forward <Args>(args)...);
                s >> t;
                d.push_back(t);
            }
        }
        return s;
    }

    template <typename T>
    concept DataStreamWritable = requires(QDataStream& s, const T& t)
    { { s << t } -> std::same_as <QDataStream&>; };

    template <typename T>
    requires (std::is_pointer_v <T> ? DataStreamWritable <std::remove_pointer_t <T>>
                                    : DataStreamWritable <T>)
    QDataStream& list_to_stream(QDataStream& s, const std::vector <T>& d){
        s << int32_t(d.size());
        for (const auto& it : d)
            if constexpr (std::is_pointer_v <T>) s << *it; else s << it;
        return s;
    }
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
}

#endif // UTIL_FEATURES_H
