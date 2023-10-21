#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function(void *ptr) {
    char *message;
    message = (char *) ptr;
    printf("%s\n", message);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    const char *message1 = "Hello from thread 1!";
    const char *message2 = "Hello from thread 2!";

    // 创建线程1
    if (pthread_create(&thread1, NULL, print_message_function, (void *) message1)) {
        fprintf(stderr, "Error - pthread_create() for thread 1\n");
        exit(EXIT_FAILURE);
    }

    // 创建线程2
    if (pthread_create(&thread2, NULL, print_message_function, (void *) message2)) {
        fprintf(stderr, "Error - pthread_create() for thread 2\n");
        exit(EXIT_FAILURE);
    }

    // 等待线程1完成
    pthread_join(thread1, NULL);
    // 等待线程2完成
    pthread_join(thread2, NULL);

    exit(EXIT_SUCCESS);
}
