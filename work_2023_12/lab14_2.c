#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StuInfo {
    int ID;
    char name[100];
    int age;
    struct StuInfo *next;
};

typedef struct{
    struct StuInfo *head;
    struct StuInfo *tail;
    struct StuInfo *max_age;
}assist_node;

void find_max_age(assist_node *node){
    int temp_age=-1;
    struct StuInfo *temp = node->head->next;
    while (temp != NULL ) {
        if(temp->age>temp_age){
            temp_age=temp->age;
            node->max_age=temp;
        }
        temp = temp->next;
    }
}

struct StuInfo *create_info(int n_ID, const char *n_name, int n_age) {
    struct StuInfo *n_stu = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    n_stu->next = NULL;
    n_stu->age = n_age;
    n_stu->ID = n_ID;
    strcpy(n_stu->name, n_name);
    return n_stu;
}

// ID不输入则在末尾添加
void add_info(assist_node *node ,const int ID, const int n_id, const char *n_name, const int n_age) {
    struct StuInfo *temp = node->head->next, *n;
    while (temp != NULL && temp->ID != ID) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the student!\n");
        return;
    }
    n = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    //printf("Enter the ID,Name,age of the n student\n");
    // scanf("%d %s %d",&n->ID,n->name,&n->age);
    //测试就不给你们输入了,本来不作为参数进入函数，现在改成了在外面写好参数
    n->age = n_age;
    n->ID = n_id;
    strcpy(n->name, n_name);
    n->next = temp->next;
    temp->next = n;
    if(n->next==NULL){
        node->tail=n;
    }
    find_max_age(node);
}

void add_last_info(assist_node*node, const int n_id, const char *n_name, const int n_age) {
    struct StuInfo *temp = node->head->next, *n;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    n = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    //printf("Enter the ID,Name,age of the n student\n");
    // scanf("%d %s %d",&n->ID,n->name,&n->age);
    n->age = n_age;
    n->ID = n_id;
    strcpy(n->name, n_name);
    temp->next = n;
    n->next = NULL;
    node->tail=n;
    find_max_age(node);
}

void delete_info(assist_node*node, int ID) {
    
    struct StuInfo *temp = node->head->next, *prev = NULL;
    if (temp != NULL && temp->ID == ID) {
        node->head->next = temp->next; 
        free(temp);            
        printf("Delete ID %d\n", ID);
        return;
    }
    while (temp != NULL && temp->ID != ID) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the student!\n");
        return;
    }
    prev->next = temp->next;
    if(prev->next==NULL){
        node->tail=prev;
    }
    printf("Delete ID %d\n\n", ID);
    free(temp);
    find_max_age(node);
}

void change_info_name(assist_node*node, const int tar_ID, const char *n_name) {
    struct StuInfo *temp = node->head->next;
    while (temp != NULL && temp->ID != tar_ID) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the student!\n");
        return;
    }
    strcpy(temp->name, n_name);
    printf("Change ID:%d name into %s\n",temp->ID,temp->name);
}

void change_info_age(assist_node*node, const int tar_ID, const int n_age) {
    struct StuInfo *temp = node->head->next;
    while (temp != NULL && temp->ID != tar_ID) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the student!\n\n");
        return;
    }
    temp->age = n_age;
    printf("Change ID:%d age into %d\n",temp->ID,temp->age);
}

void travel_info(struct StuInfo *head, const int tar_ID) {
    struct StuInfo *temp = head->next;
    while (temp != NULL && temp->ID != tar_ID) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the student!\n");
        return;
    }
    printf("The student information\nID:%d\nName:%s\nAge:%d\n\n", temp->ID, temp->name, temp->age);
}

void print_all_info(struct StuInfo *head) {
    struct StuInfo *temp = head->next;
    printf("The student information:\n\n");
    while (temp != NULL) {
        printf("ID:%d\nName:%s\nAge:%d\n\n", temp->ID, temp->name, temp->age);
        temp = temp->next;
    }
    printf("end\n\n");
}


void print_max(assist_node *node){
    find_max_age(node);
    printf("The max age is %d,its ID:%d\n",node->max_age->age,node->max_age->ID);
}

int main() {
    struct StuInfo *new_list = create_info(1, "TianSuoHaoEr", 24);
    struct StuInfo *head=(struct StuInfo*)malloc(sizeof(struct StuInfo));
    struct StuInfo *tail=(struct StuInfo*)malloc(sizeof(struct StuInfo));
    struct StuInfo *max_age_node=(struct StuInfo*)malloc(sizeof(struct StuInfo));
    head->next=new_list;
    tail->next=new_list;
    max_age_node->next=new_list;
    assist_node assistant={head,tail,max_age_node};
    add_info(&assistant, 1, 2, "A", 11);
    add_info(&assistant, 10, 2, "B", 45);
    add_last_info(&assistant, 100, "C", 14);
    print_all_info(head);
    print_max(&assistant);
    delete_info(&assistant, 1);
    print_all_info(head);
    print_max(&assistant);
    change_info_age(&assistant, 100, 1919);
    travel_info(head, 100);
    print_all_info(head);
    print_max(&assistant);
    return 0;
}
