#include <iostream>
#include<string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

using namespace fixbug;

class UserService : public UserServiceRpc
{
public:  //下面是本地服务
    // 本地业务：登录
    bool Login(std :: string name , std :: string pwd ) 
    {
        std::cout<<"正在进行本地服务: Login"<<std::endl;
        std::cout<<"name:"<<name<< " pwd:"<<pwd<<std::endl;
        return true;
    }

    // 本地业务：注册
    bool Register(uint32_t id, std :: string name , std :: string pwd ) 
    {
        std::cout<<"正在进行本地服务: Register"<<std::endl;
        std::cout<<"id:"<<id<<" name:"<<name<< " pwd:"<<pwd<<std::endl;
        return true;
    }

    // 本地业务：获取用户信息
    bool GetUserInfo(uint32_t userid)
    {
        std::cout<<"正在进行本地服务: GetUserInfo"<<std::endl;
        std::cout<<"userid:"<<userid<<std::endl;
        return true;
    }

    // 本地业务：退出登录
    bool Logout(uint32_t userid)
    {
        std::cout<<"正在进行本地服务: Logout"<<std::endl;
        std::cout<<"userid:"<<userid<<std::endl;
        return true;
    }

    // 本地业务：获取用户列表
    bool GetUserList(uint32_t page, uint32_t limit)
    {
        std::cout<<"正在进行本地服务: GetUserList"<<std::endl;
        std::cout<<"page:"<<page<<" limit:"<<limit<<std::endl;
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
        code->set_errmsg("方法调用失败!");
        response->set_success(login_result);
        

        //调用回调函数
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                        const ::fixbug::RegisterRequest*request,
                        ::fixbug::RegisterResponse*response,
                        ::google::protobuf::Closure*done)
    {
        //框架通过调用这个虚函数，会传入一个LoginRequest对象，同构这个对象获取相应的数据
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        //拿到数据后调用本地服务
        bool register_result = Register(id,name,pwd);

        // 把响应写入，包括错误码和错误信息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("方法调用失败!");
        response->set_success(register_result);
        

        //调用回调函数
        done->Run();
    }   

    void GetUserInfo(::google::protobuf::RpcController* controller,
                        const ::fixbug::GetUserInfoRequest*request,
                        ::fixbug::GetUserInfoResponse*response,
                        ::google::protobuf::Closure*done)
    {   /*下面就是进行实现*/
        //框架通过调用这个虚函数，会传入一个GetUserInfoRequest对象，通过这个对象获取相应的数据
        uint32_t userid = request->userid();

        //拿到数据后调用本地服务
        bool info_result = GetUserInfo(userid);

        // 把响应写入，包括错误码和错误信息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");

        //填充用户信息
        fixbug::UserInfo *user = response->mutable_userinfo();
        user->set_id(userid);
        user->set_name("test_user");
        
        //调用回调函数
        done->Run();
    }

    void Logout(::google::protobuf::RpcController* controller,
                        const ::fixbug::LogoutRequest*request,
                        ::fixbug::LogoutResponse*response,
                        ::google::protobuf::Closure*done)
    {
        //框架通过调用这个虚函数，会传入一个LogoutRequest对象，通过这个对象获取相应的数据
        uint32_t userid = request->userid();

        //拿到数据后调用本地服务
        bool logout_result = Logout(userid);

        // 把响应写入，包括错误码和错误信息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(logout_result);
        

        //调用回调函数
        done->Run();
    }

    void GetUserList(::google::protobuf::RpcController* controller,
                        const ::fixbug::GetUserListRequest*request,
                        ::fixbug::GetUserListResponse*response,
                        ::google::protobuf::Closure*done)
    {
        //框架通过调用这个虚函数，会传入一个GetUserListRequest对象，通过这个对象获取相应的数据
        uint32_t page = request->page();
        uint32_t limit = request->limit();

        //拿到数据后调用本地服务
        bool list_result = GetUserList(page,limit);

        // 把响应写入，包括错误码和错误信息，返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        

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


