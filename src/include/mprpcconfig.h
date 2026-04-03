#pragma once

#include <unordered_map>
#include <string>

// 閰嶇疆rpc鐨刬p鍜岀鍙ｏ紝浠ュ強zookeeper鐨刬p鍜岀鍙�
class MprpcConfig
{
public:
    //璐熻矗鍔犺浇閰嶇疆鏂囦欢
    void LoadConfigFile(const char *config_file);
    // 杩斿洖
    std::string Load(std::string key);
private:
    std::unordered_map<std::string,std::string> m_configMap;
    void Trim(std::string & src_buf);
};


