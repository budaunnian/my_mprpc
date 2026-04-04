#pragma once 
#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>


//构建框架提供给外面使用的，可以发布rpc服务的网络对象类
class RpcProvider
{
public:
    //给外部可以发布rpc服务的函数接口  用基类指针接收子类对象  
    // service指针可以指向继承它的类下面任意继承类
    void NotifyService(google::protobuf::Service *service);
    
    //启动Rpc服务节点，开始提供远程服务服务
    void Run();

private:
    //组合muduo网络库的epoll
    muduo::net::EventLoop * m_eventLoop;
    //下面两个就是处理网络连接和消息读写的回调函数
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);
    void OnMessage(const muduo::net::TcpConnectionPtr &conn,muduo::net::Buffer *buffer,muduo::Timestamp receiveTime);
};