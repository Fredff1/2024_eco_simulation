#include "conway.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

// 构造新格点，分配内存
// malloc()
Conway *new_conway(const uint16_t m, const uint16_t n) {
    Conway *c = (Conway*)malloc(sizeof(Conway));
    c->m = m;
    c->n = n;
    // 分配内存
    c->_grids = (GridState**)malloc(m * sizeof(GridState*));
    for (int i = 0; i < c->m; i++) {
        c->_grids[i] = (GridState*)malloc(m * sizeof(GridState));
    }
    if (c->_grids == NULL) {
        // 内存分配失败，进行错误处理
        free(c);
        return NULL; 
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            set_state(c, i, j, Dead);
        }
    }
    return c;
}

// 删除格点，回收分配给格点二维数组的内存
// Conway游戏本身的结构体 c 不用删除，只删除其格点
// 使用free()
void delete_grids(Conway *c) {
    if (c->m != 0 && c->n != 0 && c->_grids != NULL) {
        for (int i = 0; i < c->m; i++) {
            free(c->_grids[i]);
        }
        free(c->_grids);
        c->_grids = NULL;
    }
    c->m = 0;
    c->n = 0;
}

// 清除缓冲区
void clear_input() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF) {
        ;
    }
}

// 随机地初始化格点
void init_random(Conway *c) {
    int change_into_living;
    change_into_living = (c->m) * (c->n) * (c->possibility) / 100;
    for (int i = 0; i < change_into_living; ) {
        uint16_t rand_rows = rand() % c->m;
        uint16_t rand_cols = rand() % c->n;
        if (c->_grids[rand_rows][rand_cols] == Dead) {
            c->_grids[rand_rows][rand_cols] = Alive;
            i++;
        }        
    }
}    

bool is_valid_point(Point p, const Conway *c) {   //判断给定的点是否在网格范围内
    if (p.x >= 0 && p.x < c->m && p.y >= 0 && p.y < c->n) {
        return true;
    }
    else {
        return false;
    }
}

int count_neighbour(const Conway *c, const uint16_t x, const uint16_t y) {
    int living_neighbour = 0;
    if (x >= c->m && y >= c->n) { 
        return None;
    }
    GridState temp_grid[c->m][c->n];
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) { 
            temp_grid[i][j] = get_state(c, i, j);
        }
    }
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            Point neighbour_point = {i, j};
            if (i == x && j == y) {
                continue;
            }
            else if (is_valid_point(neighbour_point, c)) {
                if(temp_grid[i][j] == Alive || temp_grid[i][j] == Free) {
                    living_neighbour++;
                }
            }
        }
    }
    return living_neighbour;
}

// 将系统演化到下一世代
void next_generation(Conway *c) {
    GridState temp_grids[c->m][c->n];
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j <c->n; j++) {
            temp_grids[i][j] = get_next_state_cell(c, i, j);
        }
    }
    for (int i = 0; i< c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            set_state(c, i, j, temp_grids[i][j]);
        }
    }
}




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
GridState get_state(const Conway *c, const uint16_t x, const uint16_t y) {
    if (c == NULL) {
        return None;
    }
    if (x < c->m && y < c->n) {
        return c->_grids[x][y];
    }
    else {
        return None;
    }
}

void set_state(Conway *c, const uint16_t x, const uint16_t y, GridState s) {
    if (c == NULL) {
        return;
    }
    if (x < c->m && y < c->n) {
        c->_grids[x][y] = s;
        return;
    }
    else {
        return;
    }
}

// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state_cell(const Conway *c, const uint16_t x, const uint16_t y) {
    GridState next_state;
    int living_neighbour;
    if (x >= c->m && y >= c->n) {
        next_state = None;
    }
    living_neighbour = count_neighbour(c, x, y);
    if (living_neighbour < 2 && c->_grids[x][y] == Alive) {
        next_state = Dead;
    }
    else if ((living_neighbour == 2 || living_neighbour == 3) && c->_grids[x][y] == Alive) {
        next_state = Alive;
    }
    else if (living_neighbour > 3 && c->_grids[x][y] == Alive) {
        next_state = Dead;
    }
    else if (living_neighbour == 3 && c->_grids[x][y] == Dead) {
        next_state = Alive;
    }
    else {
        next_state = Dead;
    }
    return next_state;
}

// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void show_conway(const Conway *c) {
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            GridState state = get_state(c, i, j);
            if (state == Dead) {
                printf("| ");
            }
            else if (state == Alive) {
                printf("|*");
            }
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}

// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *path) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        printf("Unable to save\n");
        return -1;
    }
    int temp_rows = c->m;
    int temp_cols = c->n;
    fprintf(fp, "%d,%d\n", temp_rows, temp_cols); // 第一行的数据
    for (int i = 0; i < c->m; i++) {
        int j;
        for (j = 0; j < c->n - 1; j++) {
            GridState write_state = get_state(c, i, j);
            fprintf(fp, "%d,", write_state);
        }
        if (j == c->n - 1) {
            GridState write_state = get_state(c, i, j);
            fprintf(fp, "%d", write_state);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

//从文件读取格点
Conway *new_conway_from_file(const char *path) {
    Conway *update_conway = (Conway*)malloc(sizeof(Conway));
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Unable to open the file\n");
        return NULL;
    }

    int rows, cols;
    fscanf(fp, "%d %d", &rows, &cols);
    update_conway = new_conway(rows, cols);
    GridState new_grid[update_conway->m][update_conway->n];
    char input_state;

    char buffer[128];
    fgets(buffer, sizeof(buffer), fp);  // 跳过第一行
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            input_state = fgetc(fp);  //逐个读取字符
            if (input_state == '0') {
                new_grid[i][j] = Dead;
            }
            else if (input_state == '1') {
                new_grid[i][j] = Alive;
            }
            else if (input_state == '\n' || input_state == EOF) {
                break;
            }
            else if (input_state == ',') {
                continue;
            }
        }
        fscanf(fp, "%*c");  // 跳过行末换行符
    }
    fclose(fp);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set_state(update_conway, i, j, new_grid[i][j]);
        }
    }
    return update_conway;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//以下是特殊模式

//玩家作为上帝，可随机生成或消灭细胞
GridState turn_state(Conway *c, const uint16_t x, const uint16_t y) {
    GridState temp_grid = get_state(c, x, y);
    Point current_point = {(int)x, (int)y};
    if (!is_valid_point(current_point, c)) {
        temp_grid = None;
    }
    if (c->_grids[x][y] == Dead) {
        temp_grid = Alive;
    }
    else if (c->_grids[x][y] == Alive || c->_grids[x][y] == Free || c->_grids[x][y] == Infected) {
        temp_grid = Dead;
    }
    return temp_grid;
}

//每回合都有概率会在一个空格点随机生成病毒
//若不存在空格点，则不会产生病毒
//存活的细胞越多，生成病毒的概率越小
//病毒不会随着回合的刷新而消亡
//场上最多有一个病毒
void set_virus(Conway *c) {
    int flag = 0;
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == Dead) {
                return;
            }
        }
    }

    uint16_t rand_row = rand() % c->m;
    uint16_t rand_col = rand() % c->n;
    if (c->_grids[rand_row][rand_col] == Dead) {
        c->_grids[rand_row][rand_col] = Infected;
        printf("Virus in row %d col %d", rand_row, rand_col);
        return;
    }
    return;   
}
    
//同样地，每回合也有概率生成食物
//场上最多有一个食物
void set_food(Conway *c) {
    for (int i = 0; i < c->m; i++) { 
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == Food) {
                return;
            }
        }
    }

    uint16_t rand_row = rand() % c->m;
    uint16_t rand_col = rand() % c->n;
    if (c->_grids[rand_row][rand_col] == Dead) {     //只有当随机到空格点时，才会生成食物
        c->_grids[rand_row][rand_col] = Food;
        printf("Sausage in row %d col %d", rand_row, rand_col);
        return;
    }
    
    printf("No empty space!");
    return;
}

//若病毒周围3*3区域内存在细胞
//那么病毒就会随机杀死其中一个细胞
void virus_infect_cell(const Conway *c, const uint16_t x, const uint16_t y) {
    Point P = {x, y};
    if(!is_valid_point(P, c)) {
        return;                  //判断是否越界
    }

    if (c->_grids[x][y] != Infected) {    //判断该格点是不是病毒
        return;
    }
    else {
        int living_neighbour = count_neighbour(c, x, y);   //判断周围有没有细胞
        if (!living_neighbour) {
            return;
        }
        else {
            Point cell[8];     //记录细胞的位置
            int count_cell = 0;
            for (int i = x - 1; i <= x + 1; i++) {
                for (int j = y - 1; j <= y + 1; j++) {
                    Point p = {i, j};
                    if (!is_valid_point || (i == x && j == y)) {
                        continue;
                    }
                    else {
                        if(c->_grids[i][j] == Alive) {
                            cell[count_cell].x = i;
                            cell[count_cell].y = j;
                            count_cell++;
                        }
                    }
                }
            }

            uint16_t rand_cell = rand() % count_cell;   //随机杀死一个细胞
            c->_grids[cell[rand_cell].x][cell[rand_cell].y] = Dead;
        }
    }
}

//细胞也能够杀死病毒
//细胞杀死病毒的条件:一个病毒的上下左右四个格点均被细胞占领
GridState cell_kill_virus(const Conway *c, const uint16_t x, const uint16_t y) {
    Point P = {x, y};
    if(!is_valid_point(P, c)) {
        return None;                        //判断是否越界
    } 

    GridState temp_grid = get_state(c, x, y);
    if (temp_grid != Infected) {         //判断该格点是否为病毒
        return temp_grid;
    }
    
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            Point p = {i, j};
            if (fabs(x - i)+fabs(y - j) != 1 || !is_valid_point(p, c)) {   //若格点不在细胞的上下左右，则跳过
                continue;
            }
            if (c->_grids[x][y] != Alive && c->_grids[x][y] != Free) {     //若有一个格点未被细胞占领，则退出函数
                return temp_grid;
            }
        }
    }
    
    return Dead;     //否则满足条件，病毒被杀死
}


//病毒和细胞需要抢占食物
//病毒会主动搜索食物的位置并向食物靠近
//细胞需要依靠繁殖靠近食物
//当细胞与病毒在同一格中，病毒会吞噬细胞
//病毒会主动搜寻食物
void search_food(Conway *c) {
    Point virus_location;    //记录病毒的位置
    Point food_location;     //记录食物的位置
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == Infected) {
                virus_location.x = i;
                virus_location.y = j;
            }
            else if (c->_grids[i][j] == Food) {
                food_location.x = i;
                food_location.y = j;
            }
        }
    }

    Point closest_food_point;
    double min_distance = 0;
    for (int j = -1; j <= 1; j++) {         //遍历细胞周围的格点
        for (int k = -1; k <= 1; k++) {
            if (j == 0 && k == 0) {
                continue;
            }
            Point next_virus = {virus_location.x + j, virus_location.y + k};
            if (is_valid_point(next_virus, c)) {
                double distance_to_food = distance(next_virus, food_location);
                if (distance_to_food < min_distance) {        //判断到食物距离最小的格点
                    min_distance = distance_to_food;
                    closest_food_point = next_virus;
                }
            }
        }
    }
    c->_grids[closest_food_point.x][closest_food_point.y] = Infected;       //此时Free格点也会被杀死
    return;
}
//以下为search_food函数修改前的版本
/*    for(int i = 0; i < count_virus; i++) {
        if(row_virus[i] > row_food && col_virus[i] > col_food) {
            c->_grids[row_virus[i] - 1][col_virus[i] - 1] = Infected;
        }
        else if(row_virus[i] > row_food && col_virus[i] < col_food) {
            c->_grids[row_virus[i] - 1][col_virus[i] + 1] = Infected;
        }
        else if(row_virus[i] < row_food && col_virus[i] > col_food) {
            c->_grids[row_virus[i] + 1][col_virus[i] - 1] = Infected;
        }
        else if(row_virus[i] < row_food && col_virus[i] < col_food) {
            c->_grids[row_virus[i] + 1][col_virus[i] + 1] = Infected;
        }
        else if(row_virus[i] > row_food && col_virus[i] == col_food) {
            c->_grids[row_virus[i] - 1][col_virus[i]] = Infected;
        }
        else if(row_virus[i] < row_food && col_virus[i] == col_food) {
            c->_grids[row_virus[i] + 1][col_virus[i]] = Infected;
        }
        else if(row_virus[i] == row_food && col_virus[i] > col_food) {
            c->_grids[row_virus[i]][col_virus[i] - 1] = Infected;
        }
        else if(row_virus[i] == row_food && col_virus[i] < col_food) {
            c->_grids[row_virus[i]][col_virus[i] + 1] = Infected;
        }
    }
    
    int count = 0;         
    int min_row[5];
    int min_col[5];
    int row_flag[9];
    int col_flag[9];
    int distance_calculate[5][9] = {0};
    for(int i = 0; i < count_virus; i++){
        for(int row = row_virus[i] - 1; row <= row_virus[i] + 1; row++) {
            for(int col = col_virus[i] - 1; col <= col_virus[i] + 1; col++) {
                if(row >= 0 && row < c->m && col >= 0 && col < c->n){
                    distance_calculate[i][count] = (row_food - row)*(row_food - row) + (col_food - col) * (col_food - col);
                    count++;
                    row_flag[count] = row;
                    col_flag[count] = col;
                }
            }
        }
        int min_distance = distance_calculate[i][0];
        for(int j = 0; j < count; j++) {
            if(distance_calculate[i][j] < min_distance) {
                min_distance = distance_calculate[i][j];
                min_row[i] = row_flag[j];
                min_col[i] = col_flag[j];
            }
        }
        c->_grids[min_row[i]][min_col[i]] = Infected;
    }*/


double distance(Point p1, Point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}


//演化到下一个世代
GridState get_next_state_special(Conway *c, const uint16_t x, const uint16_t y) {
    GridState next_state_special = get_next_state_cell(c, x, y);  // 获取细胞的下一个状态  
    // 处理食物效果
    next_state_special = (c, x, y);  
    // 在空格点设置食物
    set_food(c); 
    // 设置病毒
    set_virus(c); 
    // 搜索食物
    search_food(c);
    // 获取最终的状态
    next_state_special = get_state(c, x, y);  
    return next_state_special;
}

//食物效果；
//若病毒抢占了食物，就会杀死周围3*3的细胞
//反之，若细胞抢占了食物，将食物格点置为Free
//细胞抢占食物条件：食物的上下左右有被细胞包围
//Free作用：不受Conway规则的影响，也就是说该格点一直会有细胞生存
void food_effect(Conway *c, const uint16_t x, const uint16_t y) {
    if (c->_grids[x][y] == Food) {
        if (c->_grids[x - 1][y] == Infected || c->_grids[x + 1][y] == Infected       //先判断周围是否有病毒
        ||  c->_grids[x][y - 1] == Infected || c->_grids[x][y + 1] == Infected) {    //原因：病毒有自主移动的能力，而细胞没有
            for (int i = x - 1; i <= x + 1; i++) {
                for (int j = y - 1; j <= y + 1; j++) {
                    Point food_point = {i, j};
                    if (is_valid_point(food_point, c)) {
                        c->_grids[i][j] = Dead;
                    }
                }
            }
            c->_grids[x][y] = Infected;
            return;
        }

        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                Point p = {x, y};
                if (fabs(x - i)+fabs(y - j) != 1 || !is_valid_point(p, c)) {
                    continue;
                }
                if (c->_grids[x][y] != Alive && c->_grids[x][y] != Free) {     //若有一个格点未被细胞占领，则退出函数
                    return;
                }
            }
        }
    }
    return;
}

//打印格点
void show_conway_special(const Conway *c) {
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == Dead) {
                printf("| ");
            }
            else if (c->_grids[i][j] == Alive) {
                printf("|*");
            }
            else if (c->_grids[i][j] == Food) {
                printf("|$"); 
            }
            else if (c->_grids[i][j] == Infected) {
                printf("|!");
            }
            else if (c->_grids[i][j] == Free) {
                printf("|^");
            }
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}

//日志部分
void log_message(char* message, char* level) {
    time_t current_time = time(NULL);        //获取当前时间戳
    char* timestamp = ctime(&current_time);  //转换为日期时间字符串
    timestamp[strlen(timestamp) - 1] = '\0'; // 删除换行符

    FILE* log_file = fopen(LOG_FILE, "a");   //以追加方式写入日志内容
    if (log_file) {                          //判断是否成功打开日志文件
        fprintf(log_file, "[%s] [%s] %s\n", timestamp, level, message);
        fclose(log_file);
    } else {
        printf("Failed to write log to file!\n");
    }
}

//从文件获取格点
Conway *new_conway_from_file_special(const char *path) {
    Conway *update_conway = (Conway*)malloc(sizeof(Conway));
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Unable to open the file\n");
        return NULL;
    }

    int rows, cols;
    fscanf(fp, "%d %d", &rows, &cols);
    update_conway = new_conway(rows, cols);
    GridState new_grid[update_conway->m][update_conway->n];
    char input_state;

    char buffer[128];
    fgets(buffer, sizeof(buffer), fp);  // 跳过第一行
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            input_state = fgetc(fp);  //逐个读取字符
            if (input_state == '0') {
                new_grid[i][j] = Dead;
            }
            else if (input_state == '1') {
                new_grid[i][j] = Alive;
            }
            else if (input_state == '2') {
                new_grid[i][j] = Infected;
            }
            else if (input_state == '3') {
                new_grid[i][j] == Food;
            }
            else if (input_state == '4') {
                new_grid[i][j] == Free;
            }
            else if (input_state == '\n' || input_state == EOF) {
                break;
            }
            else if (input_state == ',') {
                continue;
            }
        }
        fscanf(fp, "%*c");  // 跳过行末换行符
    }
    fclose(fp);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            set_state(update_conway, i, j, new_grid[i][j]);
        }
    }
    return update_conway;
}