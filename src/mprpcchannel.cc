#include "mprpcchannel.h"
#include <string>
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h> //close()函数需要这个头文件
#include <netinet/in.h>  // 正确（Linux网络编程头文件）
#include <errno.h>
#include"zookeeperutil.h"


//所有通过调用rpc方法都在这个函数中进行数据序列化和网络发送
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller,
                    const google::protobuf::Message *request,
                    google::protobuf::Message *response,
                    google::protobuf::Closure *done)
{
    //先创建一个method服务对象
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();//获取服务名称
    std::string method_name = method->name();//获取方法名称

    //获取参数的序列化字符串长度
    uint32_t args_size = 0;
    std::string args_str;
    //进行参数的序列化，序列化成功后就可以获取到序列化字符串的长度了
    if(request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("参数序列化失败!");
        return;
    }

    //设置发送的数据的请求头
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    //对请求头进行序列化 服务对象+方法+参数长度
    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();    
    }
    else
    {
        controller->SetFailed("请求头序列化失败!");
        return;
    }

    //添加包头长度并且组织发送数据格式
    std::string send_rpc_str;
    send_rpc_str.insert(0,std::string((char *)&header_size,4));
    send_rpc_str += rpc_header_str;//请求头
    send_rpc_str += args_str;//请求头+参数

    
    //使用tcp连接进行数据发送
    int clientfd = socket(AF_INET,SOCK_STREAM,0);//创建socket文件描述符
    if(clientfd < 0)
    {
        controller->SetFailed("创建socket失败!");
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    //获取rpc服务的ip和端口号
    // std::string ip = MprpcApplication::GetInstance().Getconfig().Load("rpcserviceip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().Getconfig().Load("rpcserviceport").c_str());

    //从zk中获取服务界节点的IP和port
    ZkClient zkCli;
    zkCli.Start();
    std::string methon_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkCli.GetData(methon_path.c_str());
    if(host_data == "")
    {
        controller->SetFailed(methon_path+ "不存在！");
        return;
    }
    int idx = host_data.find(":");
    if(idx==-1)
    {
        controller->SetFailed(methon_path+"地址无效！");
        return ;
    }
    std::string ip  = host_data.substr(0,idx);
    uint16_t port = atoi(host_data.substr(idx+1,host_data.size()-idx).c_str());

    
     //接下来打印日志，输出服务名字和方法名字
    std::cout<<"=================================================="<<std::endl; 
    std::cout<<"文件头大小: "<<header_size<<std::endl; 
    std::cout<<"文件头字符串: "<<rpc_header_str<<std::endl;
    std::cout<<"服务名字: "<<service_name<<std::endl;
    std::cout<<"方法名字: "<<method_name<<std::endl;    
    std::cout<<"参数字符串: "<<args_str<<std::endl;
    std::cout<<"=================================================="<<std::endl; 
    //连接rpc服务端
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //连接服务器
    if(connect(clientfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
    {
        controller->SetFailed("连接服务器失败!");
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    //发送数据
    if(-1==send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0))
    {
        controller->SetFailed("发送数据失败!");
        close(clientfd);
        return;
    }

    //接收rpc响应数据
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1==(recv_size=recv(clientfd,recv_buf,1024,0)))
    {
        controller->SetFailed("接收数据失败!");
        close(clientfd);
        return;
    }

    //反序列化数据
    
    if(!response->ParseFromArray(recv_buf, recv_size))
    {
        controller->SetFailed("反序列化响应数据失败!");
        close(clientfd);
        return;
    }

    close(clientfd);







}