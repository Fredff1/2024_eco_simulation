#include <stdio.h>
#include <string.h>

#define MAX_SIZE = 30


typedef enum {
    False = 0,
    True = 1,
} Bool;

//有些错误处理实在懒得搞了，只做了基本的错误处理


char password[5][30] = {{}, {}, {}, {}, {}};
char username[5][30] = {{}, {}, {}, {}, {}};
char input_username[32];
char input_password[32];
char input_1;
char input_2[100];
int Index = 0;
int count;

Bool decide_username() {
    int flag = 0;
    while (flag < 5) {
        printf("Please enter your username\n");
        scanf("%s", &input_username);
        sscanf(input_username, input_username);
        for (int i = 0; i < 5; i++) {
            if (strcmp(input_username, username[i]) == 0) {
                Index = i;
                return True;
            }
        }
        printf("The username does not exist!\n");
        flag++;
    }
    printf("You have failed too many times!\n");
    return False;
}

void decide_password() {
    int flag = 0;
    while (flag < 5) {
        printf("Please enter your password(max 30)\n");
        scanf("%s", input_password);
        if (strcmp(password[Index], input_password) == 0) {
            printf("Welcome back user:%s\n", username[Index]);
            return;
        } else {
            printf("Password wrong\n");
        }
        flag++;
    }
    printf("You have failed too many times!\n");
    return;
}

Bool createAccount() {
    printf("Please enter the username you want to create(max 30)\n");
    scanf("%s", input_username);
    if (input_username[30] != '\0') {
        printf("Too Long\n");
        return;
    }
    for (int i = 0; i < 5; i++) {
        if (strcmp(username[i], input_username) == 0) {
            // printf("%s",username[i]);
            printf("The username already exist\n");
            return False;
        }
    }
    for (int j = 0; j < 5; j++) {
        if (strcmp(username[j], "") == 0) {
            strcpy(username[j], input_username);
            printf("Enter your password\n");
            scanf("%s", input_username);
            if (input_username[30] != '\0') {
                printf("Too Long\n");
                return;
            }
            strcpy(password[j], input_username);
            printf("Your account is ready!\n");
            return True;
        }
    }
    return False;
}

int main() {
    Bool flag_main = True;
    while (flag_main) {
        for (int i = 0; i < 5; i++) {
            if (strcmp(username[i], "") != 0) {
                count++;
            }
        }
        if(count>=5){
            printf("You cannot create new account\n");
        }
        printf("Enter l to log in,or enter s to create a new account,enter q to quit\n");
        scanf(" %c", &input_1);
        switch (input_1) {
        case 'l': {
            Bool flag_continue = decide_username();
            if (flag_continue) {
                decide_password();
            }
            break;
        }
        case 's': {
            Bool flag = False;
            while (!flag) {
                flag = createAccount();
            }
            break;
        }
        case 'q': {
            flag_main = False;
            break;
        }
        default: {
            printf("Invalid input1\n");
            break;
        }
        }
    }
}