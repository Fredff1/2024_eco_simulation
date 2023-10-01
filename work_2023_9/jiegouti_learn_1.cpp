#include <stdio.h>
#include<string.h>
struct node{
    int data;
    struct node*next;
};
void traverse(struct node* head) {
    struct node* current = head;  // 初始化一个指向头节点的指针
    while (current != NULL) {     // 当当前节点不是 NULL 时执行循环
        printf("%d ", (*current).data);  // 输出当前节点的数据
        current = current->next;   // 移动到下一个节点
    }
    printf("\n");
}

int main() {
    // 创建节点
    struct node n1, n2, n3;
    n1.data = 1;
    n2.data = 2;
    n3.data = 3;

    // 连接节点以形成单链表
    n1.next = &n2;
    n2.next = &n3;
    n3.next = NULL;

    // 遍历单链表并输出结果
    traverse(&n1);
    
    return 0;
}
