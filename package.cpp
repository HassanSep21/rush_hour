#include "package.h"

Package::Package(int x, int y, int desX, int desY) : x(x), y(y), desX(desX), desY(desY), pickedUp(false), droppedOff(false), fee(20.0 + rand() % 50) {}

int Package::getX() const { return x; }
int Package::getY() const { return y; }
int Package::getDesX() const { return desX; }
int Package::getDesY() const { return desY; }
float Package::getFee() const { return fee; }

bool Package::isPickedUp() const { return pickedUp; }

void Package::setPickedUp(bool status) { pickedUp = status; }
void Package::setDroppedOff(bool status)  { droppedOff = status; }

void Package::draw()
{
    if (!pickedUp)
    {
        DrawSquare(x + 5, y + 5, 30, colors[SANDY_BROWN]); // Box
        DrawRectangle(x + 8, y + 15, 24, 10, colors[WHITE]); // Tape
    }
    else if (!droppedOff)
    {
        DrawSquare(desX + 5, desY + 5, 30, colors[LIGHT_BLUE]);
    }
}

bool Package::overlaps(int carX, int carY) const { return (pickedUp ? (carX == desX && carY == desY) : (carX == x && carY == y)); }
