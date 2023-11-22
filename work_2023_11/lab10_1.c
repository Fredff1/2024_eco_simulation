#include<stdio.h>
//Assume the first day of the year is Sunday~
typedef enum{
    False=0,
    True=1,
}Bool;

typedef enum{
    Sunday=1,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    
}Weekday;

const int DayOfMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};

Bool is_leap_year(int year){
    if((year%400==0)||(year%4==0&&year%100!=0)){
        return True;
    }
    return False;
}

int DecideDayOfMonth(int month,int year){
    if(month==2&&is_leap_year(year)==True){
        return 29;
    }
    return DayOfMonth[month-1];
}

Weekday get_day_from_month(int month,int year){
    int sumDay=0;
    if(month==1){
        return Sunday;
    }
    for(int i=0;i<month-1;i++){
        sumDay+=DayOfMonth[i];
        if(i==1){
            switch(is_leap_year(year)){
                case True:
                sumDay+=1;
                break;
                case False:
                break; 
            }
        }
        
    }
    return (Weekday)(1+sumDay%7);
}

void print_cal(int month,int year){
    Weekday startDay=get_day_from_month(month,year);
    int count=1;
    printf("Sun Mon Tue Wed Thu Fri Sat\n");
    for(int i=0;i<startDay-1;i++){
        printf("    ");
    }
    for(int i=0;i<8-startDay;i++){
        printf("  %d ",count);
        count+=1;
    }
    printf("\n");
    int flag=0;
    //printf("%d",DecideDayOfMonth(month,year));
    //printf("%d",count);
    while(count<=DecideDayOfMonth(month,year)){
        while(flag<7&&count<=DecideDayOfMonth(month,year)){
            if(count<10){
                printf("  %d ",count);
            }else{
                printf(" %d ",count);
            }
            count+=1;
            flag++;
        }
        printf("\n");
        flag=0;
    }
}

int main(int argc, char*argv[]){
    int month=6,year=1999;
    printf("Enter your year:");
    scanf("%d",&year);
    printf("Enter your month:");
    scanf("%d",&month);
    print_cal(month,year);
    return 0;
}

