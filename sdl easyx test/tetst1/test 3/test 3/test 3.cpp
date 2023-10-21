// test 3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

void* print_message_function(void* ptr) {
    char* message;
    message = (char*)ptr;
    printf("%s\n", message);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    const char* message1 = "Hello from thread 1!";
    const char* message2 = "Hello from thread 2!";

    // 创建线程1
    if (pthread_create(&thread1, NULL, print_message_function, (void*)message1)) {
        fprintf(stderr, "Error - pthread_create() for thread 1\n");
        exit(EXIT_FAILURE);
    }

    // 创建线程2
    if (pthread_create(&thread2, NULL, print_message_function, (void*)message2)) {
        fprintf(stderr, "Error - pthread_create() for thread 2\n");
        exit(EXIT_FAILURE);
    }

    // 等待线程1完成
    pthread_join(thread1, NULL);
    // 等待线程2完成
    pthread_join(thread2, NULL);

    exit(EXIT_SUCCESS);
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
