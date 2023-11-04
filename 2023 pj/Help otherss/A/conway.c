#include "conway.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// 构造新格点，分配内存
// malloc()
Conway  new_conway(const uint16_t m, const uint16_t n)
{
    Conway c;
   
    c.m=m;
    c.n=n;
    GridState **_grid = (GridState **)malloc(m * sizeof(GridState *));
    for (int i = 0; i < m; i++)
    {
        _grid[i] = (GridState *)malloc(n * sizeof(GridState));
        for (int j = 0; j < n; j++)
        {
            _grid[i][j]=STATE_NONE;
        }
    }
    c._grids=_grid;
    return c;
}

// 删除格点，回收分配给格点二维数组的内存
// Conway游戏本身的结构体 c 不用删除，只删除其格点
// 使用free()
void delete_grids(Conway *c)
{
    if (c != NULL)
    {
        for (int i = 0; i < c->m; i++)
        {
            free(c->_grids[i]);
        }
        free(c->_grids);
    }
}

// 随机地初始化格点
void init_random(Conway *c)
{
    for (int i = 0; i < c->m; i++)
    {
        for (int j = 0; j < c->n; j++)
        {
            set_state(c, i, j, (GridState)rand() % 2);
        }
    }
}

// 将系统演化到下一世代
void next_generation(Conway *c)
{
    GridState **new_grid = (GridState **)malloc(c->m * sizeof(GridState *));
    for (int i = 0; i < c->m; i++)
    {
        new_grid[i] = (GridState *)malloc(c->n * sizeof(GridState));
    }
    // 基于当前状态计算下一个状态并存储在新数组中
    for (int i = 0; i < c->m; i++)
    {
        for (int j = 0; j < c->n; j++)
        {
            new_grid[i][j] = get_next_state(c, i, j);
        }
    }
    // 释放旧的格点数组
    for (int i = 0; i < c->m; i++)
    {
        free(c->_grids[i]);
    }
    free(c->_grids);

    c->_grids = new_grid;
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
GridState get_state(const Conway *c, const uint16_t x, const uint16_t y)
{

    if (!(x < c->m) || !(x >= 0) || !(y >= 0) || !(y < c->n))return STATE_NONE;
    return c->_grids[x][y];

}

void set_state(Conway *c, const uint16_t x, const uint16_t y, GridState s)
{

    if (!(x < c->m) || !(x >= 0) || !(y >= 0) || !(y < c->n))return;
    c->_grids[x][y] = s;
}

// 获取格点下一个世代的状态
// 注意下标边界检查
// 0 <= x < m,
// 0 <= y < n,
GridState get_next_state(const Conway *c, const uint16_t x, const uint16_t y)
{
    int nei = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int neix = (x + i ) , neiy = (y + j );
            if (i==0 && j == 0)
            {
                continue;
            }
            else
            {
                if (get_state(c, neix, neiy) == STATE_ALIVE)
                {
                    nei++;
                }
            }
        }
    }
    if (get_state(c, x, y) == STATE_ALIVE)
    {
        if (nei < 2 || nei > 3)
            return STATE_DEAD;
        else
            return STATE_ALIVE;
    }
    else if(get_state(c, x, y) == STATE_DEAD)
    {
        if (nei == 3)
            return STATE_ALIVE;
        else
            return STATE_DEAD;
    }
}

// 展示格点，一般来说是printf打印吧
// 不过长和宽设置的都是uint16_t类型，稍微大一点的格点就不好打印了
void show_conway(const Conway *c)
{
    for (int i = 0; i < c->m; i++)
    {
        for (int j = 0; j < c->n; j++)
        {
            if (get_state(c, i, j) == STATE_ALIVE)
                printf("|*");
            else
                printf("| ");
        }
        printf("|\n");
    }
}

// 保存格点到文件（可能得考虑一下数据保存到文件的格式）
// 成功则返回0，失败返回非0值
int save_conway(const Conway *c, const char *path)
{
    FILE *fp= fopen(path, "w");
    if (fp==NULL)
    {
        return 1;
    }
    
    fprintf(fp,"%d,%d\n",c->m,c->n);

    //同理使用get state和set state函数更规范
    for (int i = 0; i < c->m; i++)
    {
        for (int j = 0; j < c->n; j++)
        {
            fprintf(fp,"%d,",c->_grids[i][j]);
        }
        fprintf(fp,"\n");
    }
    return 0;
}

// 从文件读取格点
// 失败则Conway._grids = NULL
// 涉及malloc()
Conway new_conway_from_file(const char *path)
{
    FILE *fp= fopen(path, "r");
    //这里也要有错误处理if(fp==NULL){
        //return NULL;
   // }
    int m,n;
    Conway c;
    char fw;
    //注意在这里如果读取失败最好也加个错误处理，比如
    //if(fscanf(fp."%d,%d\n",&m,&n)!=2){
        //return NULL;
    //}
    //如果没有读取到两个有效的整数就直接返回
    fscanf(fp,"%d,%d\n",&m,&n);
    GridState **_grid = (GridState **)malloc(m * sizeof(GridState *));
    for (int i = 0; i < m; i++)
    {
        _grid[i] = (GridState *)malloc(n * sizeof(GridState));
        //这里最好用一个暂时的变量储存得到的值，然后用set state函数，当然在这只是规范性问题，没有实际影响
        for(int j=0;j<2*n;j++){
            if (j%2==0)
            {
                fscanf(fp,"%d",&_grid[i][j/2]);
            }
            else{
                fscanf(fp,"%c",&fw);
            }
        }
        fscanf(fp,"\n");
    }
    c.m=m;
    c.n=n;
    c._grids=_grid;
    fclose(fp);
    return c;
    //return了函数就不执行了,要写到上面
    //fclose(fp);
}