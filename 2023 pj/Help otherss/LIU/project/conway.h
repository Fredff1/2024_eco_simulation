#ifndef CONWAY_H
#define CONWAY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define STATE_DEAD 0
#define STATE_ALIVE 1

// 用作get越界时的返回值
#define STATE_NONE -1

// 其它状态，可扩展
#define STATE_OTHER 2

#define LOG_FILE "app.log"

typedef enum {
    Dead = 0,
    Alive = 1,
    Infected = 2,
    Food = 3,
    Free = 4,   //Free模式可不受Conway规则制约
    None = -1,  //越界返回值
} GridState;

typedef enum {
    Normal = 0,
    Special = 1,
} GameMode;    //调节游戏模式


typedef struct {
    int x;
    int y;
} Point;  // 结构体作用：记录格点位置

// 康威生命游戏(Conway's Game of Life)
// 在一个二维方格中，每个格点有生存或者死亡的状态
// 每个格点在下一个世代的生死状态由周围8个格点的状态决定（硬边界 or 周期性边界条件）
// 基本演化规则如下：
// 1. 任何活格点，若邻居活格点数 < 2，则该格点在下一世代死亡
// 2. 任何活格点，若邻居活格点数 = 2 或 = 3，则该格点在下一世代存活
// 3. 任何活格点，若邻居活格点数 > 2，则该格点在下一世代死亡
// 4. 任何空格点，若邻居活格点数 = 3，则该格点在下一世代变为活格点
typedef struct {
    uint16_t m;    //行数
    uint16_t n;    //列数
    GridState **_grids;
    uint16_t possibility;    //概率
} Conway;

// 构造新格点，分配内存
// 创建失败则返回NULL
// malloc()
Conway *new_conway(const uint16_t m, const uint16_t n);

// 删除格点，回收内存
// free()
void delete_grids(Conway *c);

// 随机地初始化格点
void init_random(Conway *c);

//判断给定的点是否在网格范围内
bool is_valid_point(Point p, const Conway *c);

int count_neighbour(const Conway *c, const uint16_t x, const uint16_t y);

// 将系统演化到下一世代
void next_generation(Conway *c);

// 自定义的系统演化
void custom_next_generation(Conway *c);

// 获取格点的当前状态
GridState get_state(const Conway *c, const uint16_t x, const uint16_t y);

//设置格点状态
void set_state(Conway *c, const uint16_t x, const uint16_t y, GridState s);

// 获取格点下一个世代的状态
GridState get_next_state_cell(const Conway *c, const uint16_t x, const uint16_t y);

// 展示格点
void show_conway(const Conway *c);

// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *path);

// 从文件读取格点
Conway *new_conway_from_file(const char *path);

//改变格点状态
GridState turn_state(Conway *c, const uint16_t x, const uint16_t y);

//在空格点随机生成病毒
void set_virus(Conway *c);

//在空格点随机生成食物
void set_food(Conway *c);

//病毒杀死细胞
void virus_infect_cell(const Conway *c, const uint16_t x, const uint16_t y);

//细胞杀死病毒
GridState cell_kill_virus(const Conway *c, const uint16_t x, const uint16_t y);

//病毒搜寻食物
void search_food(Conway *c);

//计算距离函数
double distance(Point p1, Point p2);

//特殊模式下获取格点下一个状态
GridState get_next_state_special(Conway *c, const uint16_t x, const uint16_t y);

//食物的效果
void food_effect(Conway *c, const uint16_t x, const uint16_t y);

//特殊模式下展示格点
void show_conway_special(const Conway *c);

//日志记录
void write_log(const char *message);

//特殊模式下从文件读取格点
Conway *new_conway_from_file_special(const char *path);
#endif