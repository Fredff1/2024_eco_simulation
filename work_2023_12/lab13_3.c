#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int max_size = 10;
typedef struct {
    char name[101];
    char writer[101];
    int ISBN;
} book_data;

book_data empty_book() {
    book_data new_book = {"none", "none", -1};
    return new_book;
}

book_data *init_array() {
    book_data *bookList = (book_data *)malloc(max_size * sizeof(book_data));
    for (int i = 0; i < max_size; i++) {
        bookList[i] = empty_book();
    }
    return bookList;
}

void extend_bookList(book_data *bookList) {
    bookList=(book_data*)(bookList, sizeof(book_data) * (max_size + 1));
    max_size += 1;
}

void rec_bookList(book_data *bookList) {
    bookList=(book_data*)realloc(bookList, sizeof(book_data) * (max_size - 1));
    max_size -= 1;
}

void add_book(book_data *bookList, const char *new_name, const char *new_writer, int new_ISBN) {
    for (int i = 0; i < max_size; i++) {
        if (bookList[i].ISBN == -1) {
            strcpy(bookList[i].name,new_name);
            strcpy(bookList[i].writer,new_writer);
            if(new_ISBN<0||new_ISBN>1000){
                new_ISBN=-1;
            }
            bookList[i].ISBN=new_ISBN;
            return;
        }
    }
    extend_bookList(bookList);
    strcpy(bookList[max_size - 1].name,new_name);
    strcpy(bookList[max_size - 1].writer,new_writer);
    bookList[max_size - 1].ISBN = new_ISBN;
}

void delete_book(book_data *bookList, const int target_ISBN) {
    bool flag = false;
    for (int i = 0; i < max_size; i++) {
        if (bookList[i].ISBN == target_ISBN) {
            bookList[i] = empty_book();
            flag = true;
        }
    }
    if (flag == false) {
        printf("target book not found\n");
    }
    for (int i = max_size - 1; i >= 0; i--) {
        if (bookList[i].ISBN == -1) {
            rec_bookList(bookList);
        }
    }
}

void print_menu() {
    printf("Welcome\n");
    printf("0:add book 1:delete book 2:print current book 3:quit\n");
}

void print_all_book(book_data *bookList) {
    printf("Current books:\n");
    for (int i = 0; i < max_size; i++) {
        if (bookList[i].ISBN == -1) {
            continue;
        }
        printf("Name:%s Author:%s ISBN:%d \n", bookList[i].name, bookList[i].writer, bookList[i].ISBN);
    }
}

void read_input(char*target){
    char s;
    int pos=0;
    getchar();
    while((s=getchar())!=' '){
        target[pos]=s;
        pos++;
        if(pos>98){
            break;
        }
    }
    target[pos]='\0';
}

void main_menu(book_data*bookList){
    int cmd = 0, temp_ISBN = 0;
    char temp_writer[100], temp_name[100];
    bool flag_running = true;
    while (flag_running) {
        temp_ISBN = 0;
        memset(temp_name, '\0', sizeof(temp_name));
        memset(temp_writer, '\0', sizeof(temp_writer));
        print_menu();
        scanf("%d", &cmd);
        switch (cmd) {
        case 0:
            printf("Enter the book name,author,ISBN to add\n");
            read_input(temp_name);
            read_input(temp_writer);
            if(scanf("%d",&temp_ISBN)!=1){
                printf("Invalid input\n");
                temp_ISBN=-1;
                while(getchar()!='\n'){

                }
            }
            add_book(bookList, temp_name, temp_writer, temp_ISBN);
            break;
        case 1:
            printf("Enter the ISBN of the book you want to delete\n");
            scanf("%d", &temp_ISBN);
            delete_book(bookList, temp_ISBN);
            break;
        case 2:
            print_all_book(bookList);
            break;
        case 3:
            flag_running = false;
            break;
        default:
            printf("Invalid command\n");
        }
    }
}

int main() {
    book_data *bookList = init_array();
    add_book(bookList, "1145141919810", "beast", 1);
    print_all_book(bookList);
    main_menu(bookList);
    free(bookList);
    return 0;
}