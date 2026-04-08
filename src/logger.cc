#include "logger.h"
#include <time.h>
#include <iostream>

//设置获取单例对对象函数
Logger& Logger::GetInstance()
{
    //创建静态对象
    static Logger logger;
    return logger;
}
//构造函数启动专门的写日志线程
Logger::Logger()
{
    std::thread writeLogTask([&]()
    {
        for(;;)
        {
            //获取文件的日期
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name,"%d-%d-%d-log.txt",nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);

            //打开文件
            FILE *pf = fopen(file_name,"a+");
            if(pf==nullptr)
            {
                //文件打不开，给出错误提示
                std::cout<<"文件: "<<file_name<<" 打开错误! "<<std::endl;
                exit(EXIT_FAILURE);
            }

            

            //从队列中取出错误信息，并且写入文件
            std::string msg = m_lckQue.Pop();

            //添加时分秒信息
            char time_buf[128] = {0};
            sprintf(time_buf,"%d:%d:%d =>[%s] ",
                nowtm->tm_hour,
                nowtm->tm_min,
                nowtm->tm_sec,
                (m_loglevel== INFO ? "info" : "error"));
            msg.insert(0,time_buf);
            msg.append("\n");

            fputs(msg.c_str(),pf);
            fclose(pf);
        }
        
    });
    //设置分离线程，也就是守护线程
    writeLogTask.detach();
}

//写日志设置
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

//写日志
void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}