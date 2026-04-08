#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

// 全局Watcher回调（必须全局，适配你的代码风格）
void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT) // 会话事件（连接/断开）
    {
        if (state == ZOO_CONNECTED_STATE) // 连接成功
        {
            // 唤醒阻塞的Start方法，通知连接完成
            sem_t *sem = (sem_t *)zoo_get_context(zh);
            sem_post(sem);
            std::cout << "Zookeeper connection established successfully!" << std::endl;
        }
    }
}

// 构造函数：初始化句柄
ZkClient::ZkClient() : m_zhandle(nullptr)
{
}

// 析构函数：关闭连接
ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle); // 关闭句柄，释放资源
        m_zhandle = nullptr;
    }
}

// ===================== Start() 实现=====================
void ZkClient::Start()
{
    // 从MprpcApplication的配置文件中读取ZK地址和端口
    std::string host = MprpcApplication::GetInstance().Getconfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().Getconfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    /*
    zookeeper_mt：多线程版本
    zookeeper的API客户端程序提供了三个线程
    API调用线程
    网络I/O线程  pthread_create  poll
    watcher回调线程 pthread_create
    */
    m_zhandle = zookeeper_init(
        connstr.c_str(),  // 连接串：ip:port
        global_watcher,  // 全局Watcher回调
        30000,           // 会话超时时间：30000ms
        nullptr,         // 客户端ID（nullptr表示自动分配）
        nullptr,         // 上下文数据（后续通过zoo_set_context设置）
        0                // 日志级别
    );

    if (nullptr == m_zhandle)
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 信号量：阻塞等待连接成功
    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem); // 把信号量绑定到ZK句柄

    sem_wait(&sem); // 阻塞，直到Watcher回调中sem_post唤醒
    sem_destroy(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}

// ===================== Create() 实现 =====================
void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;

    // 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (ZNONODE == flag) // 表示path的znode节点不存在
    {
        // 创建指定path的znode节点了
        flag = zoo_create(
            m_zhandle,
            path,
            data,
            datalen,
            &ZOO_OPEN_ACL_UNSAFE, // 开放权限
            state,                // 节点类型：0永久/1临时
            path_buffer,
            bufferlen
        );

        if (flag == ZOK)
        {
            std::cout << "znode create success... path:" << path << std::endl;
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// ===================== GetData() 实现 =====================
std::string ZkClient::GetData(const char *path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);

    // 调用zoo_get获取节点数据，不注册Watcher
    int flag = zoo_get(
        m_zhandle,
        path,
        0,          // 不注册Watcher
        buffer,
        &bufferlen,
        nullptr
    );

    if (flag != ZOK)
    {
        std::cout << "get znode error... path:" << path << std::endl;
        return "";
    }
    else
    {
        // 直接返回buffer中的数据（按实际长度构造string）
        return std::string(buffer, bufferlen);
    }
}