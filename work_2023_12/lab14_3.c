#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//为什么要改成日历...我改名称很麻烦的啊...
struct StuInfo {
    char date[30];
    char name[100];
    int duration;
    struct StuInfo *next;
    struct StuInfo *previous;
};

typedef struct{
    struct StuInfo *head;
    struct StuInfo *tail;
    struct StuInfo *max_name;
}assist_node;

void find_max_duration(assist_node *node){
    int temp_duration=-1;
    struct StuInfo *temp = node->head->next;
    while (temp != NULL ) {
        if(temp->duration>temp_duration){
            temp_duration=temp->duration;
            node->max_name=temp;
        }
        temp = temp->next;
    }
}

struct StuInfo *create_info(const char* n_date, const char *n_name, int n_duration) {
    struct StuInfo *n_stu = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    n_stu->next = NULL;
    n_stu->previous=NULL;
    strcpy(n_stu->name, n_name);
    n_stu->duration=n_duration;
    strcpy(n_stu->date,n_date);
    return n_stu;
}

// ID不输入则在末尾添加
void add_info(assist_node *node ,const char* tar_name, const char* n_date, const char *n_name, const int n_duration) {
    struct StuInfo *temp = node->head->next, *n;
    while (temp != NULL && strcmp(temp->name,tar_name)!=0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the event!\n");
        return;
    }
    n = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    //printf("Enter the ID,Name,duration of the n student\n");
    // scanf("%d %s %d",&n->ID,n->name,&n->duration);
    n->duration=n_duration;
    strcpy(n->date,n_date);
    strcpy(n->name, n_name);
    n->next = temp->next;
    n->previous=temp;
    temp->next = n;
    if(n->next==NULL){
        node->tail=n;
    }
    find_max_duration(node);
}

void add_last_info(assist_node*node, const char* n_date, const char *n_name, const int n_duration) {
    struct StuInfo *temp = node->head->next, *n;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    n = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    //printf("Enter the ID,Name,duration of the n student\n");
    // scanf("%d %s %d",&n->ID,n->name,&n->duration);
    n->duration=n_duration;
    strcpy(n->date,n_date);
    strcpy(n->name, n_name);
    temp->next = n;
    n->next = NULL;
    n->previous=temp;
    node->tail=n;
    find_max_duration(node);
}

void delete_info(assist_node*node, const char*name) {
    
    struct StuInfo *temp = node->head->next, *prev = NULL;
    if (temp != NULL && strcmp(temp->name,name)==0) {
        node->head->next = temp->next; 
        free(temp);            
        printf("Delete event %s\n", name);
        return;
    }
    while (temp != NULL && strcmp(temp->name,name)!=0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the event! \n");
        return;
    }
    prev->next = temp->next;
    if(prev->next==NULL){
        node->tail=prev;
    }
    printf("Delete the event named%s \n\n",temp->name);
    free(temp);
    find_max_duration(node);
}

void change_info_name(assist_node*node, const char* tar_date, const char *n_name) {
    struct StuInfo *temp = node->head->next;
    char pre_name[100];
    strcpy(pre_name,temp->name);
    while (temp != NULL &&strcmp(temp->date,tar_date)!=0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the event!\n");
        return;
    }
    strcpy(temp->name, n_name);
    printf("Change the event named %s  into name %s\n",pre_name,temp->name);
}

void change_info_duration(assist_node*node, const char* tar_name, const int n_duration) {
    struct StuInfo *temp = node->head->next;
    while (temp != NULL && strcmp(temp->name,tar_name)!=0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the event!\n\n");
        return;
    }
    temp->duration=n_duration;
    printf("Change event %s's duration into %s\n",temp->name,temp->duration);
}

void travel_info(struct StuInfo *head, const char*tar_name) {
    struct StuInfo *temp = head->next;
    while (temp != NULL && strcmp(tar_name,temp->name)!=0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Failed to find the student!\n");
        return;
    }
    printf("The event information\nDate:%s\nName:%s\nduration:%d\n\n", temp->date, temp->name, temp->duration);
}

void print_all_info(struct StuInfo *head) {
    struct StuInfo *temp = head->next;
    printf("The event information:\n\n");
    while (temp != NULL) {
        printf("Date:%s\nName:%s\nduration:%d\n\n", temp->date, temp->name, temp->duration);
        temp = temp->next;
    }
    printf("end\n\n");
}


void print_max(assist_node *node){
    find_max_duration(node);
    printf("The max duration is %d,name:%s\n",node->max_name->duration,node->max_name->name);
}

int main() {
    struct StuInfo *new_list = create_info("11.11", "X", 10);
    struct StuInfo *head=(struct StuInfo*)malloc(sizeof(struct StuInfo));
    struct StuInfo *tail=(struct StuInfo*)malloc(sizeof(struct StuInfo));
    struct StuInfo *max_name_node=(struct StuInfo*)malloc(sizeof(struct StuInfo));
    head->next=new_list;
    tail->next=new_list;
    max_name_node->next=new_list;
    assist_node assistant={head,tail,max_name_node};
    add_info(&assistant, "A","11.12","B",20);
    add_info(&assistant, "X", "11.13", "D", 45);
    add_last_info(&assistant, "11.14", "C", 14);
    print_all_info(head);
    delete_info(&assistant, "X");
    print_all_info(head);
    change_info_duration(&assistant, "B", 1919);
    travel_info(head, "D");
    print_all_info(head);
    print_max(&assistant);
    return 0;
}
