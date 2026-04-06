#include <iostream>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

int main(int argc,char **argv)
{

    /*下面就是客户端发起请求的整个流程*/
    //进行mprpc框架的初始化
    MprpcApplication::Init(argc,argv);

    //调用发布在远程rpc中的方法
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    //先配置参数
    fixbug::LoginRequest request;
    request.set_name("buduan nian");
    request.set_pwd("123456");

    //再配置响应对象
    fixbug::LoginResponse response;

    //最后发起rpc方法的调用，等待返回结果
    stub.Login(nullptr, &request,&response, nullptr);//RpcChannel->RpcChannel::callMethod
    
    //验证返回的结果
    if(0== response.result().errcode())
    {
        std::cout<<"rpc login() 请求成功："<<response.success()<<std::endl;
    }
    else
    {
        std::cout<<"rpc login() 请求失败："<<response.result().errmsg()<<std::endl;
    }

    //演示客户端的注册
    fixbug::RegisterRequest register_request;
    register_request.set_id(1000);
    register_request.set_name("zhang san");
    register_request.set_pwd("666666666");  
    fixbug::RegisterResponse register_response;
    stub.Register(nullptr, &register_request,&register_response, nullptr);
    if(0== register_response.result().errcode())
    {
        std::cout<<"rpc register() 请求成功："<<register_response.success()<<std::endl;
    }
    else    
    {
        std::cout<<"rpc register() 请求失败："<<register_response.result().errmsg()<<std::endl;
    }

    // 演示客户端：获取用户信息
    fixbug::GetUserInfoRequest info_request;
    info_request.set_userid(1000);  
    fixbug::GetUserInfoResponse info_response;
    stub.GetUserInfo(nullptr, &info_request, &info_response, nullptr);
    if(0 == info_response.result().errcode())
    {
        std::cout<<"rpc GetUserInfo() 请求成功"<<std::endl;
        std::cout<<"userid: "<< info_response.userinfo().id() << std::endl;
        std::cout<<"username: "<< info_response.userinfo().name() << std::endl;
    }
    else    
    {
        std::cout<<"rpc GetUserInfo() 请求失败："<<info_response.result().errmsg()<<std::endl;
    }

    // 演示客户端：退出登录
    fixbug::LogoutRequest logout_request;
    logout_request.set_userid(1000);  
    fixbug::LogoutResponse logout_response;
    stub.Logout(nullptr, &logout_request, &logout_response, nullptr);
    if(0 == logout_response.result().errcode())
    {
        std::cout<<"rpc Logout() 请求成功："<<logout_response.success()<<std::endl;
    }
    else    
    {
        std::cout<<"rpc Logout() 请求失败："<<logout_response.result().errmsg()<<std::endl;
    }

    // 演示客户端：获取用户列表
    fixbug::GetUserListRequest list_request;
    list_request.set_page(1);  
    list_request.set_limit(10);
    fixbug::GetUserListResponse list_response;
    stub.GetUserList(nullptr, &list_request, &list_response, nullptr);
    if(0 == list_response.result().errcode())
    {
        std::cout<<"rpc GetUserList() 请求成功"<<std::endl;
        // 打印列表
        for(int i=0; i<list_response.userlist_size(); i++)
        {
            const fixbug::UserInfo& user = list_response.userlist(i);
            std::cout<<"userid: "<<user.id()<<" name: "<<user.name()<<std::endl;
        }
    }
    else    
    {
        std::cout<<"rpc GetUserList() 请求失败："<<list_response.result().errmsg()<<std::endl;
    }

    return 0;
}