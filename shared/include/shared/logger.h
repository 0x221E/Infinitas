#pragma once

#include <source_location>
#include <print>

namespace shared
{
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL
    };

    template<LogLevel level, class...ARGS>
    struct Log{};

    template<LogLevel level, class...ARGS>
    struct Log<level, const char*, ARGS...>
    {
        Log(const char* message, ARGS...args, std::source_location loc = std::source_location::current())
        {
            std::println("[{}] [{}-l{}]: {}", GetLevel(), loc.file_name(), loc.line(), std::vformat(message, std::make_format_args(args...)));
        }

        std::string_view GetLevel()
        {
            if constexpr(level == LogLevel::DEBUG)
            {  
                return "Debug";
            }else if constexpr(level == LogLevel::INFO)
            {
                return "Info"; 
            }
            else if constexpr(level == LogLevel::WARN)
            {
                return "Warning";
            }
            else if constexpr(level == LogLevel::ERROR)
            {
                return "Error";
            }
            else if constexpr(level == LogLevel::CRITICAL)
            {
                return "Critical";
            }
            return "Unknown";
        }
    };


    template<LogLevel L, class...ARGS>
    Log(ARGS...) -> Log<L, ARGS...>;

    template<class...ARGS>
    using DebugLog = Log<LogLevel::DEBUG, ARGS...>;

    template<class...ARGS>
    using InfoLog = Log<LogLevel::INFO, ARGS...>;

    template<class...ARGS>
    using WarnLog = Log<LogLevel::WARN, ARGS...>;

    template<class...ARGS>
    using ErrorLog = Log<LogLevel::ERROR, ARGS...>;

    template<class...ARGS>
    using CriticalLog = Log<LogLevel::CRITICAL, ARGS...>;

}

#ifdef Debug
    #define LOG_DEBUG(...) :: shared::DebugLog(__VA_ARGS__)
    #define LOG_INFO(...) :: shared::InfoLog(__VA_ARGS__)
    #define LOG_WARN(...) :: shared::WarnLog(__VA_ARGS__)
    #define LOG_ERROR(...) :: shared::ErrorLog(__VA_ARGS__)
    #define LOG_CRITICAL(...) :: shared::CriticalLog(__VA_ARGS__)
#else
    #define LOG_TRACE(...)
    #define LOG_DEBUG(...) 
    #define LOG_INFO(...) 
    #define LOG_WARN(...) 
    #define LOG_ERROR(...)  
    #define LOG_CRITICAL(...)  
#endif

#define LOG_PRODUCTION(...) :: shared::InfoLog(__VA_ARGS__)
