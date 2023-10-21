#include<stdio.h>
#include<string.h>
#include <stdlib.h>


int flag_step1=0;//决定主界面是否启动，0为启动，1为关闭
int flag_set_world=0;//
int rows=0;
int cols=0;
char command_start[15];
int flag_startgame=0;

int **decide_size_of_world(int rows,int cols){//用于菜单栏的输入
    int **world=(int**)malloc(rows*sizeof(int*));//world指针指向动态二维数组**world是指向指针的指针，用于决定行，后面是总大小
    if (world==NULL){       
        return NULL;//防止出错
    }
    for (int i=0;i<rows;i++){
        world[i]=(int*)malloc(cols*sizeof(int*));
        if (world==NULL){
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
//下面是正式游戏的函数
int count_living_cell(int **world,int now_rows,int now_cols){//注意两者都从0开始
    //暂时不检查rows和cols是否为0，以后要加入检查world是否存在的逻辑
    int count_living=0;
    if(now_rows!=0&&now_rows!=rows-1&&now_cols!=0&&now_cols!=cols-1){
        for(int i=now_rows-1;i<now_rows+2;i++){
            for(int j=now_cols-1;j<now_cols+2;j++){
                if (world[i][j]==1){
                    count_living+=1;
                }
            }
        }
    }else {for(int i=now_rows-1;i<now_rows+2;i++){//边界处需额外小心
            for(int j=now_cols-1;j<now_cols+2;j++){
                if(i>=0&&j>=0){
                    if(world[i][j]==1){
                        count_living+=1;
                    }
                    break;
                }
            }              
        }
    }
    count_living-=world[now_rows][now_cols];
    return count_living;
}


int main(){
    int **world;
    printf("welcome word\n");//程序入口
    while(flag_step1==0){
        printf("你好\n");
        scanf("%s",command_start);//输入\d指令
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
            int count_change=-1;//统计操作的次数，防止刚开始执行elseif
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
                }else if((input_1,"%d %d",&temp_rows,&temp_cols)!=2||temp_rows>=rows||temp_cols>=cols||temp_cols<0||temp_rows<0){
                    if(count_change>=0){
                    printf("无效的数字!重新输入!\n");
                    }
                }
            print_world(world,rows,cols);
            count_change+=1;
            }
            }    
}else if(strcmp(command_start,"\\h")==0){
    printf("help内容\n");//if的的下一步
}else if(strcmp(command_start,"\\p")==0){
    //打印已有地图
}else if(strcmp(command_start,"\\r")==0){//开始游戏
    flag_startgame=1;
    printf("1");
    print_world(world,rows,cols);
    while(flag_startgame==1){
        printf("输入下一步指令");
    }
}
    

    
}
}