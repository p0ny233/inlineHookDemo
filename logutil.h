#pragma once
#include <string>
#include <iostream>

typedef enum _MSG_LEVEL
{
    LOG_INFO_LEVEL = 1,
    LOG_WARN_LEVEL = 2,
    LOG_ERROR_LEVEL = 3,

} MSG_LEVEL;

class LogUtils
{
    static int LOG_SWITCH;
    template <class T>
    static void console(int level, T msg);
public:
    static void DEBUG(std::string& msg, MSG_LEVEL level = LOG_INFO_LEVEL);
    void DEBUG(const char* msg, MSG_LEVEL level = LOG_INFO_LEVEL);
};
