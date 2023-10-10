#include "conway.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// 构造新格点，分配内存
// malloc()
//这个功能已经有了，应该是用于主游戏把
void decide_new_conway(const uint16_t rows, const uint16_t cols) {
    

}

// 删除格点，回收分配给格点二维数组的内存
// Conway游戏本身的结构体 c 不用删除，只删除其格点
// 使用free()
void delete_grids(Conway *c) {

}

// 随机地初始化格点
void init_random(Conway *c) {
    
}

// 将系统演化到下一世代
void next_generation(Conway *c) {

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

}

void set_state(Conway *c, const uint16_t x, const uint16_t y, GridState s) {

}

// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state(const Conway *c, const uint16_t x, const uint16_t y) {

}

// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void show_conway(const Conway *c) {

}

// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *path) {

}

// 从文件读取格点
// 失败则Conway._grids = NULL
// 涉及malloc()
Conway new_conway_from_file(const char *path) {

}