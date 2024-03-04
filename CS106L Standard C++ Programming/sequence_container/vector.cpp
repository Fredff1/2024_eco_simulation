#include <vector>
#include <iostream>

int main(){
    std::vector<int> v1;                   // 空的 vector
    std::vector<int> v2(5, 10);            // 长度为5，每个元素初始化为10
    std::vector<int> v3 = {1, 2, 3, 4, 5}; // 初始化列表
    std::vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    std::cout << vec[0] << std::endl;    // 输出第一个元素
    std::cout << vec.at(1) << std::endl; // 输出第二个元素
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    // 或使用范围基于的 for 循环
    for (int num : vec)
    {
        std::cout << num << std::endl;
    }
    std::cout << "The vector contains " << vec.size() << " elements." << std::endl;
    vec.pop_back(); // 删除最后一个元素

    // 删除第一个元素
    vec.erase(vec.begin());
    vec.clear();

    //std::array<int> a1;
}