#include <iostream>
#include<string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>
#include "logger.h"

using namespace fixbug;

class FriendService : public FriendServiceRpc
{
public:  //下面是本地服务
    // 本地业务：添加好友
    std::vector<std::string> GetFriendsList(uint32_t userid)
    {   
        std::cout<<"正在进行本地服务: GetFriendList"<<std::endl;
        std::cout<<"userid:"<<userid<<std::endl;
        std::vector<std::string> friend_list;
        friend_list.push_back("zhang san");
        friend_list.push_back("li si");
        friend_list.push_back("wang wu");
        return friend_list;
    }

    //重写基类FriendServiceRpc的虚函数，让rps框架调用本地服务
    void GetFriendsList(::google::protobuf::RpcController* controller,
                        const ::fixbug::GetFriendsListRequest*request,
                        ::fixbug::GetFriendsListResponse*response,
                        ::google::protobuf::Closure*done)
    {   /*下面就是进行实现*/
        //框架通过调用这个虚函数，会传入一个GetFriendListRequest对象，通过
        //这个对象获取相应的数据 
        uint32_t userid = request->userid();
        std::vector<std::string> friendid_list=GetFriendsList(userid);

        //设置响应
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);       // 正确
        code->set_errmsg("success"); // 正确
        //拿出容器中的好友，传回reponse对象中
        for (const auto& friendid : friendid_list)
        {   
            response->add_friends(friendid);
        }

        done->Run();
    }
};

int main(int argc,char **argv)
{
    // //测试日志系统
    // LOG_INFO("第一次日志信息！！");
    // LOG_ERR("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
    
    //进行rpc初始化操作
    MprpcApplication::Init(argc,argv);

    // provider是rpc提供的一个网络服务对象，将UserService对象发布到rpc节点上去
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    //启动rpc服务发布节点，调用run()进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();



    return 0;
}