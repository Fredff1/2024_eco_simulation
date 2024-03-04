#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StuInfo {
    int ID;
    char name[100];
    int age;
    struct StuInfo *next;
};

struct StuInfo *create_info(int n_ID, const char *n_name, int n_age) {
    struct StuInfo *n_stu = (struct StuInfo *)malloc(sizeof(struct StuInfo));
    n_stu->next = NULL;
    n_stu->age = n_age;
    n_stu->ID = n_ID;
    strcpy(n_stu->name, n_name);
    return n_stu;
}

// ID不输入则在末尾添加
void add_info(struct StuInfo *head, const int ID, const int n_id, const char *n_name, const int n_age) {
    struct StuInfo *temp = head, *n;
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
    n->age = n_age;
    n->ID = n_id;
    strcpy(n->name, n_name);
    n->next = temp->next;
    temp->next = n;
}

void add_last_info(struct StuInfo *head, const int n_id, const char *n_name, const int n_age) {
    struct StuInfo *temp = head, *n;
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
}

void delete_info(struct StuInfo **head, const int ID) {
    if (*head == NULL) {
        printf("The list is empty!\n");
        return;
    }
    struct StuInfo *temp = *head, *prev = NULL;
    if (temp != NULL && temp->ID == ID) {
        *head = temp->next; 
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
    printf("Delete ID %d\n", ID);
    free(temp);
}

void change_info_name(struct StuInfo *head, const int tar_ID, const char *n_name) {
    struct StuInfo *temp = head;
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

void change_info_age(struct StuInfo *head, const int tar_ID, const int n_age) {
    struct StuInfo *temp = head;
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
    struct StuInfo *temp = head;
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
    struct StuInfo *temp = head;
    printf("The student information\n\n");
    while (temp != NULL) {
        printf("ID:%d\nName:%s\nAge:%d\n\n", temp->ID, temp->name, temp->age);
        temp = temp->next;
    }
    printf("end\n\n");
}

int main() {
    struct StuInfo *head = create_info(1, "TianSuoHaoEr", 24);
    add_info(head, 1, 2, "A", 11);
    add_info(head, 10, 2, "B", 45);
    add_last_info(head, 100, "C", 14);
    print_all_info(head);
    delete_info(&head, 1);
    print_all_info(head);
    change_info_age(head, 100, 1919);
    travel_info(head, 100);
    print_all_info(head);
    return 0;
}
