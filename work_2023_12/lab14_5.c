#include <stdio.h>
#include <stdlib.h>

const char* employee_type_list[5]={"full_time","part_time","temp"};
//const int salary_list[5]={114,514,1919};
//const int work_hour_list[5]={18,10,20};

typedef enum{
    full_time=0,
    part_time,
    temp,
}EmployeeType;

typedef struct{
    EmployeeType type;
    int salary;
    int work_hour;
}staff_data;

void print_data(staff_data data){
    int flag;
    flag=(int)data.type;
    printf("Type:%s salary:%d work_hour:%d\n",employee_type_list[flag],data.salary,data.work_hour);
}

int main(){
    staff_data new_comer_list[3]={{full_time,11451,14},{part_time,1919,8},{temp,0,20}};
    for(int i=0;i<3;i++){
        print_data(new_comer_list[i]);
    }
    return 0;
}