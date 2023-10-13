#include <stdio.h>
#include <pthread.h>
#include "conway.h"
#include <unistd.h>


void clear_screen() {
    // Use ANSI escape codes to clear the screen
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
    //system("cls");
}

void print_help() {

}

void print_game(const Conway* c) {

}

void* listen_break(void* flag) {
    char c = 0;
    while (c != 'b'&&c!='t') {
        scanf(" %c", &c);
        scanf("%*[^\n]"); // 清除未读内容
    }

    *(int*)flag = 1;
    return NULL;
}

void automatic_evolve(Conway* c,int time) {
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break, &flag);
    while (flag != 1) {
        next_generation(c);
        clear_screen();
        print_game(c);
        printf("automatically evolving...\n");
        print_conway(c);
        usleep(time*1000); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    print_game(c);
}

int main() {
    Conway* c = update_conway(0, 0);
    int running=1;
    c->probability=50;
    char menu_command[50];
    char basic_command;
    char filename_command[20];
    int flag_input_or_not=0;
    char filename[50];
    while(1){
        clear_screen();
        print_game(c);
        print_conway(c);
        printf("%d %d\n",c->rows,c->cols);
        printf("%d\n",c->probability);
        fgets(menu_command,50,stdin);
        sscanf(menu_command,"%c",&basic_command);
        if(basic_command=='i'){ //i rows cols 情况
            sscanf(menu_command,"%c %d %d",&basic_command,&c->rows,&c->cols);
            if(c->rows<=0||c->cols<=0){
                continue;
            }
            c=update_conway(c->rows,c->cols);
            c->probability=50;        
        }else if(basic_command=='n'){//解析单个字母命令
            next_generation(c); 
        }else if(basic_command=='l'){
            sscanf(menu_command,"%c %48s",&basic_command,&filename);
            c=new_conway_from_file(c,filename);                      
        }else if(basic_command=='s'){
            sscanf(menu_command,"%c %49s",&basic_command,&filename);
            save_conway(c,filename);
        }else if(basic_command=='c'){//解析读入 读出命令
            int time=1000;
            //while (running) {
            char cmd='c';
            char path[40];           
            if(flag_input_or_not>0){
            scanf(" %c", &cmd);   
            }                
            switch (cmd)
            {
            case 'c':
                automatic_evolve(c,time);
                flag_input_or_not+=1;
                break;
            /*others
            case 't':
                flag_input_or_not=0;
                scanf("%d",&time);
                */
            default:
                flag_input_or_not=0;
                //break;
            }

        //}
        }else if(basic_command=='r'){
            if(sscanf(menu_command,"%c %d",&basic_command,&c->probability)==2){
                init_random(c);
            }else{
                continue;
            }
        }else if(basic_command=='q'){ 
            break;  
        }else{//解析错误输入
            clear_input();
        }
        

}
    delete_grids(c);
    clear_screen();
    return 0;
}