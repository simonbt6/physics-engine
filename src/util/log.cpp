#include "log.hpp"

#include <iostream>
#include <fstream>
#include <format>
#include <ctime>

#include <windows.h>

using namespace Util;

void Log::Info(const std::string& msg)
{
#if PRINT_INFO
    _print(std::format("[INFO] {}.", msg));
#endif
#if LOG_INFO
    _log(std::format("[INFO] {}.", msg));
#endif
}

void Log::Warn(const std::string& msg)
{
#if PRINT_WARN
    _print(std::format("[WARN] {}.", msg));
#endif
#if LOG_WARN
    _log(std::format("[WARN] {}.", msg));
#endif
}

void Log::Error(const std::string& msg)
{
#if PRINT_ERROR
    _print(std::format("[ERROR] {}.", msg));
#endif
#if LOG_ERROR
    _log(std::format("[ERROR] {}.", msg));
#endif
}

void Log::Critical(const std::string& msg)
{
#if PRINT_CRITICAL
    _print(std::format("[CRITICAL] {}.", msg));
#endif
#if LOG_CRITICAL
    _log(std::format("[CRITICAL] {}.", msg));
#endif
}

void Log::_print(const std::string& msg)
{
    struct tm buf;
    auto time = std::time(0);
    localtime_s(&buf, &time);

    std::cout << std::format("[{:02d}:{:02d}:{:02d}]{}", buf.tm_hour, buf.tm_min, buf.tm_sec, msg) << std::endl;
}

void Log::_log(const std::string& msg)
{
    std::ofstream log_file;
    
    struct tm buf;
    auto time = std::time(0);
    localtime_s(&buf, &time);

    // Create directory if not exists.
    if (CreateDirectory(LOG_FOLDER_PATH, 0) || ERROR_ALREADY_EXISTS == GetLastError())
    {
            // Logging to file
        log_file.open(
            std::format(
                "{}{}_{}_{}_{}_{}.{}", 
                LOG_FOLDER_PATH, 
                "log", 
                buf.tm_mday, 
                buf.tm_mon, 
                buf.tm_year, 
                buf.tm_hour, 
                LOG_FILE_EXT),
            std::ios::out | std::ios::app | std::ios::ate);
            
        if (!log_file.is_open())
        {
            _print("[CRITICAL] Couldn't open log file.\n");
            return;
        }

        log_file << std::format("[{:02d}:{:02d}:{:02d}]{}", buf.tm_hour, buf.tm_min, buf.tm_sec, msg) << std::endl;

        log_file.close();
    }
    else
    {
        _print("[CRITICAL] Couldn't create directory or access existing.\n");
    }
}