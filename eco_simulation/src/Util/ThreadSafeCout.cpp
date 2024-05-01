#include "ThreadSafeCout.hpp"
#include <mutex>

std::mutex cout_mutex;

int getInt_test(){
    return 1;
}