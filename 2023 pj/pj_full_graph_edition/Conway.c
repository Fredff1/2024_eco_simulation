#include "conway.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// ctrl+k+0可以折叠所有函数

// ctrl+k+j可以一键打开折叠

// 环境因子

// 五种温度基因对应的适应性
float temperature_fac[5][11] = {{1.1, 1.4, 1.2, 1.0, 0.8, 0.6, 0.4, 0.3, 0.2, 0.1, 0.05},
                                {0.7, 0.8, 0.9, 1.1, 0.8, 0.75, 0.6, 0.5, 0.4, 0.3, 0.15},
                                {0.3, 0.4, 0.5, 0.8, 0.9, 1.0, 0.9, 0.7, 0.6, 0.5, 0.4},
                                {0.15, 0.3, 0.4, 0.5, 0.6, 0.75, 0.8, 1.1, 0.9, 0.8, 0.7},
                                {0.05, 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.1}};

float light_fac[5][7] = {
    {1.1, 1.4, 1.0, 0.7, 0.4, 0.15, 0.05}, {0.8, 1.0, 1.1, 0.9, 0.7, 0.5, 0.25},  {0.3, 0.5, 0.7, 1.0, 1.2, 0.7, 0.4},
    {0.25, 0.5, 0.7, 0.9, 1.1, 1.0, 0.8},  {0.05, 0.15, 0.4, 0.7, 1.0, 1.4, 1.1},
};

float oxygen_fac[5][7] = {
    {1.1, 1.4, 1.0, 0.7, 0.4, 0.15, 0.05}, {0.8, 1.0, 1.1, 0.9, 0.7, 0.5, 0.25},  {0.3, 0.5, 0.7, 1.0, 1.2, 0.7, 0.4},
    {0.25, 0.5, 0.7, 0.9, 1.1, 1.0, 0.8},  {0.05, 0.15, 0.4, 0.7, 1.0, 1.4, 1.1},
};

void delete_grids(Conway *c) {
    int temp_rows = c->rows;
    if (c->rows != 0 && c->cols != 0 && c->normal_grids != NULL) {
        for (int i = 0; i < temp_rows; i++) {
            free(c->normal_grids[i]);
        }
        free(c->normal_grids);
        c->normal_grids = NULL;
    }
    c->rows = 0;
    c->cols = 0;
}

// 清除缓冲区
void clear_input() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF) {}
}
// 前言：涉及指针操作和内存分配，随时记得指针判空

// 构造新格点，分配内存
// 创建失败则返回NULL
// malloc()
// 创建一个新的normal_grids,注意使用时应该在之前使用delete_grids函数
Conway *update_conway(uint16_t rows, uint16_t cols) {
    Conway *c = (Conway *)malloc(sizeof(Conway));
    if (c == NULL) { // 即指针c是NULL
        return NULL;
    }
    c->rows = rows;
    c->cols = cols;
    if (c->rows > 150) {
        c->rows = 150;
    }
    if (c->cols > 150) {
        c->cols = 150;
    }
    c->normal_grids = (GridState **)malloc(rows * sizeof(GridState *));
    if (c->normal_grids == NULL) {
        free(c);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        c->normal_grids[i] = (GridState *)malloc(cols * sizeof(GridState));
        if (c->normal_grids[i] == NULL) {
            for (int j = 0; j < rows; j++) {
                free(c->normal_grids[j]);
            }
            free(c->normal_grids);
            free(c);
            return NULL;
        }
        memset(c->normal_grids[i], 0, cols * sizeof(GridState));
    }
    memset(c->currentEvent, none_event, 10 * sizeof(c->currentEvent[0]));
    memset(c->event_remain_turn, 0, 10 * sizeof(int));

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
GridState get_state(const Conway *c, const uint16_t x, const uint16_t y) {
    if (c == NULL || c->rows == 0 || c->cols == 0) {
        return None;
    }
    if (x < c->rows && y < c->cols) {
        return c->normal_grids[x][y];
    } else {
        return None;
    }
}

// 设置格点状态
GridState set_state(Conway *c, const uint16_t x, const uint16_t y, GridState state) {
    if (c == NULL) {
        return None;
    }
    if (x >= c->rows || y >= c->cols) {
        return None;
    } else {
        c->normal_grids[x][y] = state;
        return state;
    }
}

// 随机地初始化格点
void init_random(Conway *c) {
    GridState temp_state;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            set_state(c, i, j, Dead);
        }
    }
    int change_into_living;
    if (c->probability < 0 || c->probability > 100) { // 判断特殊情况
        c->probability = 40;
    }
    change_into_living = (c->rows) * (c->cols) * (c->probability) / 100; // 决定需要改变的个数
    for (int i = 0; i < change_into_living; i += 0) {
        uint16_t rand_rows = rand() % (c->rows);
        uint16_t rand_cols = rand() % (c->cols);
        temp_state = get_state(c, rand_rows, rand_cols);
        if (temp_state == Dead) {
            set_state(c, rand_rows, rand_cols, Alive);
            i += 1;
        }
    }
}

int count_neighbor(const Conway *c, const uint16_t x, const uint16_t y) {
    int living_neighbor = 0;
    GridState temp_grid[c->rows][c->cols];
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_grid[i][j] = get_state(c, i, j);
        }
    }
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && j >= 0 && i < c->rows && j < c->cols) {
                if (temp_grid[i][j] == Alive) {
                    living_neighbor += 1;
                }
            }
        }
    }

    living_neighbor -= get_state(c, x, y);
    return living_neighbor;
}

// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state(const Conway *c, const uint16_t x, const uint16_t y) {
    GridState state;
    int neighbor = 0;
    if (!(x < c->rows && y < c->cols && x >= 0 && y >= 0)) {
        return None;
    }
    neighbor = count_neighbor(c, x, y);
    if (neighbor < 2) {
        return Dead;
    } else if (neighbor == 2) {
        return get_state(c, x, y);
    } else if (neighbor == 3) {
        return Alive;
    } else if (neighbor > 3) {
        return Dead;
    } else {
        return None;
    }
}

// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void print_conway(Conway *c) {
    if (c->rows < 0 || c->cols < 0) {
        c->rows = 0;
        c->cols = 0;
    }
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            GridState u = get_state(c, i, j);
            if (u == Dead) {
                printf("| ");
            } else if (u == Alive) {
                printf("|*");
            }
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}

// 将系统演化到下一世代
void next_generation(Conway *c) {
    GridState temp_grid[c->rows][c->cols];
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_grid[i][j] = get_next_state(c, i, j);
        }
    }
    // 更新原来的格点
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            set_state(c, i, j, temp_grid[i][j]);
            // c->normal_grids[i][j]=temp_grid[i][j];
        }
    }
}

// 自定义的系统演化
// void custom_next_generation(Conway *c);

// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        return -1; // 返回错误代码
    }
    int temp_rows = c->rows;
    int temp_cols = c->cols;
    fprintf(fp, "%d,%d\n", temp_rows, temp_cols); // 第一行的数据
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            GridState write_state = get_state(c, i, j);
            fprintf(fp, "%d,", write_state);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

// 从文件读取格点
// 失败则Conway._grids = NULL
Bool new_conway_from_file(Conway *c, const char *filename) {

    FILE *fp = fopen(filename, "r");
    int temp_row, temp_col;
    // printf("ok\n");
    if (fp == NULL) {
        return False;
    }
    if (fscanf(fp, "%d,%d", &temp_row, &temp_col) != 2) {
        fclose(fp);
        return False;
    }
    // if(update_conway(c->rows,c->cols)!=NULL){}
    // delete_grids(c);
    delete_grids(c);
    free(c);
    c = update_conway(temp_row, temp_col);
    // printf("%d %d\n",c->rows,c->cols);

    fscanf(fp, "\n"); // 跳过第一行
    char scan_input;
    GridState temp_grid[temp_row][temp_col];
    for (int i = 0; i < temp_row; i++) {
        for (int j = 0; j < temp_col; j++) {
            scan_input = fgetc(fp); // 逐个读取
            if (scan_input == '0') {
                temp_grid[i][j] = Dead;
            } else if (scan_input == '1') {
                temp_grid[i][j] = Alive;
            } else if (scan_input == '\n' || scan_input == EOF) {
                break;
            } else if (scan_input == ',') {
                j -= 1; // 跳过","
            }
        }
        fscanf(fp, ",");  // 最后一个逗号
        fscanf(fp, "\n"); // 跳过换行符
    }
    fclose(fp);

    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            set_state(c, i, j, temp_grid[i][j]);
        }
    }
    c->rows = temp_row;
    c->cols = temp_col;
    return True;
}

int count_living_cell_normal(Conway *c) {
    int count = 0;
    GridState temp_cell;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_cell = get_state(c, i, j);
            if (temp_cell == Alive) {
                count += 1;
            }
        }
    }
    return count;
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

// 以下函数用于特殊模式,函数名规则:与普通模式对应并加上_special

cell get_state_special(const Conway *c, const uint16_t x, const uint16_t y) {

    if (x < c->rows && y < c->cols) {
        cell temp_cell = c->special_grids[x][y];
        return temp_cell;

    } else {
        cell temp_cell;
        return temp_cell;
    }
}

int set_state_special(Conway *c, const uint16_t x, const uint16_t y, cell new_cell) {
    if (x < c->rows && y < c->cols) {
        c->special_grids[x][y] = new_cell;
        return 0;
    } else {
        return 1;
    }
}

food_gridstate get_food_state(const Conway *c, const uint16_t x, const uint16_t y) {
    if (x < c->rows && y < c->cols) {
        return c->food_grids[x][y];
    } else {
        return food_0;
    }
}

int set_food_state(Conway *c, const uint16_t x, const uint16_t y, int new_number) {
    food_gridstate new_food = (food_gridstate)new_number;
    if (x < c->rows && y < c->cols) {
        c->food_grids[x][y] = new_food;
        return 0;
    } else {
        return 1;
    }
}

cell new_cell_setting() {
    cell new_cell;
    new_cell.age = 0;
    new_cell.hunger = 0;
    new_cell.next_motion = current;
    new_cell.reproduction_count = 0;
    new_cell.type = Dead;
    new_cell.gene_setting.temper_preference = 2; // 适应正常温度
    new_cell.gene_setting.oxy_preference = 2;
    new_cell.gene_setting.light_preference = 2;
    new_cell.gene_setting.age_length = 20;
    return new_cell;
}

void set_grid_feature_default(Conway *c) {
    c->grid_feature.producer_capacity = c->rows * c->cols / 4;        // 初始化生产者的环境容纳量
    c->grid_feature.consumer_capacity = count_living_producer(c) / 3; // 消费者的环境容纳量随生产者数量变化
    c->grid_feature.oxygen_resource = normal_oxy;
    c->grid_feature.light_resource = normal_light;
    c->grid_feature.temperature = normal_tem;
    c->grid_feature.count_turn = 0;
    c->grid_feature.add_oxygen_count = 0;
}

// 创建special_grids
Conway *new_conway_special(uint16_t rows, uint16_t cols) {
    Conway *c = (Conway *)malloc(sizeof(Conway));
    if (!c) { // 即指针c是NULL
        return NULL;
    }
    c->rows = rows;
    c->cols = cols;
    if (c->rows > 150) {
        c->rows = 150;
    }
    if (c->cols > 150) {
        c->cols = 150;
    }
    c->special_grids = (cell **)malloc(rows * sizeof(cell *));
    if (!c->special_grids) {
        free(c);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        c->special_grids[i] = (cell *)malloc(cols * sizeof(cell));
        if (!c->special_grids[i]) {
            for (int j = 0; j < rows; j++) {
                free(c->special_grids[j]);
            }
            free(c->special_grids);
            free(c);
            return NULL;
        }
    } // 这里创建的是用于打印的格点

    cell initialized_cell = new_cell_setting();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            c->special_grids[i][j] = initialized_cell;
        } // 初始化每个细胞状态
    }
    // 初始化grid feature和event
    set_grid_feature_default(c);
    initialize_event(c);

    c->food_grids = (food_gridstate **)malloc(rows * sizeof(food_gridstate *));
    if (!c->food_grids) {
        free(c);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        c->food_grids[i] = (food_gridstate *)malloc(cols * sizeof(food_gridstate));
        if (!c->food_grids[i]) {
            for (int j = 0; j < rows; j++) {
                free(c->food_grids[j]);
            }
            free(c->food_grids);
            free(c);
            return NULL;
        }
        memset(c->food_grids[i], 0, cols * sizeof(int));
    }

    int start_food = rows * cols / 2; // 初始由一半的格点会生成食物
    int temp_rows, temp_cols;
    for (int i = 0; i < start_food; i += 0) {
        temp_rows = rand() % rows;
        temp_cols = rand() % cols;
        if (c->food_grids[temp_rows][temp_cols] == food_0) { // 没有食物则初始化为1个食物
            c->food_grids[temp_rows][temp_cols] = food_1;
            i += 1;
        }
    }
    c->grid_feature.count_turn = 0;
    c->grid_feature.add_oxygen_count = 0;
    c->grid_feature.light_resource = normal_light;
    c->grid_feature.oxygen_resource = normal_oxy;
    c->grid_feature.temperature = normal_tem;

    memset(c->currentEvent, none_event, 10 * sizeof(c->currentEvent[0]));
    memset(c->event_remain_turn, 0, 10 * sizeof(int));
    return c;
}

// 释放分配的内存
void delete_special_grids(Conway *c) {

    int temp_rows = c->rows;
    if (c->rows != 0 && c->cols != 0 && c->special_grids != NULL && c->food_grids != NULL) {
        for (int i = 0; i < temp_rows; i++) {
            free(c->special_grids[i]);
            free(c->food_grids[i]);
        }
        free(c->special_grids);
        free(c->food_grids);
        c->special_grids = NULL;
        c->food_grids = NULL;
        c->rows = 0;
        c->cols = 0;
    } else {
        return;
    }
}

// 打印特殊格点
void print_conway_special(Conway *c) {
    if (c->rows < 0 || c->cols < 0) {
        c->rows = 0;
        c->cols = 0;
    }
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            cell u = get_state_special(c, i, j);
            if (u.type == Dead) {
                printf("| ");
            } else if (u.type == living_producer) {
                printf("|\033[32m*\033[0m");
            } else if (u.type == living_consumer) {
                printf("|\033[31m$\033[0m");
            } else {
                printf("( ");
            }
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
    // //////打印食物，注意只是用来测试
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            int u = c->food_grids[i][j];
            printf("%d", u);
        }

        printf("|");
        printf("\n");
    }
    printf("\n\n");
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            cell temp_cell = get_state_special(c, i, j);
            if (temp_cell.type == living_consumer) {
                printf("age:%d hunger:%d reproduction_count:%d\n", temp_cell.age, temp_cell.hunger, temp_cell.reproduction_count);
            }
        }
    }
}

// 初始化细胞状态
void initialize_cell_condition(Conway *c, int x, int y) {
    // 用于赋值
    if (x > 0 && y > 0 && x < c->rows && y < c->cols) {
        c->special_grids[x][y] = new_cell_setting();
    }
    return;
}

// 用于初始化生产者和消费者
void init_random_special(Conway *c) {
    cell temp_cell = new_cell_setting();
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            set_state_special(c, i, j, temp_cell);
        }
    }
    int change_into_living;
    if (c->probability <= 0 || c->probability > 100) { // 判断特殊情况
        c->probability = 20;
    }
    change_into_living = (c->rows) * (c->cols) * (c->probability) / 100; // 决定生产者
    for (int i = 0; i < change_into_living; i += 0) {
        int rand_rows = rand() % (c->rows);
        int rand_cols = rand() % (c->cols);
        if (c->special_grids[rand_rows][rand_cols].type == Dead) {
            initialize_cell_condition(c, rand_rows, rand_cols);
            temp_cell.type = living_producer;
            temp_cell.age = 0;
            temp_cell.reproduction_count = 0;
            set_state_special(c, rand_rows, rand_cols, temp_cell);
            i += 1;
        }
    }
    cell temp_cell_2 = new_cell_setting();
    int consumer_number = change_into_living / 8;
    for (int i = 0; i < consumer_number; i += 0) { // 决定消费者
        int rand_rows = rand() % (c->rows);
        int rand_cols = rand() % (c->cols);
        // 用于判断
        temp_cell_2 = get_state_special(c, rand_rows, rand_cols);
        if (temp_cell_2.type == living_producer) {
            initialize_cell_condition(c, rand_rows, rand_cols);
            temp_cell.type = living_consumer;
            temp_cell.age = 0;
            temp_cell.hunger = 0;
            set_state_special(c, rand_rows, rand_cols, temp_cell);
            i += 1;
        }
    }
    set_grid_feature_default(c);
}

int count_special_mode_neighbors_producer(const Conway *c, const uint16_t x, const uint16_t y) {
    int living_neighbor = 0;
    cell temp_grid[c->rows][c->cols];
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_grid[i][j] = get_state_special(c, i, j);
        }
    }

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && j >= 0 && i < c->rows && j < c->cols) {
                if (temp_grid[i][j].type == living_producer) {
                    living_neighbor += 1;
                }
            }
        }
    }

    if (temp_grid[x][y].type == living_producer) {
        living_neighbor -= 1;
    }
    return living_neighbor;
}

// 计算邻居
int count_special_mode_neighbors_consumer(const Conway *c, const uint16_t x, const uint16_t y) {
    int living_neighbor = 2;
    cell temp_grid[c->rows][c->cols];
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_grid[i][j] = get_state_special(c, i, j);
        }
    }

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && j >= 0 && i < c->rows && j < c->cols) {
                if (temp_grid[i][j].type == living_consumer) {
                    living_neighbor += 1;
                }
            }
        }
    }
    living_neighbor -= temp_grid[x][y].type;
    return living_neighbor;
}

// 计算生产者总数
int count_living_producer(Conway *c) {
    int count = 0;
    cell temp_cell;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_cell = get_state_special(c, i, j);
            if (temp_cell.type == living_producer) {
                count += 1;
            }
        }
    }
    return count;
}

// 计算消费者总数
int count_living_consumer(Conway *c) {
    int count = 0;
    cell temp_cell;
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_cell = get_state_special(c, i, j);
            if (temp_cell.type == living_consumer) {
                count += 1;
            }
        }
    }
    return count;
}

// 交换两个格子的细胞状态
void swap_cell_condition(Conway *c, int x_1, int y_1, int x_2, int y_2) {
    // 用于交换
    cell temp_cell_condition;
    // 用于赋值
    cell temp_cell_condition2;
    temp_cell_condition = get_state_special(c, x_1, y_1);
    temp_cell_condition2 = get_state_special(c, x_2, y_2);
    set_state_special(c, x_1, y_1, temp_cell_condition2); // 改变第一个
    set_state_special(c, x_2, y_2, temp_cell_condition);  // 改变第二个
}

cell copy_cell_condition(Conway *c, int x, int y) {
    cell temp_cell = get_state_special(c, x, y);
    return temp_cell;
}

// 用于得到生产着的下一步状态，注意这个函数不会判断非producer
cell get_next_state_of_producer(Conway *c, int x, int y) {
    // 记录当前格点状态
    cell cell_condition = get_state_special(c, x, y);
    int count_1 = count_living_producer(c);
    c->grid_feature.producer_capacity = c->rows * c->cols / 5 * 2;
    if (cell_condition.type == living_producer) {
        int neighbor_producer = count_special_mode_neighbors_producer(c, x, y);
        if (cell_condition.age > cell_condition.gene_setting.age_length / 5) {
            cell_condition.type = Dead;
        }
        if (neighbor_producer < 2) { // 数量过少(小于容纳量三分之一时不会孤独)
            if ((count_1 * 2) >= c->grid_feature.producer_capacity) {
                cell_condition.type = Dead;
            } else {
                cell_condition.type = cell_condition.type;
            }
        } else if (neighbor_producer == 2) {
            cell_condition.type = cell_condition.type;
        } else if (neighbor_producer == 3) {
            cell_condition.type = living_producer;
            cell_condition.age = 0;
            cell_condition.reproduction_count = 0;
        } else if (neighbor_producer > 3) { // 拥挤会导致死亡
            cell_condition.type = Dead;
        } else {
            cell_condition.type = None;
        }
    }
    if (cell_condition.type == Dead) {
        cell_condition.age = 0;
        cell_condition.hunger = 0;
        cell_condition.next_motion = current;
        cell_condition.reproduction_count = 0; // 初始化死细胞所有的项目
    }
    return cell_condition;
}

// 用于计算消费者移动
int count_food_nearby(Conway *c, int x, int y) {
    int count = 0;
    int row_length = 2;
    int col_length = 2;
    cell temp_cell;
    food_gridstate temp_food;
    for (int i = (-1 * row_length); i <= row_length; i++) {
        for (int j = (-1 * col_length); j <= col_length; j++) {
            if (x + i >= 0 && x + i < c->rows && y + j >= 0 && y + j < c->cols) {
                temp_food = get_food_state(c, x + i, y + j);
                count += temp_food;
                temp_cell = get_state_special(c, x + i, y + j);
                if (temp_cell.type == living_producer) {
                    count += 2;
                }
            }
        }
    }
    return count;
}

// 得到一个消费者下一步的移动方向,倾向于往食物更多的方向
next_move consumer_default_move(Conway *c, int x, int y) {
    next_move best_direction = current;
    int max_food = -1;
    int new_x, new_y;
    int new_direction[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 上1 下2 左3 右4
    for (int i = 0; i < 4; i++) {
        new_x = x + new_direction[i][0];
        new_y = y + new_direction[i][1];
        if (new_x >= 0 && new_y >= 0 && new_x < c->rows && new_y < c->cols) {
            int count = count_food_nearby(c, new_x, new_y);
            if (count > max_food) {
                max_food = count;
                best_direction = (next_move)(i + 1);
            }
        }
    }
    if (best_direction > 0) {
        return best_direction;
    } else {
        return current;
    }
}

// 用来得到消费者的下一步状态,注意直接改变了格点
void consumer_action(Conway *c, int x, int y) {
    // 记录当前格点状态
    cell cell_condition = get_state_special(c, x, y);
    if (cell_condition.type == living_consumer) {
        int neighbor_consumer = count_special_mode_neighbors_consumer(c, x, y);
        if (neighbor_consumer > 3) {
            cell_condition.type = Dead;
        } else if (cell_condition.hunger > 4) {
            cell_condition.type = Dead;
        } else if (cell_condition.age > get_state_special(c, x, y).gene_setting.age_length) {
            cell_condition.type = Dead;
        }
        if (cell_condition.type == Dead) {
            cell_condition = new_cell_setting();
            set_state_special(c, x, y, cell_condition);
            return; // 死亡的消费者当然不会进食啦
        }
    }
    int count_food = 0;
    // 记录附近格点的状态
    cell temp_cell;
    // 得到当前移动状态
    cell temp_cell_3 = get_state_special(c, x, y);
    temp_cell_3.next_motion = consumer_default_move(c, x, y);
    set_state_special(c, x, y, temp_cell_3);
    return;
}

// 食物生产（生产者3*3范围）
void Generate_food(Conway *c, int x, int y) {
    int gen_proportion = 5 / temperature_fac[c->special_grids[x][y].gene_setting.temper_preference][c->grid_feature.temperature] /
                         oxygen_fac[c->special_grids[x][y].gene_setting.oxy_preference][c->grid_feature.oxygen_resource] /
                         light_fac[c->special_grids[x][y].gene_setting.light_preference][c->grid_feature.light_resource];
    if (gen_proportion < 1) {
        gen_proportion = 1;
    }
    int generated_food = 1;
    int flag = rand() % gen_proportion;
    if (flag != 0) {
        return;
    }
    int count = rand() % 9;
    int count_random = 0;
    for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {
            if (i >= 0 && j >= 0 && i < c->rows && j < c->cols && count_random == count) {
                food_gridstate temp_food = get_food_state(c, i, j);
                set_food_state(c, i, j, temp_food + 1);
                break;
            }
            count_random += 1;
        }
    }
    // 记录当前格点状态
}

// 用于消费者的繁殖
void propagate_new_consumer(Conway *c, int x, int y, int reproduction_count_1) {
    int rand_choice;
    int available_grid = 0;
    int count = 0;
    // 记录当前格点细胞状态
    cell temp_cell = get_state_special(c, x, y); // 原来的消费者
    if (temp_cell.type == living_consumer && temp_cell.age >= 1 && temp_cell.age < 20 && temp_cell.hunger < 3) {
        // 遍历附近的九格
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                // 用于暂时记录附近格点状态,找到条件合适的格点
                cell temp_cell_2 = get_state_special(c, i, j);
                if (i >= 0 && j >= 0 && i < c->rows && j < c->cols && (temp_cell_2.type == Dead || temp_cell_2.type == living_producer)) {
                    if (i >= 0 && j >= 0 && i < c->rows && j < c->cols && (temp_cell_2.type == Dead || temp_cell_2.type == living_producer)) {
                        available_grid += 1;
                    }
                }
            }
        }
        // 注意除以0是未定义的行为
        if (available_grid != 0) {
            rand_choice = rand() % available_grid;
        } else {
            rand_choice = 1;
        }

        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                cell temp_cell_2 = get_state_special(c, i, j);
                if (i >= 0 && j >= 0 && i < c->rows && j < c->cols && (temp_cell_2.type == Dead || temp_cell_2.type == living_producer) &&
                    count == rand_choice) {
                    if (temp_cell.reproduction_count >= reproduction_count_1) {
                        temp_cell_2 = copy_cell_condition(c, x, y);
                        temp_cell_2.age = 0;
                        temp_cell_2.reproduction_count = 0;
                        temp_cell_2.hunger = 0;
                        temp_cell.hunger += 1; // 繁殖消耗原细胞饥饿值
                        temp_cell.reproduction_count = 0;
                        set_state_special(c, x, y, temp_cell);
                        set_state_special(c, i, j, temp_cell_2);
                        return;
                    } else {
                        temp_cell.reproduction_count += 1;
                        set_state_special(c, x, y, temp_cell);
                        return;
                    }
                    count += 1;
                }
            }
        }
    }
}

// 用于繁殖生产者
void propagate_new_producer(Conway *c, int x, int y) {
    int flag = True;
    int count_turn = 0;
    while (flag == True) {
        int rand_row = rand() % c->rows;
        int rand_col = rand() % c->cols;
        if (c->special_grids[rand_row][rand_col].type == Dead) {
            cell temp_cell = copy_cell_condition(c, x, y);
            temp_cell.age = 0;
            temp_cell.reproduction_count = 0;
            set_state_special(c, rand_row, rand_col, temp_cell);
            flag = False;
        }
        count_turn += 1;
        if (count_turn > c->rows * c->cols) {
            flag = False;
        }
    }
}

// 消费者进食的逻辑
void cell_special_move(Conway *c, int x, int y) {
    int count_food = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && j >= 0 && i < c->rows && j < c->cols) {
                cell cell_condition = get_state_special(c, i, j);
                // 暂时附近格点储存食物数量
                food_gridstate temp_food = get_food_state(c, i, j);
                if (temp_food > food_0 && cell_condition.type == Dead) {
                    cell temp_cell = get_state_special(c, x, y);
                    food_gridstate temp_food = get_food_state(c, i, j);
                    temp_cell.hunger -= 1;
                    set_state_special(c, x, y, temp_cell);
                    set_food_state(c, i, j, temp_food - 1);
                    count_food += 1; // 如果附近有食物吃到食物，继续移动
                    return;

                } else if (cell_condition.type == living_producer && temp_food > food_0) {
                    cell temp_cell = get_state_special(c, x, y);
                    food_gridstate temp_food = get_food_state(c, i, j);
                    temp_cell.hunger -= 2;
                    set_state_special(c, x, y, temp_cell);
                    set_food_state(c, i, j, temp_food - 1);
                    initialize_cell_condition(c, i, j);
                    return;
                    // 吃消费者和食物
                }
                int seed = rand() % 4;
                if (cell_condition.type == living_producer && temp_food <= food_0 && seed == 0) {
                    cell temp_cell = get_state_special(c, x, y);
                    food_gridstate temp_food = get_food_state(c, i, j);
                    temp_cell.hunger -= 1;
                    set_state_special(c, x, y, temp_cell);
                    set_food_state(c, i, j, temp_food);
                    initialize_cell_condition(c, i, j);
                    count_food += 1;
                    return;
                }
            }
        }
    }

    // 没有食物就吃生产者
}

void consumer_move(Conway *c, Conway *temp_conway) {
    next_move decided_next_move; // 上1 下2 左3 右4
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            // 不是消费者不参与移动
            if (temp_conway->special_grids[i][j].type != living_consumer) {
                continue; // 不是消费者则跳过循环
            } else if (c->grid_feature.temperature < cool - 1) {
                int flag_move = rand() % (c->grid_feature.temperature + 2);
                if (flag_move == 0) {
                    continue; // 温度过低会影响消费者的移动
                }
            }
            decided_next_move = temp_conway->special_grids[i][j].next_motion;
            switch (decided_next_move) {
                // 移动的逻辑
                cell temp_cell;
            case up:
                temp_cell = get_state_special(temp_conway, i - 1, j);
                if ((i > 0) && (temp_cell.type == Dead || temp_cell.type == living_producer)) { // 照例的越界和目标判断
                    swap_cell_condition(temp_conway, i, j, i - 1, j);
                    initialize_cell_condition(temp_conway, i, j);
                    temp_conway->special_grids[i - 1][j].next_motion = current;
                    if (temp_cell.type == living_producer) {
                        temp_conway->special_grids[i - 1][j].hunger -= 1;
                    }
                } else if (i < c->rows - 1 && temp_cell.type == living_consumer) {
                    swap_cell_condition(temp_conway, i, j, i + 1, j); // 遇到消费者会反弹
                } else {
                    // swap_cell_condition(temp_conway,i,j,i+1,j);
                    // temp_conway->special_grids[i+1][j].next_motion=current;
                    temp_conway->special_grids[i][j].next_motion = current;
                }

                break;
            case down:
                temp_cell = get_state_special(temp_conway, i + 1, j);
                if ((i < c->rows - 1) && (temp_cell.type == Dead || temp_cell.type == living_producer)) {
                    swap_cell_condition(temp_conway, i, j, i + 1, j);
                    initialize_cell_condition(temp_conway, i, j);
                    temp_conway->special_grids[i + 1][j].next_motion = current;
                    if (temp_cell.type == living_producer) {
                        temp_conway->special_grids[i + 1][j].hunger -= 1;
                    }
                } else if (i > 0 && temp_cell.type == living_consumer) {
                    swap_cell_condition(temp_conway, i, j, i - 1, j);
                } else {
                    // swap_cell_condition(temp_conway,i,j,i-1,j);
                    // temp_conway->special_grids[i-1][j].next_motion=current;
                    temp_conway->special_grids[i][j].next_motion = current;
                    break;
                }

            case left:
                temp_cell = get_state_special(temp_conway, i, j - 1);
                if ((j > 0) && (temp_cell.type == Dead || temp_cell.type == living_producer)) {
                    swap_cell_condition(temp_conway, i, j, i, j - 1);
                    initialize_cell_condition(temp_conway, i, j);
                    temp_conway->special_grids[i][j - 1].next_motion = current;
                    if (temp_cell.type == living_producer) {
                        temp_conway->special_grids[i][j - 1].hunger -= 1;
                    }
                } else if (i < c->rows - 1 && temp_cell.type == living_consumer) {
                    swap_cell_condition(temp_conway, i, j, i + 1, j);
                } else {
                    // swap_cell_condition(temp_conway,i,j,i,j+1);
                    // temp_conway->special_grids[i][j+1].next_motion=current;
                    temp_conway->special_grids[i][j].next_motion = current;
                    break;
                }

            case right:
                temp_cell = get_state_special(temp_conway, i, j + 1);
                if ((j < c->cols - 1) && (temp_cell.type == Dead || temp_cell.type == living_producer)) {
                    swap_cell_condition(temp_conway, i, j, i, j + 1);
                    initialize_cell_condition(temp_conway, i, j);
                    temp_conway->special_grids[i][j + 1].next_motion = current;
                    if (temp_cell.type == living_producer) {
                        temp_conway->special_grids[i][j + 1].hunger -= 1;
                    }
                } else if (j > 0 && temp_cell.type == living_consumer) {
                    swap_cell_condition(temp_conway, i, j, i, j - 1);
                } else {
                    // swap_cell_condition(temp_conway,i,j,i,j-1);
                    // temp_conway->special_grids[i][j-1].next_motion=current;
                    temp_conway->special_grids[i][j].next_motion = current;
                }

                break;
            default:
                temp_conway->special_grids[i][j].next_motion = current;
                break;
            }
        }
    }
}

void producer_action(Conway *c, Conway *temp_conway) {
    // 先更新生产者的生存情况
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            if (temp_conway->special_grids[i][j].type == living_producer || temp_conway->special_grids[i][j].type == Dead) {
                temp_conway->special_grids[i][j] = get_next_state_of_producer(temp_conway, i, j);
            }
        }
    }
    // 每个生产者生产食物
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            if (temp_conway->special_grids[i][j].type == living_producer) {
                Generate_food(temp_conway, i, j);
            }
        }
    }
}

void all_producer_reproduction(Conway *c, Conway *temp_conway) {
    // 计算生产者数量并让它们繁殖
    int number_of_producer = count_living_producer(temp_conway);
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            if (temp_conway->special_grids[i][j].type == living_producer) {
                int is_reproducing = 4 / temperature_fac[c->special_grids[i][j].gene_setting.temper_preference][c->grid_feature.temperature] /
                                     oxygen_fac[c->special_grids[i][j].gene_setting.light_preference][c->grid_feature.light_resource] *
                                     number_of_producer / c->grid_feature.producer_capacity;

                if (is_reproducing < 1) {
                    is_reproducing = 1;
                }
                int flag_reproduce = rand() % (is_reproducing + 1);
                if (flag_reproduce == 0) {
                    propagate_new_producer(temp_conway, i, j);
                }
            }
        }
    }
    // 重新计算环境容纳量
    c->grid_feature.consumer_capacity = number_of_producer / 3;
}

void consumer_eat(Conway *c, Conway *temp_conway) {
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            if (temp_conway->special_grids[i][j].type == living_consumer) {
                cell_special_move(temp_conway, i, j);
                consumer_action(temp_conway, i, j);
                // 这一步后有消费者死亡
                if (temp_conway->special_grids[i][j].type == Dead) {
                    initialize_cell_condition(temp_conway, i, j);
                }
            }
        }
    }
}

void all_consumer_reproduction(Conway *c, Conway *temp_conway) {
    int number_of_consumer = count_living_consumer(temp_conway);
    int reproduction_need;

    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            // 由基因和环境决定繁殖需要
            reproduction_need = (int)(1 / temperature_fac[c->special_grids[i][j].gene_setting.temper_preference][c->grid_feature.temperature] /
                                      oxygen_fac[c->special_grids[i][j].gene_setting.oxy_preference][c->grid_feature.oxygen_resource] / 3 * 2 *
                                      number_of_consumer / c->grid_feature.consumer_capacity);
            if (reproduction_need < 1) {
                reproduction_need = 1;
            }
            propagate_new_consumer(temp_conway, i, j, reproduction_need);
        }
    }
}
// 消费者保底机制,优先生成在食物多的地方
void generate_extra_consumer(Conway *c, Conway *temp_conway) {
    int generate_number = 1;
    if ((count_living_consumer(c)) < c->grid_feature.consumer_capacity / 2) {
        generate_number = (c->grid_feature.consumer_capacity / 4);
    } else {
        generate_number = 0;
    }
    int count_1 = 0;
    int max_food = 0;
    int max_row = -1;
    int max_col = -1;
    int count_turn = 0;
    for (int i = 0; i < generate_number; i++) {

        for (int p = 0; p < c->rows; p++) {
            for (int j = 0; j < c->cols; j++) {
                if (temp_conway->special_grids[p][j].type != living_consumer) {
                    count_1 = count_food_nearby(c, p, j);
                    if (count_1 >= max_food) {
                        max_food = count_1;
                        max_row = p;
                        max_col = j;
                    }
                }
            }
        }
        if (max_row != -1 && max_col != -1) {
            initialize_cell_condition(temp_conway, max_row, max_col);
            cell temp_cell = new_cell_setting();
            temp_cell.type = living_consumer;
            set_state_special(temp_conway, max_row, max_col, temp_cell);
        }
        max_food = 0;
        count_turn += 1;
        if (count_turn > c->rows * c->cols) {
            break;
        }
    }
}

void copy_special_conway(Conway *c, Conway *temp_conway) {
    // 这个循环用于将temp_conway的变化拷贝至原来的c
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            if (temp_conway->food_grids[i][j] > 3) {
                temp_conway->food_grids[i][j] = food_3; // 设置食物的上限
            }
            if (temp_conway->special_grids[i][j].type == living_consumer) {
                if (temp_conway->special_grids[i][j].hunger < 0 || temp_conway->special_grids[i][j].hunger > 10) {
                    temp_conway->special_grids[i][j].hunger = 0;
                }
                temp_conway->special_grids[i][j].age += 1;
                temp_conway->special_grids[i][j].hunger += 1; // 更新年龄和饥饿
                change_gene(c, i, j);
            } else if (temp_conway->special_grids[i][j].type == living_producer) {
                temp_conway->special_grids[i][j].age += 1; // 更新生产者年龄
                change_gene(c, i, j);
            } else if (temp_conway->special_grids[i][j].type == Dead) {
                initialize_cell_condition(temp_conway, i, j);
            }
            set_state_special(c, i, j, temp_conway->special_grids[i][j]);
            set_food_state(c, i, j, temp_conway->food_grids[i][j]);
        }
    }
}

void update_grid_feature(Conway *c) {
    if ((count_living_producer(c) - count_living_consumer(c) >= c->grid_feature.producer_capacity * 3 / 2) && c->grid_feature.oxygen_resource < 4) {
        c->grid_feature.add_oxygen_count += 1;
    } else if (count_living_producer(c) <= count_living_consumer(c) * 5 && c->grid_feature.oxygen_resource > 0) {
        c->grid_feature.add_oxygen_count -= 1;
    }
    oxygen_level temp_oxy;
    if (c->grid_feature.add_oxygen_count > 5 && c->grid_feature.oxygen_resource < extreme_oxy) {
        change_oxygen(c, 1);
        c->grid_feature.add_oxygen_count = 0;
    } else if (c->grid_feature.add_oxygen_count < -2 && c->grid_feature.oxygen_resource > rare_oxy)
        change_oxygen(c, -1);
    c->grid_feature.add_oxygen_count = 0;
    c->grid_feature.count_turn += 1;
}

// 更新一个世代
void next_generation_special(Conway *c) {
    Conway *temp_conway = new_conway_special(c->rows, c->cols);
    // 创建temp_conway来进行下一步各种操作
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            temp_conway->special_grids[i][j] = get_state_special(c, i, j); // 复制到一个temp用来储存变化
            temp_conway->food_grids[i][j] = get_food_state(c, i, j);
        }
    }
    temp_conway->rows = c->rows;
    temp_conway->cols = c->cols;
    temp_conway->grid_feature.consumer_capacity = c->grid_feature.consumer_capacity;
    temp_conway->grid_feature.producer_capacity = c->grid_feature.producer_capacity;
    producer_action(c, temp_conway);
    // 消费者进食
    consumer_eat(c, temp_conway);
    // 消费者按照motion进行移动
    consumer_move(c, temp_conway);
    // 生产者繁殖
    all_producer_reproduction(c, temp_conway);
    // 这个循环用于消费者的繁殖(为了简单起见，使用分裂繁殖)
    all_consumer_reproduction(c, temp_conway);
    generate_extra_consumer(c, temp_conway);
    copy_special_conway(c, temp_conway);

    // 释放临时的temp_conway
    delete_special_grids(temp_conway);
    if(temp_conway!=NULL){
        free(temp_conway);
    }

    // 随机发生事件，可以调整概率
    int flag_event = rand() % 5;
    if (flag_event == 0) {
        event(c);
    }
    // 更改event状态
    updateEventState(c);

    // 更改一些格点特征
    update_grid_feature(c);

    return;
}

// 以下是一些具体的事件
// 1、冰川期
void event_ice_age(Conway *c) {
    if (c->grid_feature.temperature > cold) {
        change_temperature(c, -1);
    }
    if (c->grid_feature.light_resource > dark) {
        change_light(c, -1);
    }
    printf("The world is currently going through an ice age!\n");
}

void end_event_ice_age(Conway *c) {
    if (c->grid_feature.temperature < extreme_hot) {
        change_temperature(c, 1);
    }
    if (c->grid_feature.light_resource < extreme_light) {
        change_light(c, 1);
    }
}

// 2、火山喷发 有bug
void event_volcano_eruption(Conway *c) {
    if (c->grid_feature.temperature < hot) {
        change_temperature(c, 2);
    } else if (c->grid_feature.temperature < extreme_hot) {
        change_temperature(c, 1);
    }
    if (c->grid_feature.oxygen_resource > rare_oxy) {
        change_oxygen(c, -1);
    }
    if (c->grid_feature.light_resource > dark) {
        change_light(c, -1);
    }
    int rand_row = rand() % c->rows;
    int rand_col = rand() % c->cols;
    for (int i = rand_row - 2; i < rand_row + 3; i++) {
        for (int j = rand_col - 2; j < rand_col + 3; j++) {
            if (i > 0 && j > 0 && i < c->rows - 1 && j < c->cols - 1) {
                initialize_cell_condition(c, i, j);
                food_gridstate temp_food = get_food_state(c, i, j);
                set_food_state(c, i, j, temp_food + 1);
            }
        }
    }
    printf("A volcano eruption occurred!\n");
}

void end_event_volcano_eruption(Conway *c) {
    if (c->grid_feature.temperature > cold) {
        change_temperature(c, -2);
    } else if (c->grid_feature.temperature > freezing) {
        change_temperature(c, -1);
    }
    if (c->grid_feature.oxygen_resource < extreme_oxy) {
        change_oxygen(c, 1);
    }
    if (c->grid_feature.light_resource < extreme_light) {
        change_light(c, 1);
    }
}

// 3、干旱
void event_drought(Conway *c) {
    if (c->grid_feature.temperature < extreme_hot) {
        change_temperature(c, 1);
    }
    int delete_food = rand() % (c->rows * c->cols / 3) + (c->rows * c->cols / 10);
    for (int i = 0; i < delete_food; i += 0) {
        int limit_turn = c->rows * c->cols / 2;
        int count_turn = 0;
        int rand_row = rand() % c->rows;
        int rand_col = rand() % c->cols;
        int food_temp_state = get_food_state(c, rand_row, rand_col);
        if (food_temp_state > 0) {
            set_food_state(c, rand_row, rand_col, food_temp_state - 1);
            i += 1;
        }
        count_turn += 1;
        if (count_turn > limit_turn) {
            break;
        }
    }
    printf("Drought happened!\n");
}

void end_event_drought(Conway *c) {
    if (c->grid_feature.temperature > freezing) {
        change_temperature(c, -1);
    }
}

// 4、暖期
void event_warm_period(Conway *c) {
    if (c->grid_feature.temperature < extreme_hot) {
        change_temperature(c, 1);
    }
    printf("It is a warm period\n");
}

void end_event_warm_period(Conway *c) {
    if (c->grid_feature.temperature > freezing) {
        change_temperature(c, -1);
    }
}

// 5、蓝藻爆发
void event_blue_algal_bloom(Conway *c) {
    if (c->grid_feature.light_resource > dark) {
        change_light(c, -1);
    }
    if (c->grid_feature.oxygen_resource > rare_oxy) {
        change_oxygen(c, -1);
    } else if (c->grid_feature.oxygen_resource > low_oxy) {
        change_oxygen(c, -2);
    }
    int count = rand() % (c->rows * c->cols / 2);
    int rand_rows, rand_cols;
    cell temp_cell;
    for (int i = 0; i < (count); i += 0) { // 生成大量生产者
        int count_turn;
        rand_rows = rand() % c->rows;
        rand_cols = rand() % c->cols;
        temp_cell = get_state_special(c, rand_rows, rand_cols);
        if (temp_cell.type == Dead) {
            temp_cell = new_cell_setting();
            temp_cell.type = living_producer;
            set_state_special(c, rand_rows, rand_cols, temp_cell);
            i += 1; // 生成新的生产者
        }
        count_turn += 1;
        if (count_turn >= c->rows * c->cols) {
            break;
        }
    }
    printf("Blue algal bloom!\n");
}

void end_event_blue_algal_bloom(Conway *c) {
    if (c->grid_feature.light_resource < extreme_light) {
        change_light(c, 1);
    }
    if (c->grid_feature.oxygen_resource < extreme_oxy) {
        change_oxygen(c, 1);
    } else if (c->grid_feature.oxygen_resource < high_oxy) {
        change_oxygen(c, 2);
    }
}

// 6、热浪
void event_heat_wave(Conway *c) {
    if (c->grid_feature.temperature < hot) {
        change_temperature(c, 2);
    } else if (c->grid_feature.temperature < extreme_hot) {
        change_temperature(c, 1);
    }
    printf("A heat wave arrived!\n");
}

void end_event_heat_wave(Conway *c) {
    if (c->grid_feature.temperature > cold) {
        change_temperature(c, -2);
    } else if (c->grid_feature.temperature > freezing) {
        change_temperature(c, -1);
    }
}

// 其他
void set_normal_temperature(Conway *c) {
    c->grid_feature.temperature = normal_tem;
    printf("Temperature is now moderate!\n");
}

void set_normal_oxygen(Conway *c) {
    c->grid_feature.oxygen_resource = normal_oxy;
    printf("Oxygen set normal!\n");
}

void set_normal_light(Conway *c) {
    c->grid_feature.light_resource = normal_light;
    printf("light is now normal!\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 这个函数用于防止冲突的事件发生以及重复的事件发生
Bool test_event(Conway *c, event_list tested_event) {
    for (int i = 0; i < 10; i++) {
        if (c->currentEvent[i] == tested_event) {
            return True;
        }
    }
    return False;
}

// 函数用于添加event至数组中
// 突然发现大写字母代替_也很不错
void addEvent(Conway *c, event_list newEvent, int turns) {
    for (int i = 0; i < 10; ++i) {
        if (c->currentEvent[i] == none_event) { // 找到空位
            c->currentEvent[i] = newEvent;
            c->event_remain_turn[i] = turns;
            break;
        }
    }
}

// 用于更新一个世代的事件时长
void updateEventState(Conway *c) {
    for (int i = 0; i < 10; i++) {
        if (c->event_remain_turn[i] > 0) {
            c->event_remain_turn[i] -= 1;
        } else if (c->event_remain_turn[i] == 0) {
            switch (c->currentEvent[i]) {
            case ice_age: {
                end_event_ice_age(c);
                break;
            }
            case volcano_eruption: {
                end_event_volcano_eruption(c);
                break;
            }
            case drought: {
                end_event_drought(c);
                break;
            }
            case warm_period: {
                end_event_warm_period;
                break;
            }
            case blue_algal_bloom: {
                end_event_blue_algal_bloom(c);
                break;
            }
            case heat_wave: {
                end_event_heat_wave(c);
                break;
            }
            }
            c->currentEvent[i] = none_event;
        }
    }
}

// 用于随机发生事件
void event(Conway *c) {
    int random_change = rand() % 7;
    if (test_event(c, none_event) == False) { // 没有空位会阻止事件发生
        return;
    }
    switch (random_change) {
    case 0:
        if (test_event(c, ice_age) != True) {
            event_ice_age(c);
            addEvent(c, ice_age, 20);
        }
        break;
    case 1:
        if (test_event(c, volcano_eruption) != True) {
            event_volcano_eruption(c);
            addEvent(c, volcano_eruption, 10);
        }
        break;
    case 2:
        if (test_event(c, drought) != True) {
            event_drought(c);
            addEvent(c, drought, 20);
        }
        break;
    case 3:
        if (test_event(c, warm_period) != True) {
            event_warm_period(c);
            addEvent(c, warm_period, 50);
        }
        break;
    case 4:
        if (test_event(c, blue_algal_bloom) != True) {
            event_blue_algal_bloom(c);
            addEvent(c, blue_algal_bloom, 5);
        }
        break;
    case 5:
        if (test_event(c, heat_wave) != True) {
            event_heat_wave(c);
            addEvent(c, heat_wave, 15);
        }
        break;
    default:
        break;
    }
}

// 初始化Conway的event数组
void initialize_event(Conway *c) {
    for (int i = 0; i < 10; i++) {
        c->currentEvent[i] = none_event;
        c->event_remain_turn[i] = 0;
    }
}

// 需要一个日志系统来记录最新发生的事件并打印

void change_temperature(Conway *c, int change_number) {
    int temp_tem = (int)(c->grid_feature.temperature);
    temp_tem += change_number;
    temperature_state set_tem = (temperature_state)(temp_tem);
    c->grid_feature.temperature = set_tem;
}

void change_oxygen(Conway *c, int change_number) {
    int temp_oxy = (int)(c->grid_feature.oxygen_resource);
    temp_oxy += change_number;
    oxygen_level set_oxy = (oxygen_level)(temp_oxy);
    c->grid_feature.oxygen_resource = set_oxy;
}

void change_light(Conway *c, int change_number) {
    int temp_light = (int)(c->grid_feature.light_resource);
    temp_light += change_number;
    light_level set_light = (light_level)(temp_light);
    c->grid_feature.light_resource = set_light;
}

void change_gene(Conway *c, const int x, const int y) {
    if (x < 0 || y < 0 || x >= c->rows || y >= c->cols) {
        return;
    }
    int flag = rand() % 200;
    if (flag != 0) {
        return;
    }
    int choose_gene = rand() % 3;
    switch (choose_gene) {
    case 0: {
        // 温度基因
        int flag_temp = rand() % 5;
        int gene_change[5] = {0, 1, 2, 3, 4};
        c->special_grids[x][y].gene_setting.temper_preference = gene_change[flag_temp];
        break;
    }
    case 1: {
        // 氧气基因或光照基因
        int flag_temp = rand() % 5;
        int gene_change[5] = {0, 1, 2, 3, 4};
        if (get_state_special(c, x, y).type == living_producer) {
            c->special_grids[x][y].gene_setting.light_preference = gene_change[flag_temp];
        } else if (get_state_special(c, x, y).type == living_consumer) {
            c->special_grids[x][y].gene_setting.oxy_preference = gene_change[flag_temp];
        }
        break;
    }
    case 2: {
        // 年龄基因
        int flag_temp = rand() % 5;
        int gene_change[5] = {10, 15, 20, 25, 30};
        if (get_state_special(c, x, y).type == living_producer) {
            c->special_grids[x][y].gene_setting.age_length = gene_change[flag_temp] / 5;
        } else if (get_state_special(c, x, y).type == living_consumer) {
            c->special_grids[x][y].gene_setting.age_length = gene_change[flag_temp];
        }
        break;
    }
    }
}

int save_conway_special(const Conway *c, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        return -1; // 返回错误代码
    }
    int temp_rows = c->rows;
    int temp_cols = c->cols;
    fprintf(fp, "%d,%d\n", temp_rows, temp_cols); // 第一行的数据
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            GridState write_state = get_state_special(c, i, j).type;
            fprintf(fp, "%d,", write_state);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

// 从文件读取格点
// 失败则Conway._grids = NULL
Bool new_conway_from_file_special(Conway *c, const char *filename) {
    int temp_row, temp_col;
    FILE *fp = fopen(filename, "r");
    // printf("ok\n");
    if (fp == NULL) {
        return False;
    }
    if (fscanf(fp, "%d,%d", &temp_row, &temp_col) != 2) {
        fclose(fp);
        return False;
    }
    delete_special_grids(c);
    free(c);
    c = new_conway_special(temp_row, temp_col);

    fscanf(fp, "\n"); // 跳过第一行
    char scan_input;
    cell temp_cell = new_cell_setting();
    for (int i = 0; i < c->rows; i++) {
        for (int j = 0; j < c->cols; j++) {
            scan_input = fgetc(fp); // 逐个读取
            if (scan_input == '0') {
                temp_cell.type = Dead;
                set_state_special(c, i, j, temp_cell);
            } else if (scan_input == '2' || scan_input == '1') {
                temp_cell.type = living_producer;
                set_state_special(c, i, j, temp_cell);
            } else if (scan_input == '3') {
                temp_cell.type = living_consumer;
                set_state_special(c, i, j, temp_cell);
            } else if (scan_input == '\n' || scan_input == EOF) {
                break;
            } else if (scan_input == ',') {
                j -= 1; // 跳过","
            }
        }
        fscanf(fp, ",");  // 最后一个逗号
        fscanf(fp, "\n"); // 跳过换行符
        c->rows = temp_row;
        c->cols = temp_col;
    }
    fclose(fp);
    return True;
}
//////////////////////////////////////////////////////////////////////
// 以下为测试的图形处理界面
