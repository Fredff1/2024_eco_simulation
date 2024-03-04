#include <iostream>
#include <map>
#include <string>

int main() {
    // 创建一个std::map，键为std::string，值为int
    std::map<std::string, int> myMap;

    // 插入键值对
    myMap["apple"] = 5;
    myMap["banana"] = 8;
    myMap["cherry"] = 3;

    // 遍历并打印map中的所有元素
    std::cout << "Map中的元素:" << std::endl;
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // 访问特定键的值
    std::cout << "banana 的数量: " << myMap["banana"] << std::endl;

    // 查找元素
    if (myMap.find("apple") != myMap.end()) {
        std::cout << "找到了 apple" << std::endl;
    }

    return 0;
}
