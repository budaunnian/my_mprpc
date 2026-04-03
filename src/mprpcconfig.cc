#include "mprpcconfig.h"

#include <iostream>

//负责加载配置文件
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    //读配置文件
    FILE *pf = fopen(config_file,"r");
    if(nullptr == pf)
    {
        std::cout<<config_file<<"配置文件不存在！"<<std::endl;
        exit(EXIT_FAILURE);  
    }

    //截取正确的字符串配置文件
    while (!feof(pf))
    {
        char buf[512] ={0};
        fgets(buf,512,pf);//一行一行的读
        
        //去除字符串给前后多出来的空格
        std::string read_buf(buf);
        
        Trim(read_buf);

        //判断注释直接跳过，还有换行
        if(read_buf[0]=='#' || read_buf.empty())
        {
            continue;
        }

        //解析出配置项
        int idx =read_buf.find('=');
        if(idx==-1)
        {
            continue;
        }

        std::string key;
        std::string value;
        key= read_buf.substr(0,idx);
        Trim(key);
        int endidx = read_buf.find("\n",idx);
        value= read_buf.substr(idx+1,endidx-idx-1);
        Trim(value);
        //插入表中
        m_configMap[key] = value;

    }
    
}

std::string MprpcConfig::Load(std::string key)
{
    auto it  = m_configMap.find(key);
    if(it==m_configMap.end())
    {
        return " ";
    }
    return it->second;
}

//去掉前后的空格
void MprpcConfig::Trim(std::string & src_buf)
{
     //找出空格
        int idx = src_buf.find_first_not_of(' ');
        if(idx!=-1)
        {
            //找到了不是空格的位置
            src_buf = src_buf.substr(idx,src_buf.size()-idx);
        }
        idx = src_buf.find_last_not_of(' ');
        if(idx!=-1)
        {
            src_buf= src_buf.substr(0,idx+1);//也是从头开始，截取到最后一个后+1就是要截取的长度了

        }
}
