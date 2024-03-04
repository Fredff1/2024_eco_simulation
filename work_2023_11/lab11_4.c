#include <stdio.h>
#include <stdbool.h>
#include <time.h>

int *createCounter(){
    int *counter=(int*)malloc(sizeof(int));
    *counter=0;
    return counter;
}

bool is_invalid_number(int num){
    if(num== 2147483647||num<0){
        return true;
    }
    return false;
}

void increment(int* counter){
    if(is_invalid_number(*counter)){
        printf("An error occurred:number overflow\n");
        *counter=0;
        return;
    }
    *counter+=1;
}



void decrement(int* counter){
    if(is_invalid_number(*counter)){
        printf("An error occurred:number overflow\n");
        *counter=0;
        return;
    }
    *counter-=1;
}



void reset(int* counter){
    *counter=0;
}

void  random_choice(int flag,int*counter){
    if(flag==0){
        decrement(counter);
    }else if(flag==1){
        reset(counter);    
    }else{
        increment(counter);
    }
}

int main() {
    srand(time(NULL));

    int* counter1 = createCounter();
    int* counter2 = createCounter();
    
    increment(counter1);
    increment(counter1);
    increment(counter2);
    
    printf("Counter1: %d\n", *counter1); // 应输出2
    printf("Counter2: %d\n", *counter2); // 应输出1

    decrement(counter1);
    printf("Counter1 after decrement: %d\n", *counter1); // 应输出1

    reset(counter1);
    printf("Counter1 after reset: %d\n\n", *counter1); // 应输出0

    for(int i=0;i<1500;i++){
        int tar=rand()%2;
        int flag=rand()%50;
        switch(tar){
            case 0:
            random_choice(flag,counter1);
            break;
            case 1:
            random_choice(flag,counter2);
            break;
        }
        printf("Counter1: %d\n", *counter1); 
        printf("Counter2: %d\n", *counter2); 
        printf("Current turn:%d\n\n",i+1);

    }
    
    // 添加更多操作以展示计数器的独立性和持久性
    free(counter1);
    free(counter2);
    return 0;
}