#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct VehicleStatus{
    unsigned int engine_status:1;
    unsigned int is_locked:1;
    unsigned int front_light_status:1;
};

void print_status(struct VehicleStatus status_1){
    if(status_1.engine_status){
        printf("Engine on!\n");
    }else{
        printf("Engine off!\n");
    }
    if(status_1.is_locked){
        printf("Door locked!\n");
    }else{
        printf("Door is not locked!\n");
    }
    if(status_1.front_light_status){
        printf("Front light on!\n");
    }else{
        printf("Front light is not on!\n");
    }
}

void change_engine_status(struct VehicleStatus* status_1){
    status_1->engine_status=1-status_1->engine_status;
}

void change_door_status(struct VehicleStatus* status_1){
    status_1->is_locked=1-status_1->is_locked;
}

void change_light_status(struct VehicleStatus* status_1){
    status_1->front_light_status=1-status_1->front_light_status;
}

int main(){
    struct VehicleStatus status_new={1,1,1};
    print_status(status_new);
    change_door_status(&status_new);
    change_engine_status(&status_new);
    change_light_status(&status_new);
    print_status(status_new);
    return 0;
}