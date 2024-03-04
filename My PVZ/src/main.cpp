#include "Game.h"
#include <stdio.h>

int main(int argc,char* argv[]){
    Game game(1);
    game.loadMainMenu();
    game.loadMainGame(1);
    game.printGrid();
    game.mainGame();
    return 0;
}