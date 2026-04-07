#pragma once 
#include <google/protobuf/service.h>
#include <string>

class MprpcController : public google::protobuf::RpcController
{
public:
    MprpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string &reason);


    //一下不需要实现具体的方法
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure*callback);
private:
    bool m_failed;//记录rpc执行过程中的状态
    std::string m_errText;//RPC方法执行过程中的错误信息
};