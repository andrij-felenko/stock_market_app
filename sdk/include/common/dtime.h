#ifndef SDK_COMMON_DTIME_H
#define SDK_COMMON_DTIME_H

#include <QtCore/QDateTime>
#include <QtCore/QDataStream>
#include <array>

namespace sdk {
    class DTime;
    template <class T> class Wire;
}

/*!\
 * DTime stores a UTC-aligned moment and four user-controlled bits inside five
 * bytes so Trackable/Syncer can ship the same value as metadata, persist it to
 * disk, or tag it with tiny local hints.
 *
 * Layout and masks
 * - Bytes 0–3: seconds from midnight plus the upper Julian-day bits (little
 *   endian).
 * - Byte 4: \c [7] validity flag, \c [6..3] four free bits for caller-owned
 *   markers, \c [2..0] the lower Julian-day bits.
 * - \c core_mask covers the validity and Julian fields; \c free_mask exposes
 *   the four caller bits; \c n_mask() clears a region while keeping the rest.
 *
 * Typical workflow
 * - Capture a moment in UTC: \code sdk::DTime ts(QDateTime::currentDateTimeUtc()); \endcode
 * - Use the free bits for lightweight state: \code ts.setBoolFlag<0>(true); // subscribed, cached, etc. \endcode
 * - Serialize through \c Wire with the mode that matches the trip:
 *   - \c WireMode::Meta moves the packed timestamp so Syncer can compare update
 *     moments between peers.
 *   - \c WireMode::Data carries only the free bits, letting you share user
 *     tags without rewriting the recorded time.
 *   - Combine modes for full saves or first-time sync.
 *
 * Examples
 * - Tracking freshness: store \c ts in a Trackable field, send \c Wire(ts,
 *   WireMode::Meta) when pushing updates so the server knows the last change.
 * - Sharing intent without touching time: flip \c setBoolFlag<1>(true) to mark
 *   “needs refresh” and send \c Wire(ts, WireMode::Data) so only the marker
 *   travels.
 *
 * Reliability tips
 * - Keep everything in one timezone (UTC recommended); the packing is timezone
 *   agnostic.
 * - Only adjust the free bits with \c boolflag()/\c flag(); the remaining bits
 *   reconstruct the date/time and validity state.
 * - If \c QDate or \c QTime is invalid, \c setValid(false) is written; check
 *   \c valid() before using the unpacked \c QDateTime.
 */
/*
 * DTime зберігає момент часу, вирівняний під UTC, і чотири користувацькі біти у
 * пʼяти байтах, щоб Trackable/Syncer могли передавати це значення як метадані,
 * тримати його на диску або додавати невеликі локальні мітки.
 *
 * Структура та маски
 * - Байти 0–3: секунди від півночі разом зі старшими бітами юліанського дня
 *   (little endian).
 * - Байт 4: \c [7] прапор валідності, \c [6..3] чотири вільні біти для міток
 *   користувача, \c [2..0] молодші біти юліанського дня.
 * - \c core_mask охоплює валідність і юліанське число; \c free_mask відкриває
 *   чотири користувацькі біти; \c n_mask() прибирає ділянку, не чіпаючи решту.
 *
 * Звичний сценарій
 * - Зафіксуйте момент у UTC: \code sdk::DTime ts(QDateTime::currentDateTimeUtc()); \endcode
 * - Використайте вільні біти для стану: \code ts.setBoolFlag<0>(true); // підписка, кеш тощо \endcode
 * - Серіалізуйте через \c Wire з потрібним режимом:
 *   - \c WireMode::Meta рухає упакований час, щоб Syncer міг порівнювати
 *     моменти змін між вузлами.
 *   - \c WireMode::Data переносить лише вільні біти, дозволяючи ділитися
 *     мітками без переписування зафіксованого часу.
 *   - Поєднуйте режими для повних збережень або першої синхронізації.
 *
 * Приклади
 * - Відстеження актуальності: зберігайте \c ts у полі Trackable, надсилайте
 *   \c Wire(ts, WireMode::Meta) під час оновлень, щоб сервер знав останню
 *   зміну.
 * - Передача намірів без зміни часу: встановіть \c setBoolFlag<1>(true), щоб
 *   позначити «потребує оновлення», і відправте \c Wire(ts, WireMode::Data),
 *   щоб рушили лише мітки.
 *
 * Поради для коректності
 * - Тримайте один часовий пояс (рекомендовано UTC); пакування не зберігає
 *   позначку часового поясу.
 * - Міняйте тільки вільні біти через \c boolflag()/\c flag(); решта відновлює
 *   дату, час і валідність.
 * - Якщо \c QDate або \c QTime невалідні, записується \c setValid(false);
 *   перевіряйте \c valid() перед використанням розпакованого \c QDateTime.
 */
class sdk::DTime
{
    constexpr static uint8_t  core_mask() { return 0b1000'0111; }
    constexpr static uint8_t  free_mask() { return 0b0111'1000; }
    constexpr static uint8_t nfree_mask() { return static_cast <uint8_t> (~free_mask()); }

    constexpr static uint8_t n_mask(uint8_t mask) { return static_cast <uint8_t> (~mask); }

    template <uint8_t i> requires (i <= 3)
    constexpr static uint8_t boolmask() { return 0b1000u << i; }

    template <uint8_t i> requires (i <= 3)
    constexpr static uint8_t nboolmask() { return static_cast <uint8_t> (~boolmask <i> ()); }


    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    constexpr static uint8_t mask() { return ((1u << size) - 1u) << (3 + i); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    constexpr static uint8_t nmask() { return static_cast <uint8_t> (~mask <i, size> ()); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t cut(uint8_t value) { return ((1u << size) - 1u) & value; }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t cut_v(uint8_t value) { return value & mask <i, size>(); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t read_v(uint8_t value) { return cut_v <i, size>(value) >> (3 + i); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t write_v(uint8_t value) { return cut <i, size> (value) << (3 + i); }

public:
    DTime();
    DTime(const QDate& d);
    DTime(const QDateTime& dt);

    [[nodiscard]] QDate date() const;
    bool setDate(const QDate& date);

    [[nodiscard]] QTime time() const;
    bool setTime(const QTime& time);

    operator QDate() const;
    operator QTime() const;
    operator QDateTime() const;

    [[nodiscard]] QDateTime dateTime() const;
    bool setDateTime(const QDateTime& dt);

    [[nodiscard]] bool valid() const;

    template <uint8_t i> requires (i <= 3)
    bool boolflag() const { return (boolmask <i>() & get8()) != 0; }

    template <uint8_t i> requires (i <= 3)
    void setBoolFlag(bool value) {
        if (value) set8(get8() & nboolmask <i>());
        else       set8(get8() |  boolmask <i>());
    }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    uint8_t flag() const { return read_v <i, size> (get8()); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    void setFlag(uint8_t value) { set8((get8() & nmask <i, size>()) | write_v <i, size>(value)); }

private:
    void set32(uint32_t v) { std::memcpy(_.data(), &v, 4); }
    uint32_t get32() const {
        uint32_t v;
        std::memcpy(&v, _.data(), 4);
        return v;
    }

    void set8(uint8_t v) { _[4] = static_cast <std::byte> (v); }
    uint8_t get8() const { return static_cast <uint8_t> (_[4]); }

    std::array <std::byte, 5> _ = {};

    void setValid(bool valid);

    friend QDataStream& operator << (QDataStream& s, Wire <const DTime> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      DTime> d);

    friend QDataStream& operator << (QDataStream& s, const DTime& d);
    friend QDataStream& operator >> (QDataStream& s,       DTime& d);
};

#endif // SDK_COMMON_DTIME_H
