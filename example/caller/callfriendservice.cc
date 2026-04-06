#include <iostream>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

int main(int argc,char **argv)
{

    /*下面就是客户端发起请求的整个流程*/
    //进行mprpc框架的初始化
    MprpcApplication::Init(argc,argv);

    //调用发布在远程rpc中的方法
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());

    //先配置参数
    fixbug::GetFriendsListRequest request;
    request.set_userid(1000);

    //再配置响应对象
    fixbug::GetFriendsListResponse response;

    //最后发起rpc方法的调用，等待返回结果
    stub.GetFriendsList(nullptr, &request,&response, nullptr);//RpcChannel->RpcChannel::callMethod
    
    //验证返回的结果
    if(0== response.result().errcode())
    {
        std::cout<<"rpc GetFriendList() 请求成功："<<std::endl;
        //拿出容器中的好友，打印出来
        for (int i = 0; i < response.friends_size(); ++i)
        {
            std::cout<<"好友 "<<i+1<<": "<<response.friends(i)<<std::endl;
        }   

    }
    else
    {
        std::cout<<"rpc GetFriendList() 请求失败："<<response.result().errmsg()<<std::endl;
    }

    return 0;
}