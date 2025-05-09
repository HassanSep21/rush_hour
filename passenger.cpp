#include "passenger.h"

Passenger::Passenger(int x, int y, int desX, int desY) : x(x), y(y), desX(desX), desY(desY), pickedUp(false), reachedDes(false), fair(20.0 + rand() % 50) {}

int Passenger::getX() const { return x; }
int Passenger::getY() const { return y; }
int Passenger::getDesX() const { return desX; }
int Passenger::getDesY() const { return desY; }
float Passenger::getFair() const { return fair; }

bool Passenger::isPickedUp() const { return pickedUp; }

void Passenger::setPickedUp(bool status) { pickedUp = status; }
void Passenger::setReached(bool status)  { reachedDes = status; }

void Passenger::draw()
{
    if (!pickedUp)
    {
        DrawCircle(x + 20, y + 33, 5, colors[BLACK]); // Head
        DrawLine(x + 20, y + 33, x + 20, y + 15, 2, colors[BLACK]); // Torso
    
        DrawLine(x + 20, y + 30, x + 15, y + 12, 2, colors[BLACK]); // Left Hand
        DrawLine(x + 20, y + 30, x + 25, y + 12, 2, colors[BLACK]); // Right Hand
    
        DrawLine(x + 20, y + 15, x + 15, y, 2, colors[BLACK]); // Left Leg
        DrawLine(x + 20, y + 15, x + 25, y, 2, colors[BLACK]); // Right Leg
    }
    else if (!reachedDes)
    {
        DrawSquare(desX + 5, desY + 5, 30, colors[LIGHT_GREEN]);
    }
}

bool Passenger::overlaps(int carX, int carY) const { return (pickedUp ? (carX == desX && carY == desY) : (carX == x && carY == y)); }