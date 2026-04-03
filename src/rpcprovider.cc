#include "rpcprovider.h"

//给外部可以发布rpc服务的函数接口  用基类指针接收子类对象
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

}

    //启动Rpc服务节点，开始提供远程服务服务
void RpcProvider::Run()
{

}