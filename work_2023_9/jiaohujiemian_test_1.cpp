#include <graphics.h> // 引入 EasyX 图形库头文件

int main()
{
    // 初始化图形模式
    initgraph(640, 480);

    // 设置画笔颜色为红色
    setcolor(RED);

    // 在 (320, 240) 坐标处绘制一个半径为 100 的圆
    circle(320, 240, 100);

    // 暂停，等待用户按键
    _getch();

    // 关闭图形窗口
    closegraph();

    return 0;
}
