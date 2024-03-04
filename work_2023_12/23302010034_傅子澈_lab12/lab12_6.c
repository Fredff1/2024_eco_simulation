#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100

int stack[MAX_SIZE];
int *top = NULL;

int is_empty() { return top == NULL; }
int is_full() { return top == &stack[MAX_SIZE-1]; }
void reset() { return top = NULL; }
void push(int value) {
    if (is_full()) {
        printf("stack is full!\n");
        exit(1);
    }
    if(top==NULL){
        top=stack;
    }else{
        top++;
        *top = value;
    }
    
}
void pop() { 
    
    if (is_empty()) {
        fputs("stack is empty", stderr);
        exit(1);
    }

    if (top == stack) {
        top = NULL;
    } else {
        top--;
    }
 }

int main() {
    reset();
    if (!is_empty() || top != NULL) {
        fputs("stack not empty", stderr);
        exit(1);
    }
    for (int i = 0; i < MAX_SIZE; i += 1) {
        push(i);
        if (top == NULL || *top != i) {
            fputs("unexpected value", stderr);
            exit(1);
        }
    }
    if (!is_full()) {
        fputs("stack not full", stderr);
        exit(1);
    }
    for (int i = 0; i < MAX_SIZE; i += 1) {
        if (top == NULL || *top != MAX_SIZE - i - 1) {
            fputs("unexpected value", stderr);
            exit(1);
        }
        pop();
    }
    fputs("success", stdout);
}