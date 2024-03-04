#include <iostream>
#include <set>

int main() {
    std::pair<int,int> myPair(1,2);
    myPair=std::pair<int,int> (2,3);
    // 创建一个std::set
    std::set<int> mySet;

    // 插入元素
    mySet.insert(3);
    mySet.insert(1);
    mySet.insert(4);
    mySet.insert(2);
    mySet.insert(5);

    // 尝试插入重复的元素
    auto result = mySet.insert(3);
    if (!result.second) {
        std::cout << "元素 3 已经存在，未添加" << std::endl;
    }

    // 遍历并打印set中的所有元素
    std::cout << "Set中的元素:" << std::endl;
    for (int elem : mySet) {
        std::cout << elem << std::endl;
    }

    // 查找元素
    if (mySet.find(4) != mySet.end()) {
        std::cout << "找到了元素 4" << std::endl;
    }

    return 0;
}
