#include <stdio.h>

int main() {
    int a;
#ifdef _WIN32
    printf("WIN32 defined\n");
#endif

#ifdef __WINPTHREADS_VERSION
    printf("POSIX threads (winpthreads)\n");
#endif

#ifdef _POSIX_THREADS
    printf("POSIX threads\n");
#endif

    return 0;
}
