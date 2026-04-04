#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
// std::bind需要这个头文件    


//给外部可以发布rpc服务的函数接口  用基类指针接收子类对象
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    //创建传入的服务类型接收的表
    ServiceInfo service_info;

    //获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    //获取服务的名字
    std::string service_name = pserviceDesc->name();
    //获取服务的方法
    int methodCnt = pserviceDesc->method_count();

    //打印日志：输出服务名字
    std::cout<<"服务名字: "<<service_name<<std::endl;

    for(int i=0;i<methodCnt;i++)
    {
        //获取服务方法的描述信息
        const google::protobuf::MethodDescriptor *pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        //打印日志：输出服务方法名字
        std::cout<<"方法"<<i+1<<": "<<method_name<<std::endl;
        //将方法名字和方法描述存入表中
        service_info.m_methodMap.insert({method_name,pmethodDesc});
    }
    //将服务类型和服务对象存入表中
    service_info.m_service = service;
    m_serviceMap.insert({service_name,service_info});
}

    //启动Rpc服务节点，开始提供远程服务服务
void RpcProvider::Run()
{   
    //组合muduo网络库的epoll
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
    //如果断开连接就直接关闭文件描述符
    if (!conn->connected())
    {
        conn->shutdown();
    }
    

}

//用于处理已经建立连接的用户进行读写事件的回调
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp receiveTime)
{
    //接受网络发送过来的数据
    std::string recv_bug = buffer->retrieveAllAsString();

    //接收服务名和对象的名字
    uint32_t header_size =  0;
    recv_bug.copy((char*)&header_size,4,0);//将4字节长度转换为二进制长度，后续解析使用

    //开始截取真正的数据
    std::string rpc_header_str = recv_bug.substr(4,header_size);    
    mprpc::RpcHeader rpc_header;//创建对象，接受protobuf数据
    //反序列化
    std::string service_name;
    std::string method_name;    
    uint32_t args_size = 0;
    if(rpc_header.ParseFromString(rpc_header_str))
    {
        //数据解析成功
        service_name = rpc_header.service_name();
        method_name = rpc_header.method_name();
        args_size = rpc_header.args_size();
    }
    else{
        //数据解析失败
        std::cout<<"rpc_header_str: "<<rpc_header_str<<std::endl;
        std::cout<<"数据解析失败！"<<std::endl;
        return;
    }

    //获取参数的字节流信息  ？？？目前还不知道为什么不需要解析
    std::string args_str = recv_bug.substr(4+header_size,args_size);

    //接下来打印日志，输出服务名字和方法名字
    std::cout<<"=================================================="<<std::endl; 
    std::cout<<"文件头大小: "<<header_size<<std::endl; 
    std::cout<<"文件头字符串: "<<rpc_header_str<<std::endl;
    std::cout<<"服务名字: "<<service_name<<std::endl;
    std::cout<<"方法名字: "<<method_name<<std::endl;    
    std::cout<<"参数字符串: "<<args_str<<std::endl;
    std::cout<<"=================================================="<<std::endl; 
    

}