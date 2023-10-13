#include <stdint.h>
#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include "conway.h"

#include <pthread.h>

// 线程函数
void* myThread(void* arg) {
    printf("Hello from new thread!\n");
    return NULL;
}

int main() {
    pthread_t thread_id;

    printf("Before thread creation.\n");

    // 创建新线程
    if(pthread_create(&thread_id, NULL, myThread, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // 等待线程结束
    if(pthread_join(thread_id, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    printf("After thread has finished.\n");

    return 0;
}
