#include "Util/RandomUtil.hpp"


std::mutex RandomUtil::mtx;
std::mt19937 RandomUtil::generator;

void RandomUtil::initialize(){
    std::lock_guard<std::mutex> lock(mtx);
    std::random_device rd;  // 用硬件随机数生成器来种子
    generator.seed(rd());
}

int RandomUtil::getRandomInt(int min, int max) {
    std::lock_guard<std::mutex> lock(mtx);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

// 静态方法，用于生成一个指定范围的随机浮点数
double RandomUtil::getRandomDouble(double min, double max) {
    std::lock_guard<std::mutex> lock(mtx);
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

