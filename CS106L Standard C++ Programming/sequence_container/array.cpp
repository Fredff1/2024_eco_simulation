#include <iostream>
#include <array>

int main() {
    // 创建一个包含5个整数的std::array
    std::array<int, 5> myArray = {1, 2, 3, 4, 5};

    // 访问元素
    std::cout << "第一个元素: " << myArray[0] << std::endl;

    // 修改元素
    myArray[0] = 10;
    std::cout << "修改后的第一个元素: " << myArray[0] << std::endl;

    // 使用迭代器遍历数组
    std::cout << "数组元素:";
    for (auto it = myArray.begin(); it != myArray.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << std::endl;

    // 使用范围for循环
    std::cout << "使用范围for循环:";
    for (int i : myArray) {
        std::cout << ' ' << i;
    }
    std::cout << std::endl;

    return 0;
}
