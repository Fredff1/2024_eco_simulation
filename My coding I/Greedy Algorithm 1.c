#include <stdio.h>
#include <stdlib.h>
#include<time.h>
typedef struct {
    int startTime;
    int endTime;
    int isChosen;
    // char type;
} activity;

int compareActivities(const void *a, const void *b) {
    activity *activityA = (activity *)a;
    activity *activityB = (activity *)b;
    return activityA->endTime - activityB->endTime;
}

int main(int argc, char *argv[]) {
    activity a, b, c, d, e, f, g;
    activity list[] = {a, b, c, d, e, f, g};
    activity result[7];
    srand(time(NULL));
    for (int i = 0; i < 7; i++) {
        list[i].startTime = rand() % 5 + i;
        list[i].endTime = list[i].startTime + 1 + rand() % 5;
        list[i].isChosen = 0;
    }
    qsort(list, 7, sizeof(activity), compareActivities);
    int resultCount = 0;
    result[resultCount++] = list[0];

    for (int i = 1; i < 7; i++) {
        if (list[i].startTime >= result[resultCount - 1].endTime) {
            result[resultCount++] = list[i];
        }
    }

    // 打印所选活动
    for (int i = 0; i < resultCount; i++) {
        printf("Chosen Activity: Start Time = %d, End Time = %d\n", result[i].startTime, result[i].endTime);
    }

    return 0;
}