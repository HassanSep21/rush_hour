#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int type) : x(x), y(y), type(type) {}

int Obstacle::getX() const { return x; }
int Obstacle::getY() const { return y; }
int Obstacle::getType() const { return type; }
void Obstacle::setType(int type) { this->type = type; }

void Obstacle::draw()
{
    if (type == ObstacleType::TREE)
    {
        // Trunk
        DrawRectangle(x + 15, y + 5, 10, 15, colors[BROWN]);

        // Leafs
        DrawCircle(x + 20, y + 30, 7, colors[DARK_GREEN]);
        DrawCircle(x + 15, y + 22, 7, colors[DARK_GREEN]);
        DrawCircle(x + 25, y + 22, 7, colors[DARK_GREEN]);

        // Roots
        DrawTriangle(x + 20, y + 15, x + 12, y, x + 28, y, colors[BROWN]);
    }
    else if (type == ObstacleType::BOX)
    {
        DrawSquare(x + 5, y + 5, 30, colors[BROWN]); // Box
        
        // Mark
        DrawLine(x + 10, y + 28, x + 30, y + 10, 5, colors[RED]);
        DrawLine(x + 10, y + 10, x + 30, y + 28, 5, colors[RED]);
    }
}

bool Obstacle::overlaps(int carX, int carY) const { return carX == x && carY == y; }