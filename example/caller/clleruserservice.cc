#include <iostream>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

int mian(int argc,char **argv)
{
    //进行mprpc框架的初始化
    MprpcApplication::Init(argc,argv);

    //调用发布在远程rpc中的方法
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // stub.Login();//RpcChannel->RpcChannel::callMethod
    return 0;
}