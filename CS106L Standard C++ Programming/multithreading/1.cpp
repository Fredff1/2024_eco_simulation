#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

class Buffer{
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<int> buffer;
    const unsigned int maxSize;

public:
    Buffer(unsigned int size) : maxSize(size) {}

    void add(int num) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return buffer.size() < maxSize; });
        buffer.push_back(num);
        cv.notify_all();
    }

    int remove() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !buffer.empty(); });
        int back = buffer.back();
        buffer.pop_back();
        cv.notify_all();
        return back;
    }
};

class Producer {
private:
    Buffer &buffer;

public:
    Producer(Buffer &b) : buffer(b) {}

    void operator()() {
        for (int i = 0; i < 50; ++i) {
            buffer.add(i);
            std::cout << "Produced: " << i << std::endl;
        }
    }
};

class Consumer {
private:
    Buffer &buffer;

public:
    Consumer(Buffer &b) : buffer(b) {}

    void operator()() {
        for (int i = 0; i < 50; ++i) {
            int num = buffer.remove();
            std::cout << "Consumed: " << num << std::endl;
        }
    }
};



int main() {
    Buffer buffer(10);
    Producer producer(buffer);
    Consumer consumer(buffer);

    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}


