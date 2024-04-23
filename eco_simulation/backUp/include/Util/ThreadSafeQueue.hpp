#ifndef THREAD_SAFe_QUEUE_H
#define THREAD_SAFe_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>  // 包含对 std::unique_ptr 的支持



template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cond;

public:
    /*Push the value to the queue */
    void push(T&& value);

    /* Poll the first element from the queue 
       Similar to front() and pop()
       parameter:
       value:will receive the reference of the popped elements
    */
    bool poll(T& value);

    

    bool empty();

    int size();
};

template<typename T>
void ThreadSafeQueue<T>:: push(T&& value) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(std::move(value));  // 确保对 unique_ptr 使用 std::move
    cond.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::poll(T& value) {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty()) {
        return false;
    }
    value = std::move(queue.front());  // 使用 std::move 来转移所有权
    queue.pop();
    return true;
}

template<typename T>
bool ThreadSafeQueue<T>::empty()  {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
}

template<typename T>
int ThreadSafeQueue<T>::size() {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.size();
}










#endif