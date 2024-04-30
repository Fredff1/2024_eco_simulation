#ifndef THREAD_SAFE_COUT_H
#define THREAD_SAFE_COUT_H

#include <iostream>
#include <mutex>
#include <sstream>

extern std::mutex cout_mutex;

// 定义一个线程安全的封装类
class ThreadSafeCout : public std::ostringstream {
public:
    ThreadSafeCout() = default;

    // 当对象销毁时，输出其内容
    ~ThreadSafeCout() {
        std::lock_guard<std::mutex> guard(cout_mutex);
        std::cout << this->str();
    }
};

#endif // THREAD_SAFE_COUT_H
