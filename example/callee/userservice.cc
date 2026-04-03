#include <iostream>
#include<string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

using namespace fixbug;

class UserService : public UserServiceRpc
{
public:  //下面是本地服务
    bool Login(std :: string name , std :: string pwd ) 
    {
        std::cout<<"正在进行本地服务: Login"<<std::endl;
        std::cout<<"name:"<<name<< " pwd:"<<pwd<<std::endl;
        return true;
    }

    //重写基类UserServiceRpc的虚函数，让rps框架调用本地服务
    void Login(::google::protobuf::RpcController* controller,
                        const ::fixbug::LoginRequest*request,
                        ::fixbug::LoginResponse*response,
                        ::google::protobuf::Closure*done)
    {   /*下面就是进行实现*/
        //框架通过调用这个虚函数，会传入一个LoginRequest对象，同构这个对象获取相应的数据
        std::string name = request->name();
        std::string pwd = request->pwd();

        //拿到数据后调用本地服务
        bool login_result = Login(name,pwd);

        // 把响应写入，包括错误码和错误信息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        //调用回调函数
        done->Run();
    }

};

int main(int argc,char **argv)
{

    
    //进行rpc初始化操作
    MprpcApplication::Init(argc,argv);

    // provider是rpc提供的一个网络服务对象，将UserService对象发布到rpc节点上去
    RpcProvider provider;
    provider.NotifyService(new UserService());

    //启动rpc服务发布节点，调用run()进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();



    return 0;
}


