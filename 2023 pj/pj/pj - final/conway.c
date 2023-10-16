#include "conway.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void delete_grids(Conway *c){
    int temp_rows=c->rows;
    if(c->rows!=0&&c->cols!=0&&c->normal_grids!=NULL){
        for(int i=0;i<temp_rows;i++){
            free(c->normal_grids[i]);
        }    
    free(c->normal_grids);
    c->normal_grids = NULL;
    }
    c->rows=0;
    c->cols=0;
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
//创建一个新的normal_grids,注意使用时应该在之前使用delete_grids函数
Conway* update_conway( uint16_t rows,  uint16_t cols){
    Conway* c = (Conway*)malloc(sizeof(Conway));
    if (!c){//即指针c是NULL
        return NULL;
    }
    c->rows = rows;
    c->cols = cols;
    c->normal_grids = (GridState**)malloc(rows*sizeof(GridState*));
    if (!c->normal_grids) {
        free(c);
        return NULL;
    }
    for(int i=0;i<rows;i++){
        c->normal_grids[i]=(GridState*)malloc(cols*sizeof(GridState));
        if(!c->normal_grids[i]){
            for(int j=0;j<rows;j++){
                free(c->normal_grids[j]);
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
    int change_into_living;
    if(c->probability<=0||c->probability>100){//判断特殊情况
        c->probability=50;
    }
    change_into_living=(c->rows)*(c->cols)*(c->probability)/100;//决定需要改变的个数
    for(int i=0;i<change_into_living;i+=0){
         int rand_rows=rand()%(c->rows);
         int rand_cols=rand()%(c->cols);
        if(c->normal_grids[rand_rows][rand_cols]==Dead){
        c->normal_grids[rand_rows][rand_cols]=Alive;
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
                if(temp_grid[i][j]==Alive){
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
void print_conway( Conway *c){
    if(c->rows<0||c->cols<0){
        c->rows=0;
        c->cols=0;
    }
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
    int temp_rows=c->rows;
    int temp_cols=c->cols;
    fprintf(fp,"%d,%d\n",temp_rows,temp_cols);//第一行的数据
    for (int i=0;i<c->rows;i++){
        for (int j=0;j<c->cols;j++){
            GridState write_state=get_state(c,i,j);
            fprintf(fp,"%d,",write_state);
        }
    fprintf(fp,"\n");
    }
    fclose(fp);
    return 0;
}



// 从文件读取格点
// 失败则Conway._grids = NULL
Conway* new_conway_from_file(Conway *c,const char *filename){
    Conway *new_conway=(Conway*)malloc(sizeof(Conway));
     FILE*fp=fopen(filename,"r");
     //printf("ok\n");
    if (fp == NULL) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//以下函数用于特殊模式,函数名规则:与普通模式对应并加上_special

cell get_state_special(const Conway *c, const uint16_t x, const uint16_t y){
    if(x<c->rows&&y<c->cols){
    return c->special_grids[x][y];
    }
}


//创建special_grids
Conway* new_conway_special(uint16_t rows,uint16_t cols){
    Conway* c = (Conway*)malloc(sizeof(Conway));
    if(!c){//即指针c是NULL
        return NULL;
    }
    c->rows = rows;
    c->cols = cols;
    c->special_grids = (cell**)malloc(rows*sizeof(cell*));
    if(!c->special_grids){
        free(c);
        return NULL;
    }
    for(int i=0;i<rows;i++){
        c->special_grids[i]=(cell*)malloc(cols*sizeof(cell));
        if(!c->special_grids[i]){
            for(int j=0;j<rows;j++){
                free(c->special_grids[j]);
            }
            free(c->special_grids);
            free(c);
            return NULL;
        }
    }//这里创建的是用于打印的格点   
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            c->special_grids[i][j].type=Dead;
            c->special_grids[i][j].hunger=0;
            c->special_grids[i][j].age=0;
            c->special_grids[i][j].next_motion=0;
            c->special_grids[i][j].reproduction_count=0;
        }//初始化每个细胞状态
    }
    
    c->food_grids=(food_gridstate**)malloc(rows*sizeof(food_gridstate*));
    if(!c->food_grids){
        free(c);
        return NULL;
    }
    for(int i=0;i<rows;i++){
        c->food_grids[i]=(food_gridstate*)malloc(cols*sizeof(food_gridstate));
        if(!c->food_grids[i]){
            for(int j=0;j<rows;j++){
                free(c->food_grids[j]);
            }
            free(c->food_grids);
            free(c);
            return NULL;
        }
        memset(c->food_grids[i],0,cols*sizeof(int));
    }
        
        srand(time(NULL));
        int start_food=rows*cols/2;//初始由一半的格点会生成食物
        int temp_rows,temp_cols;
        for(int i=0;i<start_food;i+=0){
            temp_rows=rand()%rows;
            temp_cols=rand()%cols;
            if(c->food_grids[temp_rows][temp_cols]==food_0){//没有食物则初始化为1个食物
                c->food_grids[temp_rows][temp_cols]=food_1;
                i+=1;
            }
        }
    
    return c;
}




void delete_special_grids(Conway *c){
    int temp_rows=c->rows;
    if(c->rows!=0&&c->cols!=0&&c->special_grids!=NULL){
        for(int i=0;i<temp_rows;i++){

            free(c->special_grids[i]);
        }    
        free(c->special_grids);
        c->special_grids=NULL;
        for(int i=0;i<temp_rows;i++){
            free(c->food_grids[i]);
        }
        free(c->food_grids);
        c->food_grids=NULL;
    }
    c->rows=0;
    c->cols=0;
}

void print_conway_special(Conway *c){
    if(c->rows<0||c->cols<0){
        c->rows=0;
        c->cols=0;
    }
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            cell u=get_state_special(c,i,j);
            if(u.type==Dead){
                printf("| ");
            }else if(u.type==living_producer){
                printf("|*");
            }else if(u.type==living_consumer){
                printf("|$");
            }else{
                printf("( ");
            }
        }
    printf("|");    
    printf("\n");   
    }
    printf("\n");
// //////打印食物，注意只是用来测试
    /*for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            int u=c->food_grids[i][j];
            printf("%d",u);
            }
        
    printf("|");    
    printf("\n");   
    }
    printf("\n");
    */
    
}

//初始化细胞状态
void initialize_cell_condition(Conway*c,int x,int y){
    c->special_grids[x][y].age=0;
    c->special_grids[x][y].hunger=0;
    c->special_grids[x][y].type=Dead;
    c->special_grids[x][y].reproduction_count=0;
    c->special_grids[x][y].next_motion=0;
}



void init_random_special(Conway *c){//用于初始化生产者和消费者
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            c->special_grids[i][j].type=0;
        }
    }
    int change_into_living;
    if(c->probability<=0||c->probability>100){//判断特殊情况
        c->probability=50;
    }
    change_into_living=(c->rows)*(c->cols)*(c->probability)/100;//决定生产者
    for(int i=0;i<change_into_living;i+=0){
         int rand_rows=rand()%(c->rows);
         int rand_cols=rand()%(c->cols);
        if(c->special_grids[rand_rows][rand_cols].type==Dead){
            initialize_cell_condition(c,rand_rows,rand_cols);
            c->special_grids[rand_rows][rand_cols].type=living_producer;
            c->special_grids[rand_rows][rand_cols].age=0;
            c->special_grids[rand_rows][rand_cols].reproduction_count=0;
            i+=1;
        }
    }
    int consumer_number=change_into_living/8;
    for(int i=0;i<consumer_number;i+=0){//决定消费者
         int rand_rows=rand()%(c->rows);
         int rand_cols=rand()%(c->cols);
        if(c->special_grids[rand_rows][rand_cols].type==living_producer){
            initialize_cell_condition(c,rand_rows,rand_cols);
            c->special_grids[rand_rows][rand_cols].type=living_consumer;
            c->special_grids[rand_rows][rand_cols].age=0;
            c->special_grids[rand_rows][rand_cols].hunger=0;
            i+=1;
        }
    }
    c->grid_feature.producer_capacity=c->rows*c->cols/2;//初始化生产者的环境容纳量
    c->grid_feature.consumer_capacity=c->grid_feature.producer_capacity/3;//消费者的环境容纳量随生产者数量变化
}


int count_special_mode_neighbors_producer(const Conway*c,const uint16_t x,const uint16_t y){
    int living_neighbor=0;
    cell temp_grid[c->rows][c->cols];
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            temp_grid[i][j]=get_state_special(c,i,j);
        }
    }
   
    for(int i=x-1;i<=x+1;i++){
        for(int j=y-1;j<=y+1;j++){
            if(i>=0&&j>=0&&i<c->rows&&j<c->cols){
                if(temp_grid[i][j].type==living_producer){
                    living_neighbor+=1;
                }
             }
        }
    }
    
    if(temp_grid[x][y].type==living_producer){
        living_neighbor-=1;
    }
    return living_neighbor;
}

//计算邻居
int count_special_mode_neighbors_consumer(const Conway*c,const uint16_t x,const uint16_t y){
    int living_neighbor=2;
    cell temp_grid[c->rows][c->cols];
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            temp_grid[i][j]=get_state_special(c,i,j);
        }
    }
   
    for(int i=x-1;i<=x+1;i++){
        for(int j=y-1;j<=y+1;j++){
            if(i>=0&&j>=0&&i<c->rows&&j<c->cols){
                if(temp_grid[i][j].type==living_consumer){
                    living_neighbor+=1;
                }
             }
        }
    }
    living_neighbor-=temp_grid[x][y].type;
    return living_neighbor;
}

//计算生产者总数    
int count_living_producer(Conway*c){
    int count=0;
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(c->special_grids[i][j].type==living_producer){
                count+=1;
            }
        }
    }
    return count;
}

int count_living_consumer(Conway *c){
    int count=0;
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(c->special_grids[i][j].type==living_consumer){
                count+=1;
            }
        }
    }
    return count;
}

void swap_cell_condition(Conway*c,int x_1,int y_1,int x_2,int y_2){//交换两个格子的细胞状态
    cell temp_cell_condition;
    temp_cell_condition.type=c->special_grids[x_1][y_1].type;
    temp_cell_condition.hunger=c->special_grids[x_1][y_1].hunger;
    temp_cell_condition.age=c->special_grids[x_1][y_1].age;
    temp_cell_condition.reproduction_count=c->special_grids[x_1][y_1].reproduction_count;
    temp_cell_condition.next_motion=c->special_grids[x_1][y_1].next_motion;

    c->special_grids[x_1][y_1].type=c->special_grids[x_2][y_2].type;
    c->special_grids[x_1][y_1].age=c->special_grids[x_2][y_2].age;
    c->special_grids[x_1][y_1].hunger=c->special_grids[x_2][y_2].hunger;
    c->special_grids[x_1][y_1].reproduction_count=c->special_grids[x_2][y_2].reproduction_count;
    c->special_grids[x_1][y_1].next_motion=c->special_grids[x_2][y_2].next_motion;

    c->special_grids[x_2][y_2].type=temp_cell_condition.type;
    c->special_grids[x_2][y_2].age=temp_cell_condition.age;
    c->special_grids[x_2][y_2].hunger=temp_cell_condition.hunger;
    c->special_grids[x_2][y_2].reproduction_count=temp_cell_condition.reproduction_count;
    c->special_grids[x_2][y_2].next_motion=temp_cell_condition.next_motion;

}


//用于得到生产着的下一步状态，注意这个函数不会判断非producer
cell get_next_state_of_producer( Conway*c, int x, int y){
    cell cell_condition=get_state_special(c,x,y);
    int count_1=count_living_producer(c);
    c->grid_feature.producer_capacity=c->rows*c->cols/5*2;
    if(cell_condition.type==living_producer){
        int neighbor_producer=count_special_mode_neighbors_producer(c,x,y);
        if(cell_condition.age>6){//一般活不了这么久
            cell_condition.type=Dead; 
        }
        if(neighbor_producer<2){//数量过少(小于容纳量三分之一时不会孤独)
            if((count_1*2)>=c->grid_feature.producer_capacity){
                cell_condition.type=Dead;
            }else{
                cell_condition.type=cell_condition.type;
            }
        }else if(neighbor_producer==2){
            cell_condition.type=cell_condition.type;
        }else if(neighbor_producer==3){
            cell_condition.type=living_producer;
            cell_condition.age=0;
            cell_condition.reproduction_count=0;
        }else if(neighbor_producer>3){//拥挤会导致死亡
            cell_condition.type=Dead;
        }else{
            cell_condition.type=None;
        }
    }
    if(cell_condition.type==Dead){
        cell_condition.age=0;
        cell_condition.hunger=0;
        cell_condition.next_motion=0;
        cell_condition.reproduction_count=0;//初始化死细胞所有的项目
    }
    return cell_condition;
}      

//用于计算移动
int count_food_nearby(Conway*c,int x,int y){
    int count=0;
    int row_length=3;
    int col_length=3;
    for(int i=(-1*row_length);i<=row_length;i++){
        for(int j=(-1*col_length);j<=col_length;j++){
            if (x+i>=0&& x+i<c->rows&&y+j>=0&&y+j<c->cols){
                count+=c->food_grids[x+i][y+j];
                if(c->special_grids[x+i][y+j].type==living_producer){
                    count+=5;
                }
            }
        }
    }
    return count;
}

//得到一个消费者下一步的移动方向
next_move consumer_default_move(Conway*c,int x,int y){
    next_move best_direction=0;
    int max_food=-1;
    int new_x,new_y;
    int new_direction[4][2]={{-1,0},{1,0},{0,-1},{0,1}}; //上1 下2 左3 右4 
    for(int i=0;i<4;i++){
        new_x=x+new_direction[i][0];
        new_y=y+new_direction[i][1];
        if(new_x>=0&&new_y>=0&&new_x<c->rows&&new_y<c->cols){
            int count=count_food_nearby(c,new_x,new_y);
            if(count>max_food){
                max_food=count;
                best_direction=i+1;
            }
        }
    }
    if(best_direction>0){
        return best_direction;
    }else{
        return 0;
    }
}
//用来得到消费者的下一步状态,注意直接改变了格点
void consumer_action(Conway*c, int x, int y){
    cell cell_condition=get_state_special(c,x,y);
    if(cell_condition.type==living_consumer){
        int neighbor_consumer=count_special_mode_neighbors_producer(c,x,y);
        if(neighbor_consumer>3){
            cell_condition.type=Dead;
            
        }else if(cell_condition.hunger>5){
            cell_condition.type=Dead;
            
        }else if(cell_condition.age>50){
            cell_condition.type=Dead;
        }
    
        if(cell_condition.type==Dead){
            initialize_cell_condition(c,x,y);
            return;//死亡的消费者当然不会进食啦
        }
    }
    if(c->special_grids[x][y].hunger==0){
        return;//不饿就不动
    }
    int count_food=0;
    for(int i=x-1;i<=x+1;i++){
        for(int j=y-1;j<=y+1;j++){
            if(i>=0&&j>=0&&i<c->rows&&j<c->cols){
                if(c->special_grids[i][j].type==Dead&&c->food_grids[i][j]>food_0){
                    swap_cell_condition(c,x,y,i,j);
                    c->special_grids[i][j].hunger-=1;
                    c->food_grids[i][j]-=1;
                    initialize_cell_condition(c,x,y);
                    count_food+=1;//如果附近有食物就去吃到食物，然后函数就停止进行
                    return;
                }else if(c->special_grids[i][j].type==living_producer&&c->food_grids[i][j]>food_0){
                    swap_cell_condition(c,x,y,i,j);
                    c->special_grids[i][j].hunger-=2;
                    c->food_grids[i][j]-=1;
                    initialize_cell_condition(c,x,y);
                    count_food+=1;//如果附近有食物就去吃到食物，然后函数就停止进行
                    return;
                }
            }
            
        }
    }
    if(count_food==0){
        for(int i=x-1;i<=x+1;i++){
            for(int j=y-1;j<=y+1;j++){
                if(i>=0&&j>=0&&i<c->rows&&j<c->cols&&i!=x&&j!=y){
                    if(c->special_grids[i][j].type==living_producer){
                        c->special_grids[i][j].hunger-=2;
                        swap_cell_condition(c,x,y,i,j);
                        initialize_cell_condition(c,x,y);
                        return;//没有食物，但如果吃到了生产者就停止
                    }
                }
            }
        }
    }else{
        return;
    }
    if(count_food==0){//附近没有生产者和食物就随机移动
        /*int rand_choice;  //这是随机移动的逻辑
        srand(time(NULL));
        rand_choice=rand()%5;
        switch(rand_choice){
            case 0:
            break;
            case 1:
            if(x>=1){
                swap_cell_condition(c,x,y,x-1,y);
                initialize_cell_condition(c,x,y);//保证边界时也有相同的概率离开边界
            }else{
                swap_cell_condition(c,x,y,x+1,y);
                initialize_cell_condition(c,x,y);
            }
            break;
            case 2:
            if(y>=1){
                swap_cell_condition(c,x,y,x,y-1);
                initialize_cell_condition(c,x,y);
            }else{
                swap_cell_condition(c,x,y,x,y+1);
                initialize_cell_condition(c,x,y);
            }
            break;
            case 3:
            if(x<c->rows-1){
                swap_cell_condition(c,x,y,x+1,y);
                initialize_cell_condition(c,x,y);
            }else{
                swap_cell_condition(c,x,y,x-1,y);
                initialize_cell_condition(c,x,y);
            }
            break;
            case 4:
            if(y<c->cols-1){
                swap_cell_condition(c,x,y,x,y+1);
                initialize_cell_condition(c,x,y);
            }else{
                swap_cell_condition(c,x,y,x,y-1);
                initialize_cell_condition(c,x,y);
            }
            break;
        }*/
        c->special_grids[x][y].next_motion=consumer_default_move(c,x,y);
    }
    return ;
}

//食物生产（生产者3*3范围）
void Generate_food(Conway*c,int x,int y){
    int generated_food=1;
    int flag=rand()%3;//决定这个回合是否生成食物
    if(flag==0){
        generated_food=0;
    }
    if(c->special_grids[x][y].type!=living_producer){
        return;
    }else{
        while(generated_food<1){
            int choice=rand()%9;
            int count=0;
            for(int i=x-1;i<=x+1;i++){
                for(int j=y-1;j<=y+1;j++){
                    if(i>=0&&j>=0&&i<c->rows&&j<c->cols){
                        if(count==choice){
                            c->food_grids[i][j]+=food_1;
                            generated_food+=1;
                            break;
                        }
                        count+=1;//边界不一定可以生产食物
                    }
                }
            }
        }
    }
}



//用于消费者的繁殖
void propagate_new_consumer(Conway*c,int x,int y,int reproduction_count_1){
    srand(time(NULL));
    int rand_choice=rand()%9;
    int count=0;
    int resource_count=0;
    if(c->special_grids[x][y].type==living_consumer&&c->special_grids[x][y].hunger<=1&&c->special_grids[x][y].age>=1&&c->special_grids[x][y].age<8){
        for(int i=x-1;i<=x+1;i++){
            for(int j=y-1;j<=y+1;j++){
                resource_count=count_food_nearby(c,i,j);
                if(i>=0&&j>=0&&i<c->rows&&j<c->cols&&(c->special_grids[i][j].type==Dead||c->special_grids[i][j].type==living_producer)&&count==rand_choice&&resource_count>=5){
                    if(c->special_grids[x][y].reproduction_count>=reproduction_count_1){
                        c->special_grids[i][j].type=living_consumer;
                        //c->special_grids[x][y].hunger+=1;//繁殖消耗饥饿值
                        c->special_grids[x][y].reproduction_count=0;
                        break;
                    }else{
                        c->special_grids[x][y].reproduction_count+=1;
                        break;
                    }
                }
                count+=1;
            }
        }
    }
}

void propagate_new_producer(Conway*c){
    srand(time(NULL));
    //int rand_choice=rand()%9;
    int count=0;
    /*if(c->special_grids[x][y].type==living_producer&&c->special_grids[x][y].age>=1&&c->special_grids[x][y].age<8){
        for(int i=x-1;i<=x+1;i++){
            for(int j=y-1;j<=y+1;j++){
                if(i>=0&&j>=0&&i<c->rows&&j<c->cols&&c->special_grids[i][j].type==Dead&&count==rand_choice){
                    if(c->special_grids[x][y].reproduction_count==1){
                        c->special_grids[i][j].type=living_producer;
                        c->special_grids[x][y].reproduction_count=0;
                        break;
                    }//else{
                        //c->special_grids[x][y].reproduction_count+=1;
                       // break;
                    //}
                }
            }
        }
    }*/  //这里是随机在附近产生
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(c->special_grids[i][j].type==living_producer){
                count+=1;
            }
        }
    }
    int rand_rows,rand_cols;
    for(int i=0;i<(count/2);i+=0){
        rand_rows=rand()%c->rows;
        rand_cols=rand()%c->cols;
        if(c->special_grids[rand_rows][rand_cols].type==Dead){
            c->special_grids[rand_rows][rand_cols].type=living_producer;
            i+=1;//生成新的生产者
        }
    }
}



//更新一个世代
counting update_conway_special(Conway*c){
    Conway *temp_conway=new_conway_special(c->rows,c->cols);
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            temp_conway->special_grids[i][j]=c->special_grids[i][j];//复制到一个temp用来储存变化
            temp_conway->food_grids[i][j]=c->food_grids[i][j];
            
        }
    }

    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(c->special_grids[i][j].type==living_producer){
                int flag=rand()%2;
                if(flag==1){
                Generate_food(temp_conway,i,j);
                }
            }
        }
    }

    temp_conway->rows=c->rows;
    temp_conway->cols=c->cols;
    temp_conway->grid_feature.consumer_capacity=c->grid_feature.consumer_capacity;
    temp_conway->grid_feature.producer_capacity=c->grid_feature.producer_capacity;
    //容纳量也需要拷贝

    int reproduction_count_1=3;
    if(count_living_consumer(c)<=c->grid_feature.consumer_capacity/2){
        reproduction_count_1=1;
    }
    //先更新生产者的情况
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(temp_conway->special_grids[i][j].type==living_producer||temp_conway->special_grids[i][j].type==Dead){
                temp_conway->special_grids[i][j]=get_next_state_of_producer(temp_conway,i,j);
                
            }
        }
    }        
    //消费者进食的逻辑
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(temp_conway->special_grids[i][j].type==living_consumer){
                consumer_action(temp_conway,i,j);
                if(temp_conway->special_grids[i][j].type==Dead){
                    initialize_cell_condition(temp_conway,i,j);
                }
            }
        }
    }
    //如果没有移动，就按照motion进行移动
    next_move decided_next_move;//上1 下2 左3 右4
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(temp_conway->special_grids[i][j].type!=living_consumer){
                continue;
            }
            decided_next_move=temp_conway->special_grids[i][j].next_motion;
            switch(decided_next_move){
                case up:
                    if(i>0){
                        swap_cell_condition(temp_conway,i,j,i-1,j);
                        //initialize_cell_condition(temp_conway,i,j);
                        temp_conway->special_grids[i-1][j].next_motion=current;
                    }else{
                        //swap_cell_condition(temp_conway,i,j,i+1,j);
                        //temp_conway->special_grids[i+1][j].next_motion=current;
                        temp_conway->special_grids[i][j].next_motion=current;
                    }
                    
                    break;
                case down:
                    if(i<c->rows-1){
                        swap_cell_condition(temp_conway,i,j,i+1,j);
                        //initialize_cell_condition(temp_conway,i,j);
                        temp_conway->special_grids[i+1][j].next_motion=current;
                    }else{
                        //swap_cell_condition(temp_conway,i,j,i-1,j);
                        //temp_conway->special_grids[i-1][j].next_motion=current;
                        temp_conway->special_grids[i][j].next_motion=current;
                    break;
                    }
                    
                case left:
                if(j>0){
                        swap_cell_condition(temp_conway,i,j,i,j-1);
                        //initialize_cell_condition(temp_conway,i,j);
                        temp_conway->special_grids[i][j-1].next_motion=current;
                    }else{
                        //swap_cell_condition(temp_conway,i,j,i,j+1);
                        //temp_conway->special_grids[i][j+1].next_motion=current;
                        temp_conway->special_grids[i][j].next_motion=current;
                    }
                   
                    break;
                case right:
                    if(j<c->cols-1){
                        swap_cell_condition(temp_conway,i,j,i,j+1);
                        //initialize_cell_condition(temp_conway,i,j);
                        temp_conway->special_grids[i][j+1].next_motion=current;
                    }else{
                        //swap_cell_condition(temp_conway,i,j,i,j-1);
                        //temp_conway->special_grids[i][j-1].next_motion=current;
                        temp_conway->special_grids[i][j].next_motion=current;
                    }
                    
                    break;
                default:
                    temp_conway->special_grids[i][j].next_motion=current;
                    break;
            }
        }
    }

    //计算数量
    int number_of_producer=count_living_producer(temp_conway);
    
    //生产者繁殖的逻辑
     //for(int i=0;i<c->rows;i++){
        //for(int j=0;j<c->cols;j++){
            //if(number_of__producer<=c->grid_feature.producer_capacity){
                propagate_new_producer(temp_conway);
           //}
        //}
     //}

    //重新计算环境容纳量(暂时取消这个功能)
    c->grid_feature.consumer_capacity=number_of_producer/3;
    //这个循环用于消费者的繁殖(为了简单起见，使用分裂繁殖)
    int number_of_consumer=count_living_consumer(temp_conway);
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(number_of_consumer<c->grid_feature.consumer_capacity*3/2){
                propagate_new_consumer(temp_conway,i,j,reproduction_count_1);
            }
        }
    }
    int consumer_number_1;
    if((consumer_number_1=count_living_consumer(c))<=c->grid_feature.consumer_capacity/2){
        int generate_number=(c->grid_feature.consumer_capacity/4);
        for(int i=0;i<generate_number;i+=0){
            int temp_row=rand()%c->rows;
            int temp_col=rand()%c->cols;
            if(temp_conway->special_grids[temp_row][temp_col].type==Dead){
                initialize_cell_condition(temp_conway,temp_row,temp_col);
                temp_conway->special_grids[temp_row][temp_col].type=living_consumer;
                i+=1;
            }
        }
        
    }

    //这个循环用于改变原来的地图状态
    for(int i=0;i<c->rows;i++){
        for(int j=0;j<c->cols;j++){
            if(temp_conway->food_grids[i][j]>5){
                temp_conway->food_grids[i][j]=5;//设置食物的上限
            }
            if(temp_conway->special_grids[i][j].type==living_consumer){
                temp_conway->special_grids[i][j].age+=1;
                temp_conway->special_grids[i][j].hunger+=1;//更新年龄和饥饿
            }else if(temp_conway->special_grids[i][j].type==living_producer){
                temp_conway->special_grids[i][j].age+=1;//更新生产者年龄
            }else if(temp_conway->special_grids[i][j].type==Dead){
                initialize_cell_condition(temp_conway,i,j);
            }
            c->special_grids[i][j]=temp_conway->special_grids[i][j];
            c->food_grids[i][j]=temp_conway->food_grids[i][j];//更改c
            
        }
    }
    //c->special_grids[1][1].type=living_consumer;
    //c->special_grids[1][1].hunger=0;
    counting count_p_c;
    count_p_c.number_producer=number_of_producer;
    count_p_c.number_consumer=number_of_consumer;
    delete_special_grids(temp_conway); 
    //printf("%d %d",consumer_number_1,c->grid_feature.consumer_capacity);
    return count_p_c;
}