#pragma once

#include <unordered_map>
#include <string>

// 配置rpc的ip和端口，以及zookeeper的ip和端口
class MprpcConfig
{
public:
    //负责加载配置文件
    void LoadConfigFile(const char *config_file);
    // 返回
    std::string Load(std::string key);
private:
    std::unordered_map<std::string,std::string> m_configMap;
};


