#include "conway.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 构造新格点，分配内存
// 创建失败则返回NULL
// malloc()
Conway new_conway(const uint16_t m, const uint16_t n) {
    Conway myConway;
    myConway.m = m;
    myConway.n = n;
    GridState **_initGrids = (GridState **)malloc(m * sizeof(GridState *));
    for (int i = 0; i < m; i++) {
        _initGrids[i] = (GridState *)malloc(n * sizeof(GridState));
        for (int j = 0; j < n; j++) {
            _initGrids[i][j] = STATE_NONE;
        }
    }
    myConway._grids = _initGrids;
    return myConway;
}

// 删除格点，回收内存
// free()
void delete_grids(Conway *c) {
    if (c != NULL) {
        if (c->_grids != NULL) {
            for (int i = 0; i < c->m; i++) {
                free(c->_grids[i]);
                free(c->_grids[i]);
            }
            free(c->_grids);
        }
        free(c);
    }
}

// 随机地初始化格点
void init_random(Conway *c) {
    for (uint16_t i = 0; i < c->m; i++) {
        for (uint16_t j = 0; j < c->n; j++) {
            set_state(c, i, j, (GridState)rand() % 2);
        }
    }
}

// 将系统演化到下一世代
void next_generation(Conway *c) {
    GridState **_tempGrids = (GridState **)malloc(sizeof(GridState *) * c->m);
    if (_tempGrids == NULL) {
        fprintf(stderr, "Memory allocation failed(6).\n");
    }
    for (int i = 0; i < c->m; i++) {
        _tempGrids[i] = (GridState *)malloc(sizeof(GridState) * c->n);
        if (_tempGrids[i] == NULL) {
            fprintf(stderr, "Memory allocation failed(7).\n");
        }
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            _tempGrids[i][j] = get_next_state(c, i, j);
        }
    }
    for (int i = 0; i < c->m; i++) {
        free(c->_grids[i]);
    }
    free(c->_grids);
    c->_grids = _tempGrids;
}

void next_generation_food(Conway *c) {
    GridState **_tempGrids, *stateStorage, **cellStorage, count = 0;
    stateStorage = (GridState *)calloc(3, sizeof(GridState));
    _tempGrids = (GridState **)calloc(c->m, sizeof(GridState *));
    cellStorage = (GridState **)calloc(2, sizeof(GridState *));
    if (_tempGrids == NULL || stateStorage == NULL || cellStorage == NULL) {
        fprintf(stderr, "Memory allocation failed(3).\n");
    }
    for (int i = 0; i < c->m; i++) {
        _tempGrids[i] = (GridState *)calloc(c->n, sizeof(GridState));
        if (_tempGrids[i] == NULL) {
            fprintf(stderr, "Memory allocation failed(4).\n");
        }
    }
    for (int i = 0; i < c->m / 2; i++) {
        cellStorage[i] = (GridState *)calloc(c->m / 2, sizeof(GridState));
        if (cellStorage[i] == NULL) {
            fprintf(stderr, "Memory allocation failed(5).\n");
        }
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == STATE_FOOD) {
                stateStorage = food_evolution(c, i, j);
                _tempGrids[i][j] = stateStorage[2];
                cellStorage[0][count] = stateStorage[0];
                cellStorage[1][count] = stateStorage[1];
                count++;
            }
        }
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (get_state(c, i, j) != STATE_FOOD) {
                if (get_state(c, i, j) == STATE_COMPETE) {
                    c->_grids[i][j] = STATE_ALIVE;
                }
            }
        }
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] != STATE_FOOD) {
                int found = 0;
                for (int o = 0; o < count; o++) {
                    if (i == cellStorage[0][o] && j == cellStorage[1][o]) {
                        found = 1;
                    }
                }
                if (!found) {
                    _tempGrids[i][j] = get_next_state(c, i, j);
                } else {
                    _tempGrids[i][j] = STATE_DEAD;
                }
            }
        }
    }
    for (int i = 0; i < c->m; i++) {
        free(c->_grids[i]);
    }
    free(c->_grids);
    c->_grids = _tempGrids;
    /*free(stateStorage);
    for (int i = 0; i < 2; i++)
    {
        free(cellStorage[i]);
    }
    free(cellStorage);*/
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
    if (x < 0 || x >= c->m || y < 0 || y >= c->n) {
        return STATE_NONE;
    }
    return c->_grids[x][y];
}

void set_state(Conway *c, const uint16_t x, const uint16_t y, GridState s) { c->_grids[x][y] = s; }

// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state(const Conway *c, const uint16_t x, const uint16_t y) {
    int neighbor = 0, neighborX, neighborY;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            } else {
                neighborX = x + i, neighborY = y + j;
                if (get_state(c, neighborX, neighborY) == STATE_ALIVE || get_state(c, neighborX, neighborY) == STATE_SUPER ||
                    get_state(c, x, y) == STATE_COMPETE || get_state(c, x, y) == STATE_MOVE) {
                    neighbor++;
                }
            }
        }
    }
    if (get_state(c, x, y) == STATE_ALIVE || get_state(c, x, y) == STATE_COMPETE) {
        if (neighbor == 2 || neighbor == 3) {
            return STATE_ALIVE;
        } else {
            return STATE_DEAD;
        }
    } else if (get_state(c, x, y) == STATE_DEAD) {
        if (neighbor == 3) {
            return STATE_ALIVE;
        } else {
            return STATE_DEAD;
        }
    } else if (get_state(c, x, y) == STATE_SUPER) {
        return STATE_ALIVE;
    }
}

void food_initialize(Conway *c) {
    GridState **_foodGrids;
    _foodGrids = (GridState **)calloc(c->m, sizeof(GridState *));
    if (_foodGrids == NULL) {
        fprintf(stderr, "Memory allocation failed(1).\n");
    }
    for (int i = 0; i < c->m; i++) {
        _foodGrids[i] = (GridState *)calloc(c->n, sizeof(GridState));
        if (_foodGrids[i] == NULL) {
            fprintf(stderr, "Memory allocation failed(2).!\n", i);
        }
    }
    int record[2][c->m / 2];
    int row, col, count = 0;
    for (int j = 0; j < c->m / 2; j++) {
        row = rand() % (c->m);
        col = rand() % (c->n);
        record[0][count] = row;
        record[1][count] = col;
        count++;
    }
    for (int i = 0; i < count; i++) {
        _foodGrids[record[0][i]][record[1][i]] = STATE_FOOD;
    }
    for (int i = 0; i < count; i++) {
        c->_grids[record[0][i]][record[1][i]] += _foodGrids[record[0][i]][record[1][i]];
    }
    for (int i = 0; i < c->m; i++) {
        free(_foodGrids[i]);
    }
    free(_foodGrids);
}

void compete_setting(Conway *c) {
    GridState **_competeGrids, count = 0;
    _competeGrids = (GridState **)calloc(c->m, sizeof(GridState *));
    if (_competeGrids == NULL) {
        fprintf(stderr, "Memory allocation failed(8).\n");
    }
    for (int i = 0; i < c->m; i++) {
        _competeGrids[i] = (GridState *)calloc(c->n, sizeof(GridState *));
        if (_competeGrids[i] == NULL) {
            fprintf(stderr, "Memory allocation failed(9).\n");
        }
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] != STATE_FOOD) {
                _competeGrids[i][j] = c->_grids[i][j];
            }
        }
    }
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == STATE_FOOD) {
                for (int o = -1; o <= 1; o++) {
                    for (int p = -1; p <= 1; p++) {
                        int foodNeighborX = i + o, foodNeighborY = j + p;
                        if (foodNeighborX == i && foodNeighborY == j) {
                            _competeGrids[i][j] = STATE_FOOD;
                        } else if (get_state(c, foodNeighborX, foodNeighborY) == STATE_ALIVE &&
                                   get_next_state(c, foodNeighborX, foodNeighborY) == STATE_ALIVE) {
                            int posibility = rand() % 2;
                            if (posibility == 1) {
                                _competeGrids[foodNeighborX][foodNeighborY] = STATE_COMPETE;
                            } else {
                                _competeGrids[foodNeighborX][foodNeighborY] = STATE_ALIVE;
                            }
                        } else if (get_state(c, foodNeighborX, foodNeighborY) == STATE_ALIVE &&
                                   get_next_state(c, foodNeighborX, foodNeighborY) == STATE_DEAD) {
                            _competeGrids[foodNeighborX][foodNeighborY] = STATE_COMPETE;
                        } else if (get_state(c, foodNeighborX, foodNeighborY) == STATE_COMPETE) {
                            _competeGrids[foodNeighborX][foodNeighborY] = STATE_COMPETE;
                        } else if (get_state(c, foodNeighborX, foodNeighborY) == STATE_DEAD) {
                            _competeGrids[foodNeighborX][foodNeighborY] = STATE_DEAD;
                        } else if (get_state(c, foodNeighborX, foodNeighborY) == STATE_SUPER) {
                            _competeGrids[foodNeighborX][foodNeighborY] = STATE_SUPER;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < c->m; i++) {
        free(c->_grids[i]);
    }
    free(c->_grids);
    c->_grids = _competeGrids;
    // free(_competeGrids);
}

GridState *food_evolution(const Conway *c, const uint16_t x, const uint16_t y) {
    int count = 0, xy[2][8];
    GridState *x_y_foodState = (GridState *)malloc(3 * sizeof(GridState)); // 你忘记强制内存转化了
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            } else if (get_state(c, x + i, y + j) == STATE_COMPETE) {
                xy[0][count] = x + i;
                xy[1][count] = y + j;
                count++;
            }
        }
    }
    if (count != 0) {
        int luckyNumber = rand() % count;
        x_y_foodState[0] = xy[0][luckyNumber];
        x_y_foodState[1] = xy[1][luckyNumber];
        x_y_foodState[2] = STATE_SUPER;
    } else {
        x_y_foodState[0] = STATE_NONE;
        x_y_foodState[1] = STATE_NONE;
        x_y_foodState[2] = STATE_FOOD;
    }
    return x_y_foodState;
}

void delete_food(Conway *c) {
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (c->_grids[i][j] == STATE_FOOD) {
                c->_grids[i][j] = STATE_DEAD;
            }
        }
    }
}
// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void show_conway(const Conway *c) {
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (get_state(c, i, j) == STATE_ALIVE || get_state(c, i, j) == STATE_COMPETE) {
                printf("|");
                printf("\x1b[32m*\x1b[0m");
            } else if (get_state(c, i, j) == STATE_DEAD) {
                printf("| ");
            } else if (get_state(c, i, j) == STATE_FOOD) {
                printf("|");
                printf("\x1b[31m+\x1b[0m");
            } else if (get_state(c, i, j) == STATE_SUPER) {
                printf("|");
                printf("\x1b[33m*\x1b[0m");
            } else {
                printf("| ");
            }
        }
        printf("|\n");
    }
}

// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Error saving.\n");
        return -1;
    }
    fprintf(file, "%hu,%hu\n", c->m, c->n);
    for (int i = 0; i < c->m; i++) {
        for (int j = 0; j < c->n; j++) {
            if (get_state(c, i, j) == STATE_SUPER) {
                fprintf(file, "3,");
            } else if (get_state(c, i, j) == STATE_FOOD) {
                fprintf(file, "2,");
            } else if (get_state(c, i, j) == STATE_ALIVE || get_state(c, i, j) == STATE_COMPETE) {
                fprintf(file, "1,");
            } else {
                fprintf(file, "0,");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return 0;
}

// 从文件读取格点
// 失败则Conway._grids = NULL
// 涉及malloc()

Conway new_conway_from_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    int m, n;
    if (fscanf(file, "%d,%d\n", &m, &n) != 2) {
        printf("Error reading dimensions from file!\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    Conway c;
    char ch;
    c.m = m;
    c.n = n;
    GridState **_grids = (GridState **)malloc(m * sizeof(GridState *));
    for (int i = 0; i < m; i++) {
        _grids[i] = (GridState *)malloc(n * sizeof(GridState));
        for (int j = 0; j < 2 * n; j++) {
            if (j % 2 == 0) {
                fscanf(file, "%d", &_grids[i][j / 2]);
            } else {
                fscanf(file, "%c", &ch);
            }
        }
        fscanf(file, "\n");
    }
    c._grids = _grids;
    fclose(file);
    return c;
}