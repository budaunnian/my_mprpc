#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>



MprpcConfig MprpcApplication::m_config;

// 类外实现静态方法不用static
void ShowArgsHelp()
{
    std::cout << "错误：没有对应的配置文件" << std::endl;
}
// 进行初始化操作
void MprpcApplication::Init(int argc, char **argv)
{
    if (argc < 2)
    {
        ShowArgsHelp();     // 输出一下错误提示帮助信息
        exit(EXIT_FAILURE); // 让程序立刻停止
    }

    // 获取正确的配置文件
    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1) // 主要作用就是获取命令行的参数
    {
        switch (c)
        {
        case 'i':                 // 拿到配置文件
            config_file = optarg; // optarg全局变量
            break;
        case '?': // 无效参数

            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':': // 缺少参数值

            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    // 接下来就开始配置文件
    // 主要就是是配置rpc的ip和端口，以及zookeeper的ip和端口
    m_config.LoadConfigFile(config_file.c_str());
    
    // std::cout<<"rpcserviceip: "<<m_config.Load("rpcserviceip")<<std::endl;
    // std::cout<<"rpcserviceport: "<<m_config.Load("rpcserviceport")<<std::endl;
    // std::cout<<"zookeeperip: "<<m_config.Load("zookeeperip")<<std::endl;
    // std::cout<<"zookeeperport: "<<m_config.Load("zookeeperport")<<std::endl;


    
}
// 初始化
MprpcApplication &MprpcApplication::GetInstance() // 用static可以用类名调用函数，并且返回单例对象
{
    static MprpcApplication app;
    return app;
}
