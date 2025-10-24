#pragma once

#include <spdlog/spdlog.h>
//#define CM_DEBUG

#ifdef DEBUG
    #define LOG_TRACE(...) :: spdlog::trace(__VA_ARGS__)
    #define LOG_DEBUG(...) :: spdlog::debug(__VA_ARGS__)
    #define LOG_INFO(...) :: spdlog::info(__VA_ARGS__)
    #define LOG_WARN(...) :: spdlog::warn(__VA_ARGS__)
    #define LOG_ERROR(...) :: spdlog::error(__VA_ARGS__)
    #define LOG_CRITICAL(...) :: spdlog::critical(__VA_ARGS__)
#else
    #define LOG_TRACE(...)
    #define LOG_DEBUG(...) 
    #define LOG_INFO(...) 
    #define LOG_WARN(...) 
    #define LOG_ERROR(...)  
    #define LOG_CRITICAL(...)  
#endif

#define LOG_PRODUCTION(...) :: spdlog::info(__VA_ARGS__)

#if defined(CM_DEBUG)
    #define LOG_CM(...) :: spdlog::debug(__VA_ARGS__)
#else
    #define LOG_CM(...)
#endif