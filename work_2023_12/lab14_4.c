#include <stdio.h>
#include <stdlib.h>

union SensorData{
    int var_1;
    float var_2;
    double var_3;
};

typedef union SensorData SensorData;

typedef enum{
    integer_var=0,
    float_var,
    double_var,
}sensor_data_type;

void print_data(SensorData data,sensor_data_type type){
    switch(type){
        case integer_var:
        printf("%d\n",data.var_1);
        break;
        case float_var:
        printf("%f\n",data.var_2);
        break;
        case double_var:
        printf("%lf\n",data.var_3);
        break;
    }
}

int main(){
    int data_1=10;
    float data_2=0.1111;
    double data_3=0.2434343;
    SensorData data={data_1};
    print_data(data,integer_var);
    data.var_2=data_2;
    print_data(data,float_var);
    data.var_3=data_3;
    print_data(data,double_var);
    return 0;
}

