#include<stdio.h>
#include<string.h>
#include <stdlib.h>

/*问题1：如何持续进行
问题2：如何在循环同时输入指令
3如何处理编译器的问题
问题4 读取文件时第一行干扰*/

int flag_step1=0;//决定主界面是否启动，0为启动，1为关闭
int flag_set_world=0;//
int rows=0;
int cols=0;
char command_start[15];
int flag_startgame=0;
int** world;
int **decide_size_of_world(int rows,int cols){//用于菜单栏的输入
    int **world=(int**)malloc(rows*sizeof(int*));//world指针指向动态二维数组**world是指向指针的指针，用于决定行，后面是总大小
    if (world==NULL){       
        return NULL;//防止出错
    }
    for (int i=0;i<rows;i++){
        world[i]=(int*)malloc(cols*sizeof(int*));
        if (world[i]==NULL){
        return NULL;
    }
    }
    return world;
}
void free_world(int **world,int rows){//清除使用的内存
    for(int i=0;i<rows;i++){
        free(world[i]);
    }
    free(world);
}
void print_world(int **world,int rows,int cols){//打印整个世界界面
for (int i=0;i<rows;i++){
    for (int j=0;j<cols;j++){
        if(world[i][j]==0){
            printf( "□  ");
        }else if(world[i][j]==1){
            printf( "■  ");
            
        }
        }
    printf("\n");
    }    
}
//help函数
void printhelp(void){
    printf("可用指令如下");
}

//下面是正式游戏的函数
int count_living_cell(int **world,int now_rows,int now_cols){//注意两者都从0开始
    //暂时不检查rows和cols是否为0，以后要加入检查world是否存在的逻辑
    int count_living=0;
    if(now_rows>0&&now_rows<rows-1&&now_cols>0&&now_cols<cols-1){//处理不在边界上的情况
        for(int i=now_rows-1;i<now_rows+2;i++){
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (world[i][j]==1){
                    count_living+=1;
                }
            }
        }
    }else {for(int i=now_rows-1;i<now_rows+2;i++){//边界处需额外小心
            for(int j=now_cols-1;j<now_cols+2;j++){
                if(i>=0&&j>=0&&i<rows&&j<cols){
                    if(world[i][j]==1){
                        count_living+=1;
                    }
                    
                }
            }              
        }
    }
    count_living-=world[now_rows][now_cols];//自己要减掉
    return count_living;
}//下面函数用于更新一格的细胞
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
void read_file(char *filename ){//先read第一行得到rows cols，用decidesize函数得到world，再用一个函数写入数据
    FILE *fp=fopen(filename,"r");
    if (fp==NULL){
        printf("打开失败");
    }
    char ch;
    fscanf(fp, "%d %d", &rows, &cols);
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            ch=fgetc(fp);
            if(ch=='0'){
                world[i][j]=0;
            }else if(ch=='1'){
                world[i][j]=1;
            }else if(ch=='\n'||ch==EOF){
                break;
            }else{
                return;
            }
        }
        fscanf(fp, "\n");  // 跳过换行符

    }
    fclose(fp);

}

int main(){
    int **world;
    printf("welcome word\n");//程序入口
    while(flag_step1==0){
        printf("你好\n");
        scanf("%s",command_start);//输入\d等指令
        if(strcmp(command_start,"\\d")==0){//判断\d指令以决定
            flag_step1=1;
            system("cls");//清屏
            printf("进入地图设计模式\n");
            printf("请按顺序输入你想要的地图的长和宽,中间用空格隔开,不要超过80*80.\n");
            
            scanf("%d %d",&rows,&cols);
            if(rows>0&&cols>0&&rows<81&&cols<81){//判断行和列输入是否合法
                flag_set_world=1;
            }
            if(flag_set_world==1){
            world=decide_size_of_world(rows,cols);//创造一个指定大小的数组
            for (int i=0;i<rows;i++){
                for (int j=0;j<cols;j++){
                    world[i][j]=0;
            }
            }//用于初始化数组
            system("cls");                   
            int temp_rows,temp_cols;//输入改变的行和列
            char input_1[30];//判断输入的类型
            int flag_change=0;//判断是否退出设计细胞图的循环
            int count_change=-1;//统计操作的次数，防止刚开始执行else if
            while(flag_change==0){
                fgets(input_1,30,stdin);
                system("cls"); 
                printf("请输入你想要的活细胞位置，用空格隔开，超出范围会提示错误:\n");
                printf("如果你已输入完毕,则输入\\q\n");
                if (sscanf(input_1,"%d %d",&temp_rows,&temp_cols)==2&&0 <temp_rows && temp_rows <=rows && 0 < temp_cols && temp_cols <=cols){
                    world[temp_rows-1][temp_cols-1]=1;
                    printf("第%d行%d列变成活细胞!\n",temp_rows,temp_cols);
                }else if(strncmp(input_1,"\\q",2)==0){
                    flag_change=1;//注意，这里以后要添加读入和保存文件的逻辑
                    flag_step1=0; 
                    memset(command_start, 0, sizeof(command_start)); 
                }else if(sscanf(input_1,"%d %d",&temp_rows,&temp_cols)!=2||temp_rows>=rows||temp_cols>=cols||temp_cols<0||temp_rows<0){
                    if(count_change>=0){
                    printf("无效的数字!重新输入!\n");//对各种错误输入
                    }
                }
            print_world(world,rows,cols);
            count_change+=1;
            }
            }    
}else if(strcmp(command_start,"\\h")==0){
    printf("help内容\n");//if的的下一步
}else if(strcmp(command_start,"\\p")==0){
    print_world(world,rows,cols);
    //打印已有地图
}else if(strcmp(command_start,"\\r")==0){//开始游戏
    flag_startgame=1;
    int auto_game=0;
    char game_state[10];
    if (rows<1||cols<1){
        printf("警告:没有设置行和列的值,将自动设置为10X10.");
        rows=10;
        cols=10;
    }
    printf("1");
    print_world(world,rows,cols);
    scanf("%xs",&game_state);
    
    while(flag_startgame==1){
        update_world(world);
        print_world(world,rows,cols);
        printf("\n");
        sleep(1);//自动模式设计完成。控制的方法有点复杂，因此先搁置
        
    }
}else if(command_start[1]=='\\'&&command_start[2]=='l'){
    char filename[20]={0};
    
    strncpy(filename, command_start + 2, sizeof(filename) - 1); // 从command_start[2]开始复制
    filename[sizeof(filename) - 1] = '\0';
    read_file(filename);
    print_world(world,rows,cols);
    flag_step1=0; //回到开始界面
    memset(command_start, 0, sizeof(command_start));//初始化输入指令的数组
}
    

    
}
}