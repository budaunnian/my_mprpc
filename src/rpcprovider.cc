#include "rpcprovider.h"
#include <string>
#include "mprpcapplication.h"

//给外部可以发布rpc服务的函数接口  用基类指针接收子类对象
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

}

    //启动Rpc服务节点，开始提供远程服务服务
void RpcProvider::Run()
{   
    //智能指针初始化
    m_eventLoop = new muduo::net::EventLoop();
    //获取ip
    std::string ip = MprpcApplication::GetInstance().Getconfig().Load("rpcserviceip");
    //获取无符号整型的端口
    uint16_t port = atoi(MprpcApplication::GetInstance().Getconfig().Load("rpcserviceport").c_str());
    //连接provider的ip和端口
    muduo::net::InetAddress address(ip,port);

    //创建连接tcpserviced对象
    muduo::net::TcpServer server(m_eventLoop,address,"RpcProvider");//第一个参数就线程调度用的，用于处理事件

    //绑定连接回调函数和消息读写回调方法,分离网络连接和消息读写处理
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    //设置muduo库的线程数量，muduo库会自己分配I/O线程和worker线程
    server.setThreadNum(4);

    //打印日志，输出当前rpc服务节点的信息
    std::cout<<"RpcProvider开始服务: ip: "<<ip<<" port: "<<port<<std::endl;

    //接下来就开启动服务阻塞监听了
    server.start();
    m_eventLoop->loop();

}

//获取新的网络连接
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    

}

//用于处理已经建立连接的用户进行读写事件的回调
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn,muduo::net::Buffer *buffer,muduo::Timestamp receiveTime)
{

}