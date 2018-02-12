#pragma once

#include <functional>
#include <memory>
#include <experimental/optional>
#include <sstream>

#include <mutex>
#include <shared_mutex>

#include <sstream>
#include <thread>

#include <string>

#include <boost/config/detail/suffix.hpp>
#include <boost/assert.hpp>

namespace yazik {

template<typename T>
class intrusive_ptr;

template<typename T>
class intrusive_weak_ptr;

namespace utility {

enum LoggerLevel {
    LVL_DEBUG,
    LVL_INFO,
    LVL_WARN,
    LVL_ERROR,
    LVL_CRIT
};


//----------------------------------------------------------------------

inline std::string getLogPrefByLevel(LoggerLevel logLvl) {
    switch (logLvl) {
    case LVL_DEBUG:
        return "Debug: ";
    case LVL_ERROR:
        return "Error: ";
    case LVL_WARN:
        return "Warning: ";
    case LVL_INFO:
        return "Info: ";
    case LVL_CRIT:
        return "Crit: ";
    }
}

//----------------------------------------------------------------------

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

struct logger {

    //≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

    class builder {
    public:

        //----------------------------------------------------------------------

        using t = std::string;
        template <typename... T_Args>
        BOOST_FORCEINLINE
        builder(T_Args... args) {
            buildStr(std::forward<T_Args>(args)...);
        }

        //----------------------------------------------------------------------

        BOOST_FORCEINLINE
        t str() {
            return _oss.str();
        }

        //----------------------------------------------------------------------

        BOOST_FORCEINLINE
        operator t() {
            return _oss.str();
        }

        //----------------------------------------------------------------------

    private:
        std::ostringstream _oss;

        //----------------------------------------------------------------------

        template <typename T>
        BOOST_FORCEINLINE
        void buildStr(T t) {
            _oss << t;
        }

        //----------------------------------------------------------------------

        template <typename T, typename... T_Args>
        BOOST_FORCEINLINE
        void buildStr(T t, T_Args... args) {
            buildStr(t);
            buildStr(std::forward<T_Args>(args)...);
        }

        //----------------------------------------------------------------------
    };

    //≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

    template<typename... T_Args>
    static BOOST_FORCEINLINE
    void log(LoggerLevel level, T_Args... args) {
        auto str = builder(std::forward<T_Args>(args)...).str();
        std::cout << str;
//        platform::log(level,str);
    }

    //----------------------------------------------------------------------
};

//≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈≈

} //end of utility namespace

} //end of yazik namespace

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define YazikLogIMPL(LEVEL, ...) \
    ::yazik::utility::logger::log(::yazik::utility::LoggerLevel::LEVEL, __VA_ARGS__)

#define YazikLog(LEVEL, ...) \
    YazikLogIMPL(LEVEL, __FILENAME__, ":", __LINE__, ": ", ##__VA_ARGS__)

#define YInfo(...)   YazikLog(LVL_INFO, ##__VA_ARGS__)
#define YDebug(...)  YazikLog(LVL_DEBUG, ##__VA_ARGS__)
#define YWarn(...)   YazikLog(LVL_WARN, ##__VA_ARGS__)
#define YErr(...)    YazikLog(LVL_ERROR, ##__VA_ARGS__)
#define YCrit(...)   YazikLog(LVL_CRIT, ##__VA_ARGS__)

#ifndef BOOST_ASSERT_IS_VOID
#   define YAZIK_IF_DEBUG(STATE)             STATE
#   define YAZIK_IF_NO_DEBUG(STATE)
#   define YAZIK_IF_DEBUG_OR(STATE1,STATE2)  STATE1
#   define YAZIK_DO_IN_DEBUG(STATE)          do {STATE;} while(0)
#   define YAZIK_BREAKPOINT(STATE)           try {if (STATE) throw 42;} catch(...) {}
#else
#   define YAZIK_IF_DEBUG(STATE)
#   define YAZIK_IF_NO_DEBUG(STATE)          STATE
#   define YAZIK_IF_DEBUG_OR(STATE1,STATE2)  STATE2
#   define YAZIK_DO_IN_DEBUG(STATE)          do {} while(0)
#   define YAZIK_BREAKPOINT(STATE)           do {} while(0)
#endif

#ifndef BOOST_ASSERT_IS_VOID
#define YAssert(COND, ...)   \
    if (!(COND)) { YCrit("(", #COND, ")", ##__VA_ARGS__); BOOST_ASSERT(COND); }
#define YCheckWithAssert(COND, ...) \
    (COND?true:(YCrit("(", #COND, ")", ##__VA_ARGS__),BOOST_ASSERT(COND),false))
#define YCheckWithAsserttNot(COND, ...) \
    (!(COND)?false:(YCrit("(NOT ", #COND, ")", ##__VA_ARGS__),BOOST_ASSERT(!(COND)),true))
#else
#define YAssert(COND, ...)   \
    if (!(COND)) { YCrit("(", #COND, ")", ##__VA_ARGS__); }
#define YCheckWithAssert(COND, ...) \
    (COND?true:(YCrit("(", #COND, ")", ##__VA_ARGS__),false))
#define YCheckWithAsserttNot(COND, ...) \
    (!(COND)?true:(YCrit("(NOT ", #COND, ")", ##__VA_ARGS__),false))
#endif
