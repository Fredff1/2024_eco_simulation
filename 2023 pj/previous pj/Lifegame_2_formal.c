#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<conio.h>
#include <windows.h>
#include <time.h> 

int rows=0,cols=0;
int **world;
int flag_main_menu=0;
int flag_set_world=0;
//我习惯在if后空格,else if后不空格 刚开始用的是sleep函数，后来发现需要以毫秒为单位，又加入了windows.h的Sleep函数，以后不会混用
//以下是功能性函数

//初始化世界函数
int **decide_size_of_world(int rows,int cols){//用于菜单栏的输入
    int **world=(int**)malloc(rows*sizeof(int*));//world指针指向动态二维数组**world是指向指针的指针，用于决定行，后面是总大小
    if (world==NULL){       
        return NULL;//防止出错
    }
    for (int i=0;i<rows;i++){
        world[i]=(int*)malloc(cols*sizeof(int));
        if (world[i]==NULL){
        return NULL;
    }
    }
    return world;
}
//打开文件,下面的printf是debug时使用的
void read_file(char *filename){
    FILE*fp=fopen(filename,"r");
    //printf("打开成功\n");
    if (fp == NULL) {
        printf("错误\n");
        return;
    }
    if (fscanf(fp, "%d %d", &rows, &cols) != 2) {
        //printf("读取行数和列数失败\n");
        return;
    }//读取行列信息
    //printf("%d %d第一行读取成功\n",rows,cols);
    world=decide_size_of_world(rows,cols);
    

    //printf("初始化成功\n");
    // 创建world
    fscanf(fp, "\n");
    //printf("进行中");
    char scan_input;
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            scan_input=fgetc(fp);//逐个读取
            if (scan_input=='0'){
                world[i][j]=0;                
            }else if(scan_input=='1'){
                world[i][j]=1;
            }else if(scan_input=='\n'||scan_input==EOF){
                break;
            }else if(scan_input==' '){
                j-=1;//跳过空格
            }

        }fscanf(fp,"\n");//跳过换行符
        //printf("换行成功\n");
    }
    fclose(fp);
}
//书写已有的点阵图
void write_file(char *filename){
    FILE*fp=fopen(filename,"w");
    fprintf(fp, "%d %d\n", rows, cols);//第一行的数据
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            fprintf(fp,"%d ",world[i][j]);
        }
    fprintf(fp,"\n");
    }
    fclose(fp);
}

//清除内存实验函数
void free_world(int **world,int rows){
    for(int i=0;i<rows;i++){
        free(world[i]);
    }
    free(world);
}
//打印世界函数
void print_world(int **world,int rows,int cols){
printf(" ");
for(int i=1;i<=cols;i+=1){
    if (i<10){
        printf(" %d ",i);
    }else{
        printf(" %d",i);
    }
}
printf("\n");
for (int i=0;i<rows;i++){
    if (i+1<10){
        printf(" %d",i+1);
    }else{
        printf("%d",i+1);
    }
    for (int j=0;j<cols;j++){
        if (world[i][j]==0){
            printf( "□  ");
        }else if(world[i][j]==1){
            printf( "■  ");
            
        }
        }
    printf("\n");
    }    
}
//下面是计算邻居函数
int count_living_cell(int **world,int now_rows,int now_cols){//注意两者都从0开始
    //暂时不检查rows和cols是否为0，以后要加入检查world是否存在的逻辑
    int count_living=0;
    if (now_rows>0&&now_rows<rows-1&&now_cols>0&&now_cols<cols-1){//处理不在边界上的情况
        for(int i=now_rows-1;i<now_rows+2;i++){
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (world[i][j]==1){
                    count_living+=1;
                }
            }
        }
    }else {for(int i=now_rows-1;i<now_rows+2;i++){//边界处需额外小心
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (i>=0&&j>=0&&i<rows&&j<cols){
                    if (world[i][j]==1){
                        count_living+=1;
                    }
                    
                }
            }              
        }
    }
    count_living-=world[now_rows][now_cols];//自己要减掉
    return count_living;
}
//下面函数用于更新一格的细胞
void update_world(int **world){
    int temp_world[rows][cols];//储存每一轮修改，防止中途的修改影响改变
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            int count_neighbor=count_living_cell(world,i,j);
            if (count_neighbor<2||count_neighbor>3){
                temp_world[i][j]=0;
            }else if(count_neighbor==2||(count_neighbor==3&&world[i][j]==1)){
                temp_world[i][j]=world[i][j];
                
            }else if(count_neighbor==3&&world[i][j]==0){
                temp_world[i][j]=1;//按照游戏规则进行更新
            }
        }
    }
    for(int i=0;i<rows;i+=1){
        for(int j=0;j<cols;j+=1){
            world[i][j]=temp_world[i][j];//通过拷贝更新world
        }
    }
}
//清除缓冲区函数
void clear_all_input() {
    int remain;
    while ((remain = getchar()) != '\n' && remain != EOF);
}


//以下是正式游戏进程函数

//欢迎界面函数
void welcome_to_game(){
    printf("****************************************\n");
    printf("****************************************\n");
    printf("\n");
    printf("\n");
    printf("          生    命    游    戏           \n");
    printf("             祝你van的开心!              \n");
    printf("\n");
    printf("\n");
    printf("****************************************\n");
    printf("****************************************\n");
}
//help函数
void print_help(void){
    printf("可用指令如下\n[\\h]打印命令提示\n[\\p]打印当前地图\n[\\l]导入地图\n[\\s]导出地图\n[\\d]进入地图设计模式\n");
    printf("[\\q]退出地图设计模式\n[\\r]开始生命游戏\n[end]退出游戏\n");
} 

//主游戏
void main_game(){
    int flag_start_game=1;
    int auto_game;
    char game_state[10];
    char input_test[20];
    int pause_time=1000;
    int flag_auto_pause=0;
    print_world(world,rows,cols);
    printf("请选择运行模式,输入\\a进入自动模式(自动模式下默认间隔为1秒),输入\\g进入手动模式\n");
    scanf("%s",game_state);
    clear_all_input();  
    if (strcmp(game_state,"\\a")==0) {
        auto_game=1;
    }else if(strcmp(game_state,"\\g")==0){
        auto_game=0;
    }else{
        printf("无效输入，默认为自动模式");
        auto_game=1;
        Sleep(1000);
    }
    system("cls");
    print_world(world,rows,cols);
    Sleep(1000);
    system("cls");
    while(flag_start_game==1){            
        if (auto_game==1){            
            if (kbhit()){//检测键盘输入
                char pressed=getch();
                if (pressed=='\r'){//Windows系统中回车由\r\n表示，所以比较\r并读取第二个字符以免出错
                    
                    system("cls");
                    print_world(world,rows,cols);
                     printf("自动模式已暂停\n输入\\e进入手动模式,输入\\q回到菜单栏,输入\\t可修改每一世代之间的间隔时间,按回车继续自动模式\n");
                    fgets(input_test,sizeof(input_test),stdin);
                    if (strcmp(input_test,"\n")==0){
                        system("cls");
                        continue;
                    }else if(strcmp(input_test,"\\e\n")==0){
                        auto_game=0;
                        continue;
                    }else if(strcmp(input_test,"\\q\n")==0){
                        flag_start_game=0;
                        break;
                    }else if(strcmp(input_test,"\\t\n")==0){
                        printf("请输入你希望的间隔时间(以毫秒为单位)\n");                     
                        if(scanf("%d",&pause_time)==1&&pause_time>0){                       
                            printf("已修改时间为%d,请等待两秒",pause_time);
                            clear_all_input();
                            Sleep(2000);
                            system("cls");
                            continue;
                        }else{
                            printf("错误的输入,不改变时间");
                            pause_time=1000;
                            continue;
                        }
                    }
                    
                }
            }  
        
            update_world(world);
            print_world(world,rows,cols);
            printf("处于自动模式,按回车暂停以进一步改变间隔时间、退出至菜单或进入手动模式");
            printf("\n");
            Sleep(pause_time);
            system("cls");
        }else if(auto_game==0){//手动模式
            char continue_game[10];
            printf("你处于手动模式\n输入\\g更新一世代,输入\\q回到菜单栏,输入\\sw进入自动模式\n");
            scanf("%s",continue_game);
            if (strcmp(continue_game,"\\g")==0){
                system("cls");
                update_world(world);
                print_world(world,rows,cols);
                printf("\n");
                printf("请输入下一步命令\n");
            }else if(strcmp(continue_game,"\\q")==0){
                flag_start_game=0;
                printf("已回到菜单\n");
            }else if(strcmp(continue_game,"\\sw")==0){//切换模式
                auto_game=1;
                clear_all_input();
                system("cls");
                printf("进入自动模式\n");
            }else{
                system("cls");
                print_world(world,rows,cols);
                printf("无效输入!\n");
                clear_all_input();

        }


        } 
    }    
}

//整个打开文件过程
void import_file(){
    char filename[20];
    char input_4[10];
            printf("请输入你需要导入的文件名,注意后缀:");
            scanf("%s",filename);
            free_world(world,rows);
            rows=0,cols=0;           
            read_file(filename);
            system("cls");
            print_world(world,rows,cols);
            printf("文件%s打开如上\n",filename);
            printf("按回车返回菜单\n");
            while(!kbhit()){}
            getch();
            system("cls");
            
}
//保存地图
void save_file(){
    char filename[20];
        printf("请输入你需要导出的文件名(会覆盖已有的同名文件),输入\\q退出:");
        scanf("%s",filename);
        if(strcmp(filename,"\\q")!=0){
        write_file(filename);
        printf("成功保存为%s\n",filename);
        }else{
            printf("已退出，未保存\n");
        }
}
//打印现有地图
void print_current_world(){
    if (rows>0&&cols>0){
        printf("当前的地图状态如下\n");
        print_world(world,rows,cols);
    }else{
        printf("当前没有合适的地图!\n");
    }
}
//细胞图设计模式
void design_mode(char menu_command){//这里遇到清除缓冲区的问题,不清理会导致无限循环
    flag_set_world=0;
    char quit_command_1[10];
    printf("进入地图设计模式\n");
     while(flag_set_world==0)  {
            printf("请按顺序输入你想要的地图的长和宽,中间用空格隔开,不要超过60*60.输入\\q可退出该模式\n");            
            if (scanf("%d %d",&rows,&cols)==2){
                 if (rows>0&&cols>0&&rows<81&&cols<81){//判断行和列输入是否合法
                     flag_set_world=1;}
            }else if(scanf("%s",quit_command_1)==1){
                if(strcmp(quit_command_1,"\\q")==0){
                    break;
                }                
            }else {
                printf("不合法的输入\n");
        }
            if (flag_set_world==1){
            world=decide_size_of_world(rows,cols);//创造一个指定大小的数组
            for (int i=0;i<rows;i++){
                for (int j=0;j<cols;j++){
                    world[i][j]=0;
            }
            }//用于初始化数组
            system("cls");                   
            int temp_rows,temp_cols;//输入改变的行和列
            char input_1[100];//判断输入的类型
            int flag_change=0;//判断是否退出设计细胞图的循环
            int count_change=-1;//统计操作的次数，防止刚开始执行else if
            int random_generator;//用于生产随机个细胞
            while(flag_change==0){
                int leap=0;
                input_1[0] = '\0';
                fgets(input_1,100,stdin);
                char *input_1_trace=input_1;
                system("cls"); 
                if (sscanf(input_1,"%d %d",&temp_rows,&temp_cols)==2){
                    while (sscanf(input_1_trace, "%d %d%n", &temp_rows, &temp_cols,&leap) == 2) { // 当我们可以读取两个数字时且用%n读取以及读取的字符数，在下一次跳过
                        if (0 < temp_rows && temp_rows <= rows && 0 < temp_cols && temp_cols <= cols) {
                            world[temp_rows - 1][temp_cols - 1] = 1; // 将指定位置设置为活细胞
                            printf("第%d行%d列变成了活细胞!\n", temp_rows, temp_cols);
                        }else if(temp_rows<0&&temp_cols<0&&abs(temp_rows)<=rows&&abs(temp_cols)<=cols&&world[abs(temp_rows)-1][abs(temp_cols)-1]==1){
                            world[abs(temp_rows)-1][abs(temp_cols)-1]=0;
                            printf("第%d行%d列变成死细胞\n",abs(temp_rows),abs(temp_cols));

                        }else {
                            printf("坐标(%d,%d)超出范围，跳过\n", temp_rows, temp_cols);
                        }
                        input_1_trace+=leap;
                         while (*input_1_trace && (*input_1_trace == ' ')){//指针指向不是\0或空格
                             input_1_trace+=1;
                         }           
                    }                     
                    print_world(world,rows,cols);
                    count_change+=1; 
                    printf("请输入你想要的活细胞位置，用空格隔开，允许多个输入，同样用空格隔开，超出范围会提示错误:\n");
                    printf("输入-x -y (x  y是有活细胞的位置)可以删除活细胞\n");
                    printf("输入random 数字（注意空格）可以随机生成活细胞,推荐填满至少10%%的格子\n");
                    printf("如果你已输入完毕,则输入\\q\n");                   
                    continue;
                }else if(strncmp(input_1,"\\q",2)==0){//保存文档
                    printf("是否需要保存地图至文件,如果是请输入\\s,如果要返回上一个页面则输入\\b输入\n输入其它字符将不保存且退出\n");
                    char input_2[20];
                    scanf("%s",input_2);
                    if(strcmp(input_2,"\\s")==0){
                        save_file();
                    }else if (strcmp(input_2,"\\b")==0){
                        continue;
                    }else{
                    flag_set_world=1;
                    flag_change=1;
                    break; //退出设计循环  
                    }               
                }else if(sscanf(input_1,"random %d",&random_generator)==1){
                    srand(time(NULL));
                    if (random_generator<=(rows*cols)){
                        for(int i=0;i<random_generator;){
                            int random_rows=rand()%rows;
                            int random_cols=rand()%cols;
                            if (world[random_rows][random_cols]==0){
                                world[random_rows][random_cols]=1;
                                i+=1;
                            }
                        }
                    }else{
                        printf("请输入合适的数字");
                        continue;
                    }
                       
                }else if(strcmp(input_1,"\\h")==0){
                    print_help();
                }else if(sscanf(input_1,"%d %d",&temp_rows,&temp_cols)!=2||temp_rows>=rows||temp_cols>=cols){
                    if(count_change>=0){
                    printf("无效的数字!重新输入!\n");//对各种错误输入
                    }
                } 
            print_world(world,rows,cols);
            printf("请输入你想要的活细胞位置，用空格隔开，允许多个输入，同样用空格隔开，超出范围会提示错误:\n");
            printf("输入-x -y (x  y是有活细胞的位置)可以删除活细胞\n");
            printf("输入random 数字（注意空格）可以随机生成活细胞,推荐填满至少10%%的格子\n");
            printf("如果你已输入完毕,则输入\\q\n");
            count_change+=1;//防止设置地图大小时出现错误提示
            }
        } 

}
}





//正式开始
int main(){
    rows=0,cols=0;
    char menu_command[20]; 
    system("cls");  
    while(flag_main_menu==0){
        welcome_to_game();//欢迎界面
        print_help();
        printf("\n");
        printf("你现在在菜单栏\n");
        printf("输入下一步指令,然后按回车\n");
        scanf("%s",menu_command);
        if (strcmp(menu_command,"\\d")==0){
            system("cls");
            design_mode(menu_command);           
        }else if(strcmp(menu_command,"\\r")==0){//正式开始游戏
            if(rows>0&&cols>0){
                system("cls");
                main_game();
            }else{
                system("cls");
                printf("没有初始化地图\n");
                continue;
            }
        }else if(strcmp(menu_command,"\\h")==0){//打印帮助文档
            system("cls");
            
            continue;
        }else if(strcmp(menu_command,"\\l")==0){//输入文档
            import_file();
            continue;
        }else if(strcmp(menu_command,"\\s")==0){//输出并保存
            save_file();
        }else if(strcmp(menu_command,"\\p")==0){//打印当前世界
            if (rows<=0||cols<=0){
                printf("没有合适地图\n");
                system("cls");
                continue;
            }else{ 
                system("cls");          
                print_current_world(); 
                char input_3[10];
                printf("按回车返回菜单");
                while(!kbhit()){}
                getch();
                system("cls");
            }                                
        }else if(strcmp(menu_command,"end")==0){//结束游戏
            break;
        }
        system("cls");
    }
    return 0;
}
//关于行列对齐问题，似乎和电脑分辨率有关

