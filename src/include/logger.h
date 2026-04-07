#pragma once
#include <string>
#include "lockqueue.h"


enum LogLevel
{
    INFO,//普通信息
    ERROR,//错误信息
};

class Logger
{
public:
    //设置获取单例对对象函数
    static Logger& GetInstance();
    //写日志设置
    void SetLogLevel(LogLevel level);
    //写日志
    void Log(std::string msg);
private:
    int m_loglevel;
    LockQueue<std::string> m_lckQue; //写日志缓存

    //单例模式
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) =delete;

};

//定义宏打包处理日志
#define LOG_INFO(logmsgformat,...)\
    do \
    {  \
        Logger& logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO);\
        char c[1024] ={0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__); \
        logger.Log(c); \
    } while(0)

#define LOG_ERR(logmsgformat,...)\
    do \
    {  \
        Logger& logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR);\
        char c[1024] ={0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__); \
        logger.Log(c); \
    } while(0)
   
    