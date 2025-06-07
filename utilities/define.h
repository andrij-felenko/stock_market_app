#ifndef UTIL_DEFINE_H
#define UTIL_DEFINE_H

// --------------------------------------------------------------------------------------
#define GETVAR(type, name) \
private: \
    type _##name; \
public: \
    type name() const { return _##name; } \
private:
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
#define PROPREADF(type, name, func) \
private: \
    Q_PROPERTY(type name READ func NOTIFY name##Changed FINAL) \
    type _##name; \
signals: \
    void name##Changed(type); \
private:
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
#define PROPREAD(type, name) \
private: \
    Q_PROPERTY(type name READ name NOTIFY name##Changed FINAL) \
GETVAR(type, name) \
signals: \
    void name##Changed(type); \
private:
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
#define PROPFULLFR(type, name, func) \
private: \
    Q_PROPERTY(type name READ name WRITE set_##name NOTIFY name##Changed FINAL) \
GETVAR(type, name)\
signals: \
    void name##Changed(type); \
public: \
    void set_##name(type newv) { \
        if (_##name == newv) \
            return; \
        _##name = newv; \
        emit name##Changed(newv); \
    } \
private:
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
#define PROPFULLFS(type, name, func) \
private: \
    Q_PROPERTY(type name READ name WRITE set NOTIFY name##Changed FINAL) \
GETVAR(type, name)\
signals: \
    void name##Changed(type name); \
private:
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
#define PROPFULLFF(type, name, read, set) \
    Q_PROPERTY(type name READ read WRITE set NOTIFY name##Changed FINAL) \
private: \
    type _##name; \
signals: \
    void name##Changed(type);  \
private:
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
#define PROPFULL(type, name) \
private: \
    Q_PROPERTY(type name READ name WRITE set_##name NOTIFY name##Changed FINAL) \
GETVAR(type, name)\
public: \
    void set_##name(type newv) { \
        if (_##name == newv) \
            return; \
        _##name = newv; \
        emit name##Changed(newv); \
    } \
private:
// --------------------------------------------------------------------------------------

#define PROP(type, name) \
private: type _##name; \
public: type name##() const { return _##name; } \
private:

#endif
