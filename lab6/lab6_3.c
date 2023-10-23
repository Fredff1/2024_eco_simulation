#include<stdio.h>

double calculate_area_max(double epsilon){
    double area=0;
    double y;
    for (double i=0;i<1;i+=epsilon){
        y=sqrt(1-i*i);
        area+=epsilon*y;
    }
    area*=4;
    return area;
}

double calculate_area_min(double epsilon){
    double area=0;
    double y;
    for(double i=0;i<1-epsilon;i+=epsilon){
        double x=i+epsilon;
        y=sqrt(1-x*x);
        area+=epsilon*y;
    }
    area*=4;
    return area;
}
int main(){
    printf("(%lf,%lf)\n",calculate_area_min(0.001),calculate_area_max(0.001));
    printf("(%lf,%lf)\n",calculate_area_min(0.0001),calculate_area_max(0.0001));
}