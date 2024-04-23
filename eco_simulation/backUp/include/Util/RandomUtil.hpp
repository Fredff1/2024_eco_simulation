#ifndef RANDOM_UTIL_H
#define RANDOM_UTIL_H
#include <random>
#include <mutex>

class RandomUtil {
private:
    static std::mutex mtx;  // 静态互斥锁
    static std::mt19937 generator;

public:

    static void initialize();
    // 静态方法，用于生成一个指定范围的随机整数
    static int getRandomInt(int min, int max);

    // 静态方法，用于生成一个指定范围的随机浮点数
    static double getRandomDouble(double min, double max);
};




#endif 
