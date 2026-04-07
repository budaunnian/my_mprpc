#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

//异步写日志的日志队列
template<typename T>
class LockQueue
{
public:
    //使用模板就要在类内进行实现
    void Push(const T &data)
    {
        //拿锁
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condvariable.notify_one();//条件变量通知写线程
    }
    T Pop()
    {
        //拿锁
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty())
        {
            //线程等待并且释放锁
            m_condvariable.wait(lock);
        }
        //从头开始获取日志
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;

};
