#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

#define ROWS 10
#define COLS 10


void printGrid(int grid[ROWS][COLS]) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			printf("%c ", grid[i][j] ? 'O' : '.');
		}
		printf("\n");
	}
}

int countNeighbors(int x, int y, int grid[ROWS][COLS]) {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int newX = x + i;
			int newY = y + j;
			if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS) {
				count += grid[newX][newY];
			}
		}
	}
	count -= grid[x][y];  // Exclude the cell itself
	return count;
}

void updateGrid(int grid[ROWS][COLS]) {
	int newGrid[ROWS][COLS];

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			int neighbors = countNeighbors(i, j, grid);
			if (grid[i][j] && (neighbors < 2 || neighbors > 3)) {
				newGrid[i][j] = 0;
			} else if (!grid[i][j] && neighbors == 3) {
				newGrid[i][j] = 1;
			} else {
				newGrid[i][j] = grid[i][j];
			}

			grid[i][j] = newGrid[i][j];

		}
	}




}



int main() {
	int grid[ROWS][COLS] = {

		{1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 1, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 1, 0, 0, 1, 1, 0, 0, 0, 0},
		{0, 0, 1, 0, 1, 1, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 0, 0, 0, 0, 0}
	};

	while (1) {
		printGrid(grid);
		updateGrid(grid);
		sleep(1);
		printf("\n");  // 
		// printf("\033c");  
	}

	return 0;
}
