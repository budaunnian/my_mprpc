#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main()
{
    // //创建响应
    // LoginResponse rsp;
    // ResultCode *rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录处理失败了");

    //总结：也就是说protobuf里面的成员里面有另一个成员对象就通过
    //rsp.mutable_对象名()来获取指针，然后用这个指针来进行赋值操作

    GetFriendListResponse rsp;//创建响应好友队列对象
    ResultCode *rc =  rsp.mutable_result();//获取错误码对象的指针
    rc->set_errcode(0);//利用指针给错误码初始化

    //添加用户user1到队列总去
    User *user1= rsp.add_friedn_list();
    user1->set_name("张三");
    user1->set_age(999);
    user1->set_sex(User::MAN);

    //添加用户user2到队列中去
    User *user2=rsp.add_friedn_list();
    user2->set_name("李四");
    user2->set_age(888);
    user2->set_sex(User::WOMAN);

    //进行输出
    std::cout<<rsp.friedn_list_size()<<std::endl;



    
    return 0 ;
}




// int main1()
// {
//     /*--------------请求--------------------*/
//     // //根据protobuf的请求格式，补充数据
//     // LoginRequest req;//根据消息体实例对象
//     // req.set_name("zhang san");//初始话消息体中的变量
//     // req.set_pwd("123456789");

//     // //进行序列化处理
//     // std::string send_str;
//     // if(req.SerializeToString(&send_str))
//     // {
//     //     std::cout <<send_str.c_str() << std::endl;
//     // }

//     // //进行一个数据的反序列化
//     // LoginRequest reqB;
//     // if(reqB.ParseFromString(send_str))//反序列化函数传入的是数据已经序列化的字符串变量
//     // {
//     //     // 对象接收
//     //     std::cout << reqB.name()<<std::endl;
//     //     std::cout<< reqB.pwd()<<std::endl;
//     // }

//     /*-------------------响应--------------------*/
//     LoginResponse rps;
//     rps.set_errcode(666);//通过set函数对参数进行初始话
//     rps.set_errmsg("牛逼失败了");
//     rps.set_success(1);

//     //请求序列化
//     std::string  send_rps;//定义一个字符串变量接收序列化的数据
//     if(rps.SerializeToString(&send_rps)){
//         std::cout<< send_rps.c_str()<<std::endl;//c_str()函数是调用序列化后的数据

//     }

//     //反序列化
//     //1.创建对象接收
//     LoginResponse rpsB;
//     if(rpsB.ParseFromString(send_rps))
//     {
//         //传入接收后的数据后，通过过对象调用变量
//         std::cout<<rpsB.errcode()<<std::endl;
//         std::cout<<rpsB.errmsg()<<std::endl;
//         std::cout<<rpsB.success()<<std::endl;
//     }

    
//     //return 0;
// }