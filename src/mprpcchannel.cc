#include "mprpcchannel.h"

//所有通过调用rpc方法都在这个函数中进行数据序列化和网络发送
void CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller,
                    const google::protobuf::Message *request,
                    google::protobuf::Message *response,
                    google::protobuf::Closure *done)
{
    
}