#include<stdio.h>

//年份对应的生肖和月份对应的星座是随机设置的，就当是平行世界的年份好了，星座简单记为A B C.....

int main(){
    int year,stella;
    int input_year,input_month;
    
    printf("Please enter your birthday\n");
    scanf("%d %d",&input_year,&input_month);
    printf("Your constellation is:");
    input_year=input_year%12;
    switch(input_month){
        case 1:
        printf("A\nyou are lazy\n");
        break;
        case 2:
        printf("B \nou are beautiful\n");
        break;
        case 3:
        printf("C\nyou are brave\n");
        break;
        case 4:
        printf("D\nno feature\n");
        break;
        case 5:
        printf("E\nyou are determined\n");
        break;
        case 6:
        printf("F\nyou are jealous\n");
        break;
        case 7:
        printf("G\nyou are slim\n");
        break;
        case 8:
        printf("H\nyou are ???\n");
        break;
        case 9:
        printf("I\nyou are pleasant\n");
        break;
        case 10:
        printf("J\nyou are !!!\n");
        break;
        case 11:
        printf("K\nno more...\n");
        break;
        case 12:
        printf("L\nyou are a hero\n");
        break;
        default:
        printf("Invalid input!\n");
    }
    printf("\n");
    printf("Your zodiac:");
    switch(input_year){
        case 1:
        printf("mouse\n");
        break;
        case 2:
        printf("cow\n");
        break;
        case 3:
        printf("tiger\n");
        break;
        case 4:
        printf("rabbit\n");
        break;
        case 5:
        printf("dragon\n");
        break;
        case 6:
        printf("snake\n");
        break;
        case 7:
        printf("horse\n");
        break;
        case 8:
        printf("sheep\n");
        break;
        case 9:
        printf("monkey\n");
        break;
        case 10:
        printf("hen\n");
        break;
        case 11:
        printf("dog\n");
        break;
        case 12:
        printf("pig\n");
        break;
        default:
        printf("Invalid input!\n");
    }
    printf("\n");
}