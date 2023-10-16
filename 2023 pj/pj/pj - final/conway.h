#ifndef CONWAY_H
#define CONWAY_H

#include <stdint.h>
#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include<conio.h>

#define STATE_DEAD 0
#define STATE_ALIVE 1

// 用作get越界时的返回值
#define STATE_NONE -1

// 其它状态，可扩展
#define STATE_OTHER 2
  
typedef enum{
    Normal=1,
    Special=2,
}Gamemode;

typedef enum {
    Dead=0,
    Alive=1,
    living_producer=2,
    living_consumer=3,
    barrier=4,        
    None=-1,//越界返回值
 }GridState; 

typedef enum{
    food_0=0,
    food_1=1,
    food_2=2,
    food_3=3,
    food_4=4,
    food_5=5,
}food_gridstate;

typedef enum{//上1 下2 左3 右4 
    current=0,
    up=1,
    down=2,
    left=3,
    right=4,
}next_move;
// 康威生命游戏(Conway's Game of Life)
// 在一个二维方格中，每个格点有生存或者死亡的状态
// 每个格点在下一个世代的生死状态由周围8个格点的状态决定（硬边界 or 周期性边界条件）
// 基本演化规则如下：
// 1. 任何活格点，若邻居活格点数 < 2，则该格点在下一世代死亡
// 2. 任何活格点，若邻居活格点数 = 2 或 = 3，则该格点在下一世代存活
// 3. 任何活格点，若邻居活格点数 > 2，则该格点在下一世代死亡
// 4. 任何空格点，若邻居活格点数 = 3，则该格点在下一世代变为活格点


typedef struct {
    GridState type;
    uint16_t hunger;
    uint16_t age;
    uint16_t reproduction_count;
    next_move next_motion;
    
}cell;

typedef struct {
    int consumer_capacity;
    
    int producer_capacity;

    int temperature;

    //int PH;

}grid_features;
typedef struct {
    // 行数
    uint16_t rows;

    // 列数
    uint16_t cols;

    GridState **normal_grids;

    cell **special_grids;

    food_gridstate **food_grids;

    grid_features grid_feature;

    int probability;

} Conway;

typedef struct{
    int number_producer;
    int number_consumer;
}counting;






//计算邻居
GridState count_neighbor(const Conway *c, const uint16_t x, const uint16_t y);

//清除缓冲区，因为fgets和scanf混用
void clear_input();


Conway* update_conway( uint16_t rows,  uint16_t cols);


// 删除格点，回收内存
// free()
// 构造新格点，分配内存
// malloc()

void decide_new_conway(const uint16_t rows, const uint16_t cols);
    



// 删除格点，回收分配给格点二维数组的内存
// Conway游戏本身的结构体 c 不用删除，只删除其格点
// 使用free()
void delete_grids(Conway *c);



// 随机地初始化格点
void init_random(Conway *c);
    


// 将系统演化到下一世代
void next_generation(Conway *c);



// 获取格点的当前状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
// 虽然看上去这样一个封装没有必要，毕竟可以 c->_grids[x][y]来访问
// 但是封装后会安全一点
// 越界或者遇到空指针返回GridState::None ?
// if (get_current_state(c, x, y) == GridState::None) {
//     // balabalabala
// }
GridState get_state(const Conway *c, const uint16_t x, const uint16_t y);



void set_state(Conway *c, const uint16_t x, const uint16_t y, GridState s);


// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state(const Conway *c, const uint16_t x, const uint16_t y);



// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void print_conway( Conway *c);



// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *path);



// 从文件读取格点
// 失败则Conway._grids = NULL
// 涉及malloc()
Conway* new_conway_from_file(Conway *c,const char *filename);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//特殊模式函数

//创建special类型conway指针
Conway* new_conway_special(uint16_t rows,uint16_t cols);

//删除special类型Conway并删除
void delete_special_grids(Conway *c);

//打印special类型Conway
void print_conway_special(Conway *c);

//初始化细胞状态
void initialize_cell_condition(Conway*c,int x,int y);

//随机初始化special类型Conway
void init_random_special(Conway *c);

//计算邻居
int count_special_mode_neighbors_producer(const Conway*special_grid,const uint16_t x,const uint16_t y);

//计算邻居
int count_special_mode_neighbors_consumer(const Conway*special_grid,const uint16_t x,const uint16_t y);

int count_living_producer(Conway*c);

int count_living_consumer(Conway*c);
//交换两个细胞的状态（用于移动细胞）
void swap_cell_condition(Conway*c,int x_1,int y_1,int x_2,int y_2);

//得到下一步状态
cell get_next_state_of_producer( Conway*special_grid, int x, int y);

//用于计算移动
int count_food_nearby(Conway*c,int x,int y);

next_move consumer_default_move(Conway*c,int x,int y);

//得到下一步状态
void consumer_action( Conway*special_grid, int x, int y);

//食物由消费者自己出
void Generate_food(Conway*c,int x,int y);



void propagate_new_producer(Conway*c);

void propagate_new_consumer(Conway*c,int x,int y,int reproduction_count_1);



counting update_conway_special(Conway*c);
#endif