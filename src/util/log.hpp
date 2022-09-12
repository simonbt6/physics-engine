#pragma once

#include <string>

#define PRINT_INFO true
#define PRINT_WARN true
#define PRINT_ERROR true
#define PRINT_CRITICAL true

#define LOG_INFO true
#define LOG_WARN true
#define LOG_ERROR true
#define LOG_CRITICAL true

// Defines .*ext* without separating dot.
#define LOG_FILE_EXT "log"

// Defines relative/absolute folder path with final separator.
#define LOG_FOLDER_PATH "log/"

namespace Util
{
    class Log
    {
    public:
        static void Info(const std::string& msg);
        static void Warn(const std::string& msg);
        static void Error(const std::string& msg);
        static void Critical(const std::string& msg);
    
    private:
        static void _print(const std::string& msg);
        static void _log(const std::string& msg);
    };
};