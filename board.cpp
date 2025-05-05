#include "board.h"

/*
    0 = Road
    1 = Building

*/

Board::Board()
{
    srand(time(0));

    generateGrid();
}

void Board::generateGrid() 
{
    // Fill all cells with walls
    for (int i = 0; i < SIZE; i++) 
    {
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = 0;
    }

    // Generate roads using recursive backtracking
    generateRoad(1, 1);

    // Randomly remove some buildings
    for (int i = 0; i < 100; i++)
    {
        int randXIndex = rand() % SIZE - 1;
        int randYIndex = rand() % SIZE - 1;

        if (grid[randXIndex][randYIndex] == 1)
            grid[randXIndex][randYIndex] = 0;
    }
}

void Board::generateRoad(int x, int y) 
{
    grid[x][y] = 1;  // Mark the starting cell as a road

    // 4 directions: right, down, left, up
    int dir[4][2] = { {0,2}, {2,0}, {0,-2}, {-2,0} };

    // Shuffle the directions
    for (int i = 0; i < 4; i++) 
    {
        int r = rand() % 4;
        int tx = dir[i][0], ty = dir[i][1];
        dir[i][0] = dir[r][0];
        dir[i][1] = dir[r][1];
        dir[r][0] = tx;
        dir[r][1] = ty;
    }

    // Try all 4 directions
    for (int i = 0; i < 4; i++) 
    {
        int newX = x + dir[i][0];
        int newY = y + dir[i][1];

        // If new cell is within bounds and not visited
        if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE && grid[newX][newY] == 0) 
        {
            // removing the walls in between current and neighbor cells
            grid[x + dir[i][0] / 2][y + dir[i][1] / 2] = 1;

            // Recursively continue the road generation
            generateRoad(newX, newY);
        }
    }
}


void Board::drawGrid() 
{
    DrawSquare(GRID_START_X, GRID_START_Y, 800, colors[GRAY]); // Base of Board

    for (int i = 0; i < SIZE; i++) 
    {
        for (int j = 0; j < SIZE; j++) 
        {
            if (grid[i][j] == 1)
                DrawSquare(GRID_START_X + CELL_SIZE * j, GRID_START_Y + CELL_SIZE * i, CELL_SIZE, colors[BLACK]);
        }
    }

    // Draw vertical lines
    for (int i = 0; i <= SIZE; i++) 
    {
        DrawLine(GRID_START_X + CELL_SIZE * i, GRID_START_Y, GRID_START_X + CELL_SIZE * i, GRID_END_Y, 2, colors[BLACK]);
        DrawLine(GRID_START_X, GRID_START_Y + CELL_SIZE * i, GRID_END_X, GRID_START_Y + CELL_SIZE * i, 2, colors[BLACK]);
    }
}

bool Board::isRoad(int i, int j)
{
    return grid[i][j] != 0;
}