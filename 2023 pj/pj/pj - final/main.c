#include <stdio.h>
#include <pthread.h>
#include "conway.h"
#include <unistd.h>


void clear_screen() {
    // Use ANSI escape codes to clear the screen
    //system("cls");
    printf("\e[1;1H\e[2J");
    printf("\e[?25l");
    
}

void print_help() {

}

void print_game(const Conway* c) {

}

void* listen_break(void* flag) {
    char c = 0;
    while (c != 'b'&&c!='t') {
        scanf(" %c", &c);
        scanf("%*[^\n]");// 清除未读内容
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
        print_conway(c);
        printf("automatically evolving...\n");
        usleep(time*1000); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    clear_screen();
}


void* listen_break_special(void* flag) {
    char c = 0;
    while (c != 'b'&&c!='t') {
        scanf(" %c", &c);
        scanf("%*[^\n]");// 清除未读内容
    }

    *(int*)flag = 1;
    return NULL;
}

void automatic_evolve_special(Conway* c,int time) {
    int flag = 0;
    pthread_t listener;
    pthread_create(&listener, NULL, listen_break_special, &flag);
    while (flag != 1) {
        counting count_p_c=update_conway_special(c);
        clear_screen();
        print_game_conway_help_special(c);
        printf("Current producer:%d Current consumer:%d \n",count_p_c.number_producer,count_p_c.number_consumer);
        printf("automatically evolving...\n");
        usleep(time*1000); // 每秒演化一次
    }

    pthread_join(listener, NULL);
    clear_screen();
}

void print_game_conway_help(Conway *c){//集成打印游戏界面
    print_game(c);
    print_conway(c);
    print_help();   
}

void print_game_conway_help_special(Conway*c){  
    print_game(c);
    print_conway_special(c);
    print_help();
}



int main() {
    Conway* c = update_conway(0, 0);
    c->probability=50;
    srand(time(NULL));
    int time=1000;
    int flag_input_or_not=0;//多线程
    int flag_mode=Normal;
    char menu_command[100];//输入的指令
    char basic_command;//解析基础指令如b l c
    char filename[50];//获得打开/保存的文件名
    clear_screen();
    print_game(c);
    print_conway(c);
    print_help();
    while(1){//主循环
        if(flag_mode==Normal){
            printf("Current game mode:Normal Mode\nPlease enter your options!\n");
            fgets(menu_command,100,stdin);
            clear_screen();
            sscanf(menu_command,"%c",&basic_command);
            if(basic_command=='i'){ //i rows cols 情况
                sscanf(menu_command,"%c %d %d",&basic_command,&c->rows,&c->cols);
                if(c->rows<=0||c->cols<=0||c->rows>300||c->cols>300){//输入负数会导致变为非常大的整数
                    c->rows=0;
                    c->cols=0;
                    delete_grids(c);
                    c=update_conway(0,0);
                    print_game_conway_help(c);
                    printf("warning!Invalid input detected!Please try again\n");//错误时的返回值
                    continue;
                }
                c=update_conway(c->rows,c->cols);//初始化     
            }else if(basic_command=='n'){//手动下一世代
                next_generation(c);
                print_game_conway_help(c);
                printf("Grids updated!\n");
                continue; 
            }else if(basic_command=='l'){//导入文件
                sscanf(menu_command,"%c %48s",&basic_command,&filename);//读取文件名
                c=new_conway_from_file(c,filename);  
                
            }else if(basic_command=='s'){//保存文件
                sscanf(menu_command,"%c %49s",&basic_command,&filename);//读取文件名
                save_conway(c,filename);
                printf("Grids saved as %s\n",filename);
                print_game_conway_help(c);
                continue;
            }else if(basic_command=='c'){//自动演化模式
                if(c->rows<=0||c->cols<=0){
                    c->rows=0;
                    c->cols=0;
                    print_game_conway_help(c);
                    printf("Invalid rows/cols detected!Failed to activate auto mode!\n");
                    continue;
                }        
                //while (running) {
                char cmd='c';
                char path[40];           
                //if(flag_input_or_not>0){
                    //scanf(" %c", &cmd); 
                    //clear_input();  
                //}                
                switch (cmd){
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
                    clear_screen();
                    print_game_conway_help(c);
                    printf("auto mode disabled!\n");
                    sleep(2);
                // continue;
                    break;
                }
            //}
            }else if(basic_command=='r'){//随机初始化
                int temp_flag_random_set_living=sscanf(menu_command,"%c %d",&basic_command,&c->probability);
                if(temp_flag_random_set_living==2&&c->probability>0&&c->probability<100){
                    init_random(c);
                    print_game_conway_help(c);
                    printf("Successfully randomly set grids!\n");
                    continue;
                }else{
                    c->probability=50;
                    init_random(c);
                    print_game_conway_help(c);
                    printf("Warning:An error occurred when trying to set grids.The grids have been set default!\n");
                    continue;
                }
            }else if(basic_command=='q'){ //退出游戏
                break;  
            }else if(basic_command=='t'){//改变时间
                print_game_conway_help(c);
                printf("Please enter the pause time you want (Example:100ms)\n");
                int temp_flag;
                temp_flag=scanf("%d",&time);
                clear_input();
                if(temp_flag!=1){
                    printf("Invalid time input!\nTime remain 1000ms\n");
                    time=1000;
                    continue;
                }
                continue;
            }else if(basic_command=='h'){//帮助
                print_help();
            }else if(basic_command=='p'){//改变游戏模式
                //print_game_conway_help(c);
                printf("Change game mode to Special mode!\n");
                flag_mode=Special;
                delete_grids(c);
                c->rows=0;
                c->cols=0;
                continue;
            }else{//解析错误输入
                //clear_input(); 
                print_game_conway_help(c);
                printf("Invalid input!\n");  
                continue;        
            }
            print_game_conway_help(c);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }else if(flag_mode==Special){//到特殊模式
            printf("Current game mode:Special Mode\nPlease enter your options!\n");
            fgets(menu_command,100,stdin);
            clear_screen();
            sscanf(menu_command,"%c",&basic_command);
            if(basic_command=='i'){ //i rows cols 情况
                sscanf(menu_command,"%c %d %d",&basic_command,&c->rows,&c->cols);
                if(c->rows<=0||c->cols<=0||c->rows>300||c->cols>300){//输入负数会导致变为非常大的整数
                    c->rows=0;
                    c->cols=0;
                    delete_special_grids(c);
                    c=new_conway_special(0,0);
                    print_game_conway_help_special(c);
                    printf("Warning!Invalid input detected!Please try again\n\n");//错误时的返回值
                    continue;
                }
                c=new_conway_special(c->rows,c->cols);
                print_game_conway_help_special(c);
                continue;
            }else if(basic_command=='r'){//随机生成
                int temp_flag_random_set_living=sscanf(menu_command,"%c %d",&basic_command,&c->probability);
                if(temp_flag_random_set_living==2&&c->probability>0&&c->probability<100){
                    init_random_special(c);
                    print_game_conway_help_special(c);
                    printf("Successfully randomly set grids!\n");
                    continue;
                }else{//一样的错误处理
                    c->probability=50;
                    init_random_special(c);
                    print_game_conway_help_special(c);
                    printf("Warning:An error occurred when trying to set grids.The grids have been set default!\n");
                    continue;
                }
            }else if(basic_command=='p'){
                printf("Change game mode to Normal mode!\n");
                flag_mode=Normal;
                delete_special_grids(c);//切换模式时会删除已有的格点
                c->rows=0;
                c->cols=0;
                continue;
            }else if(basic_command=='n'){
                counting count_p_c=update_conway_special(c);
                print_game_conway_help_special(c);
                printf("Current producer:%d Current consumer:%d \n",count_p_c.number_producer,count_p_c.number_consumer);
                continue;
            }else if(basic_command=='c'){//自动演化模式
                if(c->rows<=0||c->cols<=0){
                    c->rows=0;
                    c->cols=0;
                    print_game_conway_help_special(c);
                    printf("Invalid rows/cols detected!Failed to activate auto mode!\n");
                    continue;
                }        
                char cmd='c';
                char path[40];           
                switch (cmd){
                case 'c':
                    automatic_evolve_special(c,time);
                    flag_input_or_not+=1;
                    break;
                
                default:
                    flag_input_or_not=0;
                    clear_screen();
                    print_game_conway_help_special(c);
                    printf("auto mode disabled!\n");
                    sleep(2);
                    break;
                }
            }    
        print_game_conway_help_special(c);
        }
    }
    delete_grids(c);
    clear_screen();
    return 0;
}