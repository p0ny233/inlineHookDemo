#include "LogUtil.h"

int LogUtils::LOG_SWITCH = 1;

void LogUtils::DEBUG(const char* msg, MSG_LEVEL level)
{
    if (LOG_SWITCH)
        console(level, msg);
}

void LogUtils::DEBUG(std::string& msg, MSG_LEVEL level)
{
    if (LOG_SWITCH)
        console(level, msg);
}


template <class T>
void LogUtils::console(int level, T msg)
{
    switch (level)
    {
        case LOG_INFO_LEVEL:
            std::cout << "Level: INFO" << ", MSG: " << msg << std::endl;
            break;
        case LOG_WARN_LEVEL:
            std::cout << "Level: WARN" << ", MSG: " << msg << std::endl;
            break;
        case LOG_ERROR_LEVEL:
            std::cout << "Level: ERROR" << ", MSG: " << msg << std::endl;
            break;
    }
}

