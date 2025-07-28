#ifndef UTIL_DEFINE_H
#define UTIL_DEFINE_H

/**
 * \brief Summary of macro definitions in this file.
 *
 * This header defines utility macros for enum-to-string conversion,
 * serialization, singleton patterns, Qt integration, and debug helpers.
 *
 * - \c N_NS_ENUM             -- Declares Q_FLAGS and Q_ENUM_NS for scoped enums with flags.
 * - \c NFILELINE             -- Logs current file and line with qDebug.
 * - \c NCRITONLY             -- Logs current file and line as critical error.
 * - \c NCRITVOID_B           -- Logs critical and performs bare `return`.
 * - \c NCRITVOID             -- Logs critical and returns from void function.
 * - \c NCRIT_STR             -- Logs critical error with custom message.
 * - \c NCRIT                 -- Logs critical error with empty message.
 * - \c NCRITEMPTYSTR         -- Logs critical and returns empty string from QString function.
 * - \c N_SINGLETON           -- Declares singleton pattern using QSharedPointer.
 * - \c NDECLARE_ENUM_FUNCTION -- Declares conversion functions for enum: toString, fromString, etc.
 * - \c NDECLARE_FLAG_FUNCTION -- Declares conversion functions for QFlags enum types.
 * - \c NDEFINITION_FLAG_FUNCTION -- Defines conversion logic for flag enums.
 * - \c ENUM_AND_NUMBER       -- Casts number & value mask to an enum value.
 * - \c NDEFINITION_ENUM_FUNCTION -- Defines conversion logic for simple enums.
 * - \c N_INSERTVALUE         -- Maps enum value to string in a QMap initializer.
 * - \c IO                    -- Declares friend QDataStream operators for a class.
 * - \c IO_LIST               -- Declares friend QDataStream operators for QList<class>.
 * - \c IO_PTR_LIST           -- Declares friend QDataStream operators for QList<QSharedPointer<T>>.
 * - \c DEBUG                 -- Declares QDebug output operator for a class.
 */


#include <QtCore/QDateTime>
#include <QtCore/QIODevice>
#include <QtCore/QSharedPointer>

typedef QList <QPoint>      QPointList;
typedef QList <QPointF>     QPointFList;
typedef QList <QPointList>  QPointMatrix;
typedef std::optional <QPointMatrix> QPointMatrixOpt;
typedef QList <const QPointList*> QPointMatrixRef;

typedef QSharedPointer <QByteArray> BData;
typedef QDataStream QDS;

#define N_NS_ENUM(name) \
    Q_DECLARE_FLAGS(name##s, name) \
    Q_DECLARE_OPERATORS_FOR_FLAGS(name##s) \
    Q_FLAG_NS(name##s) \
    Q_ENUM_NS(name)

// --------------------------------------------------------------------------------------
/**
 * \brief Macros for simplified debugging and critical logging with file and
 * line info.
 *
 * These macros facilitate the output of debugging and critical error messages,
 * incorporating the source file name and line number for enhanced log
 * traceability.
 *
 * `NFILELINE`:
 * Logs the current file and line number for debugging purposes.
 * Example output: "main.cpp:10"
 *
 * `NCRITONLY`:
 * Outputs a critical log message with the file and line number, without extra text.
 * Example output: "Critical: main.cpp:12"
 *
 * `NCRITVOID_B`:
 * Critical log with file and line, then returns from the function. No semicolon
 * allows customization.
 * Example use leads to output: "Critical: main.cpp:14" then returns.
 *
 * `NCRITVOID`:
 * Outputs critical log and returns from void functions. Auto includes semicolon.
 * Example use leads to output: "Critical: main.cpp:16" then returns.
 *
 * `NCRIT_STR(Default)`:
 * Critical log with file, line, and a default message.
 * Example output: "Critical: main.cpp:18 Default message"
 *
 * `NCRIT`:
 * Calls `NCRIT_STR` with an empty string for a message.
 * Example output: "Critical: main.cpp:20"
 *
 * `NCRITEMPTYSTR`:
 * Critical log and returns an empty string. For string-returning functions.
 * Example use leads to output: "Critical: main.cpp:22" then returns "".
 *
 * Usage example:
 * \code{.cpp}
 * void myFunction() {
 *     NFILELINE  // Debug log with file and line
 *     if (errorCondition) {
 *         NCRITVOID  // Log critical and return from void function
 *     }
 * }
 * QString getValue() {
 *     if (errorCondition) {
 *         NCRITEMPTYSTR;  // Log critical and return empty string
 *     }
 *     return "value";
 * }
 * \endcode
 *
 * \note Utilizes qDebug() and qCritical() from Qt. Include <QDebug> as needed.
 */
#define NFILELINE qDebug() << __FILE__ << ":" << __LINE__;
#define NCRITONLY qCritical() << __FILE__ << ":" << __LINE__;
#define NCRITVOID_B NCRITONLY return
#define NCRITVOID NCRITVOID_B;
#define NCRIT_STR(Default) qCritical() << __FILE__ << ":" << __LINE__ << Default;
#define NCRIT NCRIT_STR("")
#define NCRITEMPTYSTR NCRITVOID_B "";
// ======================================================================================


// --------------------------------------------------------------------------------------
/**
 * \brief Macro for defining a Qt-supported singleton class.
 *
 * `N_SINGLETON(CLASS)` enables the singleton pattern for a CLASS using
 * QSharedPointer for thread-safe access and singleton instance management.
 * Instances can optionally have a QObject parent for Qt's memory management.
 *
 * Usage:
 * Use within a class to enable singleton access. Generates two static methods:
 *
 * - `static QSharedPointer<CLASS> get()`: Returns a shared pointer to the
 *   singleton instance. Creates with nullptr parent if it doesn't exist.
 *
 * - `static QSharedPointer<CLASS> get(QObject* parent)`: Similar to get(),
 *   but allows specifying a parent for Qt memory management.
 *
 * Copy constructor and assignment operator are disabled to prevent copying.
 *
 * Example:
 * \code{.cpp}
 * class MyClass {
 *     N_SINGLETON(MyClass)
 * public:
 *     void myFunction();
 * };
 * \endcode
 *
 * To access the singleton:
 * \code{.cpp}
 * auto instance = MyClass::get();
 * instance->myFunction();
 * \endcode
 *
 * \note Requires Qt. Designed for QObject-derived classes.
 *
 * \param CLASS The class name for singleton application.
 */
#define N_SINGLETON(CLASS) \
public: \
    static QSharedPointer <CLASS> get() { \
        static QSharedPointer <CLASS> ptr; \
        if (ptr.isNull()) \
        ptr = QSharedPointer <CLASS>::create(nullptr); \
        return ptr; \
} \
    static QSharedPointer <CLASS> get(QObject* parent) { \
        static QSharedPointer <CLASS> ptr; \
        if (ptr.isNull()) \
        ptr = QSharedPointer <CLASS>::create(parent); \
        return ptr; \
} \
    private: \
    Q_DISABLE_COPY(CLASS) \
    friend class QSharedPointer <CLASS>;
// ======================================================================================


// --------------------------------------------------------------------------------------
/*!
 * \brief Macro: NDECLARE_ENUM_FUNCTION(Type, Default)
 *
 * \par Description
 * This macro generates a set of functions and declarations for an enumeration Type.
 *
 * \par Parameters
 * - Type: The enumeration type.
 * - Default: The default enumeration value.
 *
 * \par Generated Functions/Declarations
 * - extern const QMap<Type, QByteArray> Type##Map;
 * - extern const Type Type##_Default;
 * - extern uint16_t Type##_0;
 * - const QByteArray Type##_str();
 * - uint16_t toNumber(const Type & e);
 * - QByteArray toString(const Type & e);
 * - Type find##Type (const QByteArray& str);
 * - Type find##Type (const uint16_t& number);
 * - QByteArray toByte(const Type & e);
 * - Type fromByte##Type (const QByteArray& data);
 * - QDataStream& operator << (QDataStream& s, const Type & e);
 * - QDataStream& operator >> (QDataStream& s,       Type & e);
 * - QDebug operator << (QDebug d, const Type & e);
 * - Q_ENUM_NS(Type)
 */
#define NDECLARE_ENUM_FUNCTION(Type, Default) \
    extern const QMap <Type, QByteArray> Type##Map; \
    extern const Type Type##_Default; \
    extern uint16_t Type##_0; \
    \
    const QByteArray Type##_str(); \
    \
    uint16_t Type##Shift(const Type & e); \
    Type     Type##Shift(uint16_t shift); \
    \
    uint16_t toNumber(const Type & e); \
    QByteArray toString(const Type & e); \
    \
    Type find##Type (const QByteArray& str); \
    Type find##Type (const uint16_t& number); \
    \
    QByteArray toByte       (const Type & e); \
    Type     fromByte##Type (const QByteArray& data); \
    \
    QDataStream& operator << (QDataStream& s, const Type & e); \
    QDataStream& operator >> (QDataStream& s,       Type & e); \
    \
    QDebug operator << (QDebug d, const Type & e); \
    Q_ENUM_NS(Type)
// ======================================================================================


// --------------------------------------------------------------------------------------
/*!
 * \brief Macro: NDECLARE_FLAG_FUNCTION(Type, Default, FlagName)
 *
 * \par Description
 * This macro generates a set of functions and declarations for a flag type.
 *
 * \par Parameters
 * - Type: The enumeration type for the flag.
 * - Default: The default enumeration value.
 * - FlagName: The name of the flag.
 *
 * \par Generated Functions/Declarations
 * - Q_DECLARE_FLAGS(FlagName, Type)
 * - NDECLARE_ENUM_FUNCTION(Type, Default)
 * - uint16_t toNumber(const FlagName & e);
 * - QList<Type> toTypeEnumList(const FlagName & e);
 * - QByteArray toString(const FlagName & e);
 * - FlagName findFlagName(const QByteArray& str);
 * - QByteArray toByte(const FlagName & e);
 * - FlagName fromByteFlagName(const QByteArray& data);
 * - QDataStream& operator << (QDataStream& s, const FlagName & e);
 * - QDataStream& operator >> (QDataStream& s, FlagName & e);
 * - QDebug operator << (QDebug& d, const FlagName & e);
 * - Q_FLAG_NS(FlagName)
 */
#define NDECLARE_FLAG_FUNCTION(Type, Default, FlagName) \
Q_DECLARE_FLAGS(FlagName, Type) \
    NDECLARE_ENUM_FUNCTION(Type, Default) \
    \
    uint16_t toNumber(const FlagName & e); \
    \
    QList <Type> to##Type##EnumList(const FlagName & e); \
    \
    QByteArray toString(const FlagName & e); \
    FlagName find##FlagName (const QByteArray& str); \
    \
    QByteArray toByte           (const FlagName & e); \
    FlagName fromByte##FlagName (const QByteArray& data); \
    \
    QDataStream& operator << (QDataStream& s, const FlagName & e); \
    QDataStream& operator >> (QDataStream& s,       FlagName & e); \
    \
    QDebug operator << (QDebug& d, const FlagName & e); \
    Q_FLAG_NS(FlagName)

/*!
 * \macro NDEFINITION_FLAG_FUNCTION(Type, Default, FlagName, MinValue, MaxValue)
 * \brief Definition of Flag Functions.
 *
 * This macro generates a comprehensive set of functions and declarations for a
 * flag enumeration Type. It simplifies common operations on flag enumerations,
 * including conversions, string representations, and more.
 *
 * \param Type The enumeration type.
 * \param Default The default enumeration value.
 * \param FlagName The flag enumeration type.
 * \param MinValue The minimum possible value for the flag enumeration.
 * \param MaxValue The maximum possible value for the flag enumeration.
 *
 * The macro generates the following functions and declarations:
 *
 * - `const Type Type##_Default`: The default value for the enumeration.
 * - `uint16_t Type##_0`: The minimum value possible for the enumeration.
 * - `const QByteArray Type##_str()`: Returns a string representation of the
 *   enumeration type.
 * - `uint16_t toNumber(const Type & e)`: Converts an enumeration value to its
 *   corresponding number.
 * - `QByteArray toString(const Type & e)`: Converts an enumeration value to its
 *   string representation.
 * - `Type find##Type(const QByteArray& str)`: Finds an enumeration value from its
 *   string representation.
 * - `Type find##Type(const uint16_t& number)`: Finds an enumeration value from its
 *   numerical representation.
 * - `QByteArray toByte(const Type & e)`: Converts an enumeration value to a
 *   QByteArray for serialization.
 * - `Type fromByte##Type(const QByteArray& data)`: Converts a QByteArray to an
 *   enumeration value for deserialization.
 * - `QDataStream& operator<<(QDataStream& s, const Type & e)`: Writes an
 *   enumeration value to a QDataStream.
 * - `QDataStream& operator>>(QDataStream& s, Type & e)`: Reads an enumeration value
 *   from a QDataStream.
 * - `QDebug operator<<(QDebug d, const Type & e)`: Provides a debug-friendly
 *   representation of the enumeration value.
 *
 * Additionally, for flag enumerations, the macro generates:
 *
 * - `uint16_t toNumber(const FlagName & e)`: Converts a flag enumeration value to
 *   its corresponding number.
 * - `QList<Type> to##Type##EnumList(const FlagName & e)`: Converts a flag
 *   enumeration value to a list of individual enumeration values.
 * - `QByteArray toString(const FlagName & e)`: Converts a flag enumeration value to
 *   its string representation, combining multiple individual values with '|'.
 * - `FlagName find##FlagName(const QByteArray& str)`: Finds a flag enumeration
 *   value from its string representation.
 * - `QDataStream& operator<<(QDataStream& s, const FlagName & e)`: Writes a flag
 *   enumeration value to a QDataStream.
 * - `QDataStream& operator>>(QDataStream& s, FlagName & e)`: Reads a flag
 *   enumeration value from a QDataStream.
 * - `QDebug operator<<(QDebug& d, const FlagName & e)`: Provides a debug-friendly
 *   representation of the flag enumeration value.
 *
 * Usage of this macro simplifies the implementation of common operations on flag
 * enumerated types. It enforces a consistent and efficient approach to managing
 * flag enumeration values.
 */
#define NDEFINITION_FLAG_FUNCTION(Type, Default, FlagName, MinValue, MaxValue) \
    NDEFINITION_ENUM_FUNCTION(Type, Default, MinValue, MaxValue)\
    \
    uint16_t toNumber(const FlagName & e) { return static_cast <uint16_t> (e); } \
    \
    QList <Type> to##Type##EnumList(const FlagName & e); \
    \
    QByteArray toString(const FlagName & e) { \
        QList <Type> array = to##Type##EnumList (e); \
        QByteArrayList list; \
        for (auto it : array) \
        list.push_back(toString(it)); \
        return list.join('|'); \
    } \
    \
    FlagName find##FlagName (const QByteArray& str); \
    \
    QByteArray toByte(const FlagName & e) { \
        uint16_t number = toNumber(e); \
        return QByteArray(reinterpret_cast <const char*>(&number), sizeof(uint16_t)); \
    } \
    \
    FlagName fromByte##FlagName (const QByteArray& data){ \
        if (data.size() != sizeof(uint16_t)) return FlagName ( Type##_Default ); \
        uint16_t value = *reinterpret_cast <const uint16_t*> (data.constData()); \
        return static_cast <FlagName> (value); \
    } \
    \
    QDataStream& operator << (QDataStream& s, const FlagName & e){ \
        return s << toByte(e); \
    } \
    QDataStream& operator >> (QDataStream& s, FlagName & e){ \
        QByteArray data; \
        s >> data; \
        e = fromByte##FlagName (data); \
        return s; \
    } \
    \
    QDebug operator << (QDebug& d, const FlagName & e){ \
        return d << toNumber(e) << "[" << #FlagName << ":: (" << toString(e) << "), " \
        << QString::number(toNumber(e), 16) << "]"; \
}
// ======================================================================================


// --------------------------------------------------------------------------------------
/*!
 * \brief Macro: ENUM_AND_NUMBER(Type, number, e)
 *
 * \par Description
 * This macro converts an enumeration value (e) to its corresponding integer representation (number)
 * and casts it back to the enumeration type (Type).
 *
 * \par Parameters
 * - Type: The enumeration type.
 * - number: The integer representation of the enumeration value.
 * - e: The enumeration value.
 *
 * \par Example Usage
 * Type result = ENUM_AND_NUMBER(Type, number, e);
 */
#define ENUM_AND_NUMBER(Type, number, e) \
static_cast<Type>(number & static_cast<uint16_t>(e))


/*!
 * \macro NDEFINITION_ENUM_FUNCTION(Type, Default, MinValue, MaxValue)
 * \brief Definition of Enum Functions.
 *
 * This macro generates a comprehensive set of functions and declarations for an
 * enumeration Type. It is intended to facilitate various operations on the
 * enumeration, including conversions, string representations, and more.
 *
 * \param Type The enumeration type.
 * \param Default The default enumeration value.
 * \param MinValue The minimum possible value for the enumeration.
 * \param MaxValue The maximum possible value for the enumeration.
 *
 * The macro generates the following functions and declarations:
 *
 * - `const Type Type##_Default`: The default value for the enumeration.
 * - `uint16_t Type##_0`: The minimum value possible for the enumeration.
 * - `const QByteArray Type##_str()`: Returns a string representation of the
 *   enumeration type.
 * - `uint16_t toNumber(const Type & e)`: Converts an enumeration value to its
 *   corresponding number.
 * - `QByteArray toString(const Type & e)`: Converts an enumeration value to its
 *   string representation.
 * - `Type find##Type(const QByteArray& str)`: Finds an enumeration value from its
 *   string representation.
 * - `Type find##Type(const uint16_t& number)`: Finds an enumeration value from its
 *   numerical representation.
 * - `QByteArray toByte(const Type & e)`: Converts an enumeration value to a
 *   QByteArray for serialization.
 * - `Type fromByte##Type(const QByteArray& data)`: Converts a QByteArray to an
 *   enumeration value for deserialization.
 * - `QDataStream& operator<<(QDataStream& s, const Type & e)`: Writes an
 *   enumeration value to a QDataStream.
 * - `QDataStream& operator>>(QDataStream& s, Type & e)`: Reads an enumeration value
 *   from a QDataStream.
 * - `QDebug operator<<(QDebug d, const Type & e)`: Provides a debug-friendly
 *   representation of the enumeration value.
 *
 * Usage of this macro simplifies the implementation of common operations on
 * enumerated types. It enforces a consistent and efficient approach to managing
 * enumeration values.
 */
#define NDEFINITION_ENUM_FUNCTION(Type, Default, MinValue, MaxValue) \
const Type Type##_Default = Type :: Default; \
    uint16_t Type##_0 = MinValue; \
    \
    const QByteArray Type##_str() { return #Type; } \
    \
    uint16_t Type##Shift(const Type & e){ return static_cast<uint16_t>(e) - MinValue; } \
    Type     Type##Shift(uint16_t shift){ return static_cast<Type>(shift + MinValue); } \
    \
    uint16_t   toNumber(const Type & e) { return static_cast <uint16_t> (e); } \
    QByteArray toString(const Type & e) { return Type##Map.value(e); } \
    \
    Type find##Type(const QByteArray& str){ return Type##Map.key(str, Type##_Default);} \
    \
    Type find##Type (const uint16_t& number){ \
        if (number > MaxValue || number < MinValue) \
        return Type##_Default;  \
        return static_cast < Type > (number); \
} \
    \
    QByteArray toByte(const Type & e) { \
        uint16_t number = toNumber(e); \
        return QByteArray(reinterpret_cast <const char*>(&number), sizeof(uint16_t)); \
} \
    \
    Type fromByte##Type (const QByteArray& data){ \
        if (data.size() != sizeof(uint16_t)) return Type##_Default; \
        uint16_t value = *reinterpret_cast <const uint16_t*> (data.constData()); \
        return find##Type (value); \
} \
    \
    QDataStream& operator << (QDataStream& s, const Type & e){ \
        return s << toByte(e); \
} \
    \
    QDataStream& operator >> (QDataStream& s, Type & e){ \
        QByteArray data; \
        s >> data; \
        e = fromByte##Type (data); \
        return s; \
} \
    \
    QDebug operator << (QDebug d, const Type & e){ \
        QString msg("["); \
        msg.append(#Type); \
        msg.append("::"); \
        msg.append(toString(e)); \
        msg.append(", "); \
        msg.append(QString::number(toNumber(e), 16)); \
        msg.append("]"); \
        d.nospace() << msg.toStdString().c_str(); \
        return d.maybeSpace(); \
}


// --------------------------------------------------------------------------------------
/**
 * \brief Insert a pair of an enumeration value and its string representation into a map.
 *
 * This macro is used to insert a pair consisting of an enumeration value and its
 * string representation into a map. It simplifies the process of populating a map
 * that associates enumeration values with their corresponding strings.
 *
 * \param Type The enumeration type.
 * \param Value The enumeration value to insert.
 *
 * Example usage:
 * \code
 * N_INSERTVALUE(MyEnum, Value1)
 * N_INSERTVALUE(MyEnum, Value2)
 * \endcode
 *
 * In the above example, the macro is used to insert `Value1` and `Value2` from
 * the `MyEnum` enumeration into the map.
 */
#define N_INSERTVALUE(Type, Value) { Type :: Value , #Value }
// ======================================================================================



// --------------------------------------------------------------------------------------
/**
 * \brief Macro to declare serialization and deserialization operators for a class.
 *
 * This macro `IO(CLASS)` simplifies the declaration of friend serialization and
 * deserialization operators for a given class. It declares both `operator<<` and
 * `operator>>` as friends to the class, allowing them to access private and protected
 * members. These operators should be defined elsewhere to implement the actual
 * serialization logic using QDataStream (or any stream type aliased as QDS).
 *
 * Usage:
 * Place the `IO(CLASS)` macro inside the class definition where you want to enable
 * serialization and deserialization. Then, implement the corresponding `operator<<`
 * and `operator>>` functions to handle the class's serialization logic.
 *
 * Example:
 * \code{.cpp}
 * class MyClass {
 * public:
 *     MyClass();
 *     // Other member functions...
 *
 *     IO(MyClass)
 * };
 *
 * // Implementation of serialization and deserialization
 * QDS& operator<<(QDS& stream, const MyClass& c) {
 *     // Serialize `c` into `stream`
 *     return stream;
 * }
 *
 * QDS& operator>>(QDS& stream, MyClass& c) {
 *     // Deserialize `stream` into `c`
 *     return stream;
 * }
 * \endcode
 *
 * \param CLASS The class name to apply the serialization operators to.
 * \note Replace `QDS` with `QDataStream` or any other stream type as needed.
 */
#define IO_READ(CLASS)  QDS& operator << (QDS& s, const CLASS & c);
#define IO_WRITE(CLASS) QDS& operator >> (QDS& s,       CLASS & c);
#define IO(CLASS) \
    friend IO_READ(CLASS) \
    friend IO_WRITE(CLASS)


/**
 * \brief Macro to declare serialization and deserialization operators for QList of a class.
 *
 * This macro `IO_LIST(CLASS)` extends the functionality provided by `IO(CLASS)` to
 * support serialization and deserialization of QList containing objects of the specified
 * CLASS. It declares `operator<<` and `operator>>` as friends to the class for QList,
 * enabling them to serialize and deserialize a list of CLASS objects using QDataStream
 * (or any stream type aliased as QDS). It also includes debug output to indicate the
 * size of the list being serialized or deserialized.
 *
 * Usage:
 * Place the `IO_LIST(CLASS)` macro inside the class definition to enable serialization
 * and deserialization of QList containing that class. Ensure the `IO(CLASS)` macro is
 * also defined to handle serialization of individual objects. Then, implement the
 * necessary logic within the declared operators.
 *
 * Example:
 * \code{.cpp}
 * class MyClass {
 * public:
 *     MyClass();
 *     // Other member functions...
 *
 *     IO_LIST(MyClass)
 * };
 *
 * // Assuming IO(MyClass) is already defined and implements serialization of MyClass
 * \endcode
 *
 * The operators automatically handle the serialization of the list's length as a
 * uint16_t and iterate over the list to serialize or deserialize each object.
 *
 * \param CLASS The class name of the objects stored in the QList.
 * \note This macro assumes `QDataStream` (or QDS) and `QList` are used for
 *       serialization. It also uses qDebug() for logging the size of the list during
 *       serialization and deserialization processes. Make sure to include <QDataStream>
 *       and <QDebug> in your implementation files.
 */
#define IO_LIST_WRITE_IN(CLASS) \
        s << uint16_t(l.length());\
        qDebug() << "IO size" << uint16_t(l.length());\
        for (const auto& c : l)\
            s << c;\
        return s;\

#define IO_LIST_WRITE(CLASS) \
    QDS& operator << (QDS& s, const QList <CLASS>& l){ \
        IO_LIST_WRITE_IN(CLASS) \
    }

#define IO_LIST_READ(CLASS) \
    QDS& operator >> (QDS& s, QList <CLASS>& l){\
        l.clear();\
        uint16_t size;\
        s >> size;\
        qDebug() << "IO size >>" << size;\
        for (uint16_t i = 0; i < size; ++i){\
            CLASS c;\
            s >> c;\
            l.append(c);\
        }\
        return s;\
    }

#define IO_LIST(CLASS) \
    IO(CLASS) \
    friend IO_LIST_WRITE(CLASS) \
    friend IO_LIST_READ(CLASS)


/**
 * \brief Macro for QList<QSharedPointer<CLASS>> serialization.
 *
 * `IO_PTR_LIST(CLASS)` extends `IO(CLASS)` for QLists holding QSharedPointer
 * to CLASS objects. It declares `operator<<` and `operator>>` as friends,
 * enabling serialization with QDataStream (or QDS). It logs list sizes during
 * serialization and deserialization for debugging.
 *
 * Usage:
 * Use `IO_PTR_LIST(CLASS)` within a class to enable serialization of
 * QList<QSharedPointer<CLASS>>. Ensure `IO(CLASS)` is defined for serializing
 * CLASS objects. The macro handles the list's size as uint16_t and iterates
 * over it to serialize/deserialize each pointed-to object.
 *
 * Example:
 * \code{.cpp}
 * class MyClass {
 * public:
 *     MyClass();
 *     // Member functions...
 *
 *     IO_PTR_LIST(MyClass)
 * };
 * // IO(CLASS) must be defined for MyClass serialization
 * \endcode
 *
 * This setup ensures correct serialization of QLists containing QSharedPointer
 * objects, including serialization of the objects themselves, and reconstructs
 * the list on deserialization.
 *
 * \param CLASS The class of objects managed by QSharedPointer in QList.
 * \note Uses `QDataStream` (or QDS) for serialization. Ensure <QDataStream>
 *       and <QDebug> are included for functionality and logging.
 */
#define IO_PTR_LIST(CLASS) \
    IO(CLASS) \
    friend QDS& operator << (QDS& s, const QList <QSharedPointer <CLASS>>& l){\
        s << uint16_t(l.length());\
        qDebug() << "prime size" << uint16_t(l.length());\
        for (const auto& c : l)\
            s << *c.data();\
        return s;\
    }\
    friend QDS& operator >> (QDS& s, QList <QSharedPointer <CLASS>>& l){\
        l.clear();\
        uint16_t size;\
        s >> size;\
        qDebug() << "prime size >>" << size;\
        for (uint16_t i = 0; i < size; ++i){\
            QSharedPointer <CLASS> c(new CLASS());\
            s >> *c.data();\
            l.append(c);\
        }\
        return s;\
    }\
// ======================================================================================

#define DEBUG(CLASS) friend QDebug operator << (QDebug d, const CLASS & c);

#endif
