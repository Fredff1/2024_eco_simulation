#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 这和前面的区别几乎没有啊，我直接拿图书馆那个改都可以

int max_size = 50;
typedef struct {
    char name[101];
    char type[101];
    int number;
    int score;
} Athlete;




Athlete empty_athlete() {
    Athlete new_book = {"none", "none", -1, 0};
    return new_book;
}

Athlete *init_array() {
    Athlete *athleteList = (Athlete *)malloc(max_size * sizeof(Athlete));
    for (int i = 0; i < max_size; i++) {
        athleteList[i] = empty_athlete();
    }
    return athleteList;
}

void extend_athleteList(Athlete *athleteList) {
    athleteList = (Athlete *)(athleteList, sizeof(Athlete) * (max_size + 1));
    max_size += 1;
}

void rec_athleteList(Athlete *athleteList) {
    athleteList = (Athlete *)realloc(athleteList, sizeof(Athlete) * (max_size - 1));
    max_size -= 1;
}

void add_athlete(Athlete *athleteList, const char *new_name, const char *new_type, int new_number, int new_score) {
    for (int i = 0; i < max_size; i++) {
        if (athleteList[i].number == -1) {
            strcpy(athleteList[i].name, new_name);
            strcpy(athleteList[i].type, new_type);
            if (new_number < 0 || new_number > 10000) {
                new_number = -1;
            }
            athleteList[i].number = new_number;
            athleteList[i].score = new_score;
            return;
        }
    }
    extend_athleteList(athleteList);
    strcpy(athleteList[max_size - 1].name, new_name);
    strcpy(athleteList[max_size - 1].type, new_type);
    athleteList[max_size - 1].number = new_number;
}

void delete_athlete(Athlete *athleteList, const int target_number) {
    bool flag = false;
    for (int i = 0; i < max_size; i++) {
        if (athleteList[i].number == target_number) {
            athleteList[i] = empty_athlete();
            flag = true;
        }
    }
    if (flag == false) {
        printf("target not found\n");
    }
    for (int i = max_size - 1; i >= 0; i--) {
        if (athleteList[i].number == -1) {
            rec_athleteList(athleteList);
        }
    }
}

void print_menu() {
    printf("Welcome\n");
    printf("0:add athlete 1:delete athlete 2:print current athleteList 3:update score 4:quit\n");
}

void print_all_athlete(Athlete *athleteList) {
    printf("Current athletes:\n");
    for (int i = 0; i < max_size; i++) {
        if (athleteList[i].number == -1) {
            continue;
        }
        printf("Name:%s Sport:%s number:%d Score:%d\n", athleteList[i].name, athleteList[i].type, athleteList[i].number,athleteList[i].score);
    }
}

void read_input(char *target) {
    char s;
    int pos = 0;
    getchar();
    while ((s = getchar()) != ' ') {
        target[pos] = s;
        pos++;
        if (pos > 98) {
            break;
        }
    }
    target[pos] = '\0';
}

void update_score(Athlete *athleteList, int num, int new_score) {
    bool flag = false;
    for (int i = 0; i < max_size; i++) {
        if (athleteList[i].number == num) {
            flag = true;
            athleteList[i].score = new_score;
        }
    }
    if (flag == false) {
        printf("The athlete does not exist\n");
    }
}

void main_menu(Athlete *athleteList) {
    int cmd = 0, temp_number = 0, temp_score = 0;
    char temp_type[100], temp_name[100];
    bool flag_running = true;
    while (flag_running) {
        temp_number = 0;
        temp_score = 0;
        memset(temp_name, '\0', sizeof(temp_name));
        memset(temp_type, '\0', sizeof(temp_type));
        print_menu();
        scanf("%d", &cmd);
        switch (cmd) {
        case 0:
            printf("Enter the athlete name,sport type,number,score to add\n");
            read_input(temp_name);
            read_input(temp_type);
            if (scanf("%d %d", &temp_number, &temp_score) != 2) {
                printf("Invalid input\n");
                temp_number = -1;
                while (getchar() != '\n') {}
            }
            add_athlete(athleteList, temp_name, temp_type, temp_number, temp_score);
            break;
        case 1:
            printf("Enter the number of the athlete you want to delete\n");
            scanf("%d", &temp_number);
            delete_athlete(athleteList, temp_number);
            break;
        case 2:
            print_all_athlete(athleteList);
            break;
        case 4:
            flag_running = false;
            break;
        case 3:
            printf("Enter the number of the athlete and the new score\n");
            if (scanf("%d %d", &temp_number, &temp_score) != 2) {
                printf("Invalid input\n");
                temp_number = -1;
                while (getchar() != '\n') {}
            }
            update_score(athleteList, temp_number, temp_score);
            break;
        default:
            printf("Invalid command\n");
        }
    }
}

int main() {
    Athlete *athleteList = init_array();
    add_athlete(athleteList, "beast_114514", "sleep", 1919, 810);
    print_all_athlete(athleteList);
    main_menu(athleteList);
    free(athleteList);
    return 0;
}