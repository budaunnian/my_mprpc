#include <iostream>
#include<string>
#include "user.pb.h"

using namespace fixbug;

class UserService : public UserServiceRpc
{
public:  //下面是本地服务
    bool Login(std :: string name , std :: string pwd ) 
    {
        std::cout<<"正在进行本地服务: Login"<<std::endl;
        std::cout<<"name:"<<name<< " pwd:"<<pwd<<std::endl;

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
    }

};

int main()
{
    return 0;
}


