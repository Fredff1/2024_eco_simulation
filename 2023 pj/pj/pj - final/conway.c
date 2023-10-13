#include "conway.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void delete_grids(Conway *c){
    if(c->rows!=0&&c->cols!=0&&c->normal_grids!=NULL){
        for(int i=0;i<c->rows;i++){
            free(c->normal_grids[i]);
        }    
    free(c->normal_grids);
    c->normal_grids = NULL;
    }
}


void clear_input(){
    int x;
    while((x=getchar())!='\n'&&x!=EOF){
    }
}
// 前言：涉及指针操作和内存分配，随时记得指针判空

// 构造新格点，分配内存
// 创建失败则返回NULL
// malloc()
Conway* update_conway( uint16_t rows,  uint16_t cols){
    Conway* c = (Conway*)malloc(sizeof(Conway));
    if (!c){
        return NULL;
    }
    c->rows = rows;
    c->cols = cols;
    delete_grids(c);
    c->normal_grids = (GridState**)malloc(rows*sizeof(GridState*));
    if (!c->normal_grids) {
        free(c);
        return NULL;
    }
    for(int i=0;i<rows;i++){
        c->normal_grids[i]=(GridState*)malloc(cols*sizeof(GridState));
        if(!c->normal_grids[i]){
            for(int j=0;j<rows;j++){
                free(c->normal_grids[i]);
            }
            free(c->normal_grids);
            free(c);
            return NULL;
        }
        memset(c->normal_grids[i], 0, cols * sizeof(int));
    }
    
    return c;
}



// 获取格点的当前状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
// 虽然看上去这样一个封装没有必要，毕竟可以 c->_grids[x][y]来访问
// 但是封装后会安全一点
// 越界或者遇到空指针返回GridState::None ?
// if (get_current_state(c, x, y) == GridState::None) {
//     
// }
GridState get_state(const Conway *c, const uint16_t x, const uint16_t y){
    if(x<c->rows&&y<c->cols){
    return c->normal_grids[x][y];
    }else{
        return None;
    }
    
    
}

// 随机地初始化格点
void init_random(Conway *c){
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            c->normal_grids[i][j]=0;
        }
    }
    srand(time(NULL));
    int change_into_living;
    if(c->probability<=0||c->probability>100){
        c->probability=50;
    }
    change_into_living=(c->rows)*(c->cols)*(c->probability)/100;
    for(int i=0;i<change_into_living;i+=0){
         int rand_rows=rand()%(c->rows);
         int rand_cols=rand()%(c->cols);
        if(c->normal_grids[rand_rows][rand_cols]==0){
        c->normal_grids[rand_rows][rand_cols]=1;
        i+=1;
        }
    }

}





GridState count_neighbor(const Conway *c, const uint16_t x, const uint16_t y){
    int living_neighbor=0;
    GridState temp_grid[c->rows][c->cols];
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            temp_grid[i][j]=get_state(c,i,j);
        }
    }
   
    for(int i=x-1;i<=x+1;i++){
        for(int j=y-1;j<=y+1;j++){
            if(i>=0&&j>=0&&i<c->rows&&j<c->cols){
                if(temp_grid[i][j]==1){
                    living_neighbor+=1;
                }
             }
        }
    }
    
    living_neighbor-=get_state(c,x,y);
    return living_neighbor;
}

// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state(const Conway *c, const uint16_t x, const uint16_t y){
    GridState state;
    int neighbor=0;
    if(!(x<c->rows&&y<c->cols&&x>=0&&y>=0)){
        return None;
    }
    neighbor=count_neighbor(c,x,y);
    if(neighbor<2){
        return Dead;
    }else if(neighbor==2){
        return get_state(c,x,y);
    }else if(neighbor==3){
        return Alive;
    }else if(neighbor>3){
        return Dead;
    }else{
        return None;
    }

}

//没看出这个函数有什么用，删了
/*void set_state(Conway *c, const uint16_t x, const uint16_t y){
    GridState neighbor=0;
    if(!(x<c->rows&&y<c->cols&&x>=0&&y>=0)){
        return;
    }
    GridState temp_grid[c->rows][c->cols];
    neighbor=count_neighbor(c,x,y);
    
    
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            =temp_
        }
        }
}
*/


// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void print_conway(const Conway *c){
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            GridState u=get_state(c,i,j);
            if(u==Dead){
                printf("| ");
            }else if(u==Alive){
                printf("|*");
            }
        }
    printf("|");    
    printf("\n");   
    }
    printf("\n");
}



// 将系统演化到下一世代
void next_generation(Conway *c){
    GridState temp_grid[c->rows][c->cols];
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            temp_grid[i][j]=get_next_state(c,i,j);
        }
    }
    //更新原来的格点
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            c->normal_grids[i][j]=temp_grid[i][j];
        }
    }
}

// 自定义的系统演化
void custom_next_generation(Conway *c);



// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *filename){
    FILE*fp=fopen(filename,"w");
    if (!fp) {
        return -1; // 返回错误代码
    }
    fprintf(fp, "%d %d\n", c->rows, c->cols);//第一行的数据
    for (int i=0;i<c->rows;i++){
        for (int j=0;j<c->cols;j++){
            GridState write_state=get_state(c,i,j);
            fprintf(fp,"%d,",write_state);
        }
    fprintf(fp,",");
    fprintf(fp,"\n");
    }
    fclose(fp);
    return 0;
}



// 从文件读取格点
// 失败则Conway._grids = NULL
Conway* new_conway_from_file(Conway *c,const char *filename){
    Conway *new_conway;
     FILE*fp=fopen(filename,"r");
     printf("ok\n");
    if (fp == NULL) {
        printf("错误\n");
        return NULL;
    }
    fscanf(fp,"%d,%d",&new_conway->rows,&new_conway->cols);
        
    
    //if(update_conway(c->rows,c->cols)!=NULL){}
    //delete_grids(c);
    new_conway=update_conway(new_conway->rows,new_conway->cols);
    //printf("%d %d\n",new_conway->rows,new_conway->cols);

    
    fscanf(fp, "\n");//跳过第一行
    char scan_input;
    GridState temp_grid[new_conway->rows][new_conway->cols];
    for (int i=0;i<new_conway->rows;i++){
        for (int j=0;j<new_conway->cols;j++){
            scan_input=fgetc(fp);//逐个读取
            if (scan_input=='0'){
                temp_grid[i][j]=Dead;  
                           
            }else if(scan_input=='1'){
                temp_grid[i][j]=Alive;
               
            }else if(scan_input=='\n'||scan_input==EOF){
                break;
            }else if(scan_input==','){
                j-=1;//跳过","
            }

        }
        fscanf(fp,",");//最后一个逗号
        fscanf(fp,"\n");//跳过换行符
    }
    fclose(fp);
    
    

     for(int i=0;i<new_conway->rows;i++){
        for(int j=0;j<new_conway->cols;j++){
            new_conway->normal_grids[i][j]=temp_grid[i][j];
        }
    }
    return new_conway;
}

