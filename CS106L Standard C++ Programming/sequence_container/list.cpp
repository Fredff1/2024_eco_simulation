#include <iostream>
#include <list>

int main() {
    // 创建一个std::list
    std::list<int> myList = {1, 2, 3, 4, 5};

    // 向list中添加元素
    myList.push_back(6); // 在末尾添加
    myList.push_front(0); // 在开头添加

    // 遍历list并打印元素
    std::cout << "List的元素:";
    for (int elem : myList) {
        std::cout << ' ' << elem;
    }
    std::cout << std::endl;

    // 删除元素
    myList.erase(++myList.begin()); // 删除第二个元素

    // 再次遍历list
    std::cout << "删除元素后的List:";
    for (int elem : myList) {
        std::cout << ' ' << elem;
    }
    std::cout << std::endl;

    // 反向遍历
    std::cout << "List的反向元素:";
    for (auto it = myList.rbegin(); it != myList.rend(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << std::endl;

    return 0;
}
