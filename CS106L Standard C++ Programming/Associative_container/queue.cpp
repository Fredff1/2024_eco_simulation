#include <iostream>
#include <queue>

int main() {
    std::queue<int> myQueue;

    // 向队列添加元素
    myQueue.push(1);
    myQueue.push(2);
    myQueue.push(3);

    // 访问队首元素
    std::cout << "队首元素: " << myQueue.front() << std::endl;

    // 移除元素
    myQueue.pop();
    std::cout << "移除后的队首元素: " << myQueue.front() << std::endl;

    // 检查队列是否为空
    if (!myQueue.empty()) {
        std::cout << "队列不为空" << std::endl;
    }

    return 0;
}
