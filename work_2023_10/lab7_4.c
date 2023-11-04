#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
typedef enum{
    dead=0,
    alive=1,
} gridState;

typedef struct{
    gridState*grid;

    int length;

    int turns;

}lifeGame;

gridState get_next_state(int x,lifeGame* l){
    gridState nextState=dead;
    int count;
    if(x>0&&x<l->length-1){
        count=l->grid[x-1]+l->grid[x+1]; 
    }else if(x==l->length-1){
        count=l->grid[x-1]+l->grid[0];
    }else if(x==0){
        count=l->grid[1]+l->grid[l->length-1];
    }

    if(l->grid[x]==alive){
        switch(count){
            case 0:
            nextState=alive;
            break;
            case 1:
            nextState=alive;
            break;
            case 2:
            nextState=dead;
            break;
        }
    }else if(l->grid[x]==dead){
        switch (count){
            case 0:
            nextState=dead;
            break;
            case 2:
            nextState=alive;
            break;
            case 1:
            if(x>0&&x<l->length-1){
                if(l->grid[x+1]==alive){
                    nextState=alive;
                }else {
                    nextState=dead;
                }
            }else if(x==0){
                if(l->grid[1]==alive){
                    nextState=alive;
                }else{
                    nextState=dead;
                }
            }else if(x==l->length-1){
                if(l->grid[0]==alive){
                    nextState=alive;
                }else{
                    nextState=dead;
                }
            }
        }
        
    }
        return nextState;
}

lifeGame*newGame(int len){
    lifeGame*l=(lifeGame*)malloc(sizeof(lifeGame*));
    l->grid=(gridState*)malloc(sizeof(gridState)*len);
    l->length=len;
    memset(l->grid,'0',sizeof(l->grid));
    return l;
}

void freeGame(lifeGame*l){
    free(l->grid);
    free(l);
    return;
}

void next_generation(lifeGame*l){
    gridState temp_grid[l->length];
    for(int i=0;i<l->length;i++){
        temp_grid[i]=get_next_state(i,l);
    }
    for(int i=0;i<l->length;i++){
        l->grid[i]=temp_grid[i];
    }
}

void printGame(lifeGame*l){
    for(int i=0;i<l->length;i++){
        if(l->grid[i]==dead){
            printf("| ");

        }else{
            printf("|*");
        }
    }
    printf("|\n");
}

int main(){
    srand(time(NULL));
    int len;
    lifeGame*l;
    printf("Enter your gird length\n");
    scanf("%d",&len);
    l=newGame(len);
    int t;
    for(int i=0;i<l->length;i++){
        l->grid[i]=(gridState)rand()%2;
    }
    printGame(l);
    printf("Enter your expected turns\n");
    scanf("%d",&l->turns);
    if(l->turns<25){
        l->turns=25;
    }
    printGame(l);
    while(t<=l->turns){
        next_generation(l);
        printGame(l);
        _sleep(1000);
        t+=1;
        
    }
    freeGame(l);
    printf("Game over!\n");
    return 0;
}

