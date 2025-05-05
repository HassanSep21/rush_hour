#ifndef BOARD_H
#define BOARD_H

#include "util.h"

class Board
{
private:
    static const int CELL_SIZE = 40;

    static const int GRID_START_X = 200;
    static const int GRID_START_Y = 100;

    static const int GRID_END_X = 1000;
    static const int GRID_END_Y = 900;

    static const int SIZE = 20;
    int grid[SIZE][SIZE];

public:
    Board();
    void generateGrid();
    void generateRoad(int x, int y);
    void drawGrid();
    bool isRoad(int i, int j);
};

#endif