#pragma once  //允许在头文件中重复定义

#include "mprpcconfig.h"

//构建mprpc框架的基类
class MprpcApplication
{
public:

    static void Init(int argc,char ** argv);//初始化
    static MprpcApplication& GetInstance(); //用static可以用类名调用函数，并且返回单例对象


private:
    static MprpcConfig m_config;
    MprpcApplication(){} //私有构造，禁止别人在类外创建对象
    MprpcApplication(const MprpcApplication&) = delete;//删除拷贝构造
    MprpcApplication(MprpcApplication&&) = delete;//删除移动构造
};