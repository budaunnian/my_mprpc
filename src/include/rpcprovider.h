#pragma once 
#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <string>
#include <functional> 
#include <google/protobuf/descriptor.h>
#include <unordered_map>



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
    //muduo网络服务端
    //组合muduo网络库的epoll
    muduo::net::EventLoop * m_eventLoop;

    //用一张表来存储服务对象和服务方法的映射关系
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; //存储服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*> m_methodMap; //存储服务方法 方法名和方法描述对应
    };
    //再用一张表实现服务名和服务对象的映射关系
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;
    
    //下面两个就是处理网络连接和消息读写的回调函数
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);
    void OnMessage(const muduo::net::TcpConnectionPtr &conn,muduo::net::Buffer *buffer,muduo::Timestamp receiveTime);

    // 用于Closure回调操作，实现序列化rpc响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn,google::protobuf::Message *response);
    
};