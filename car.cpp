#include "car.h"

Car::Car(int x, int y, float *color, int fuel) : x(x), y(y), color(color ? color : colors[rand() % COLOR_COUNT]), fuel(fuel) {}

void Car::moveUp() 
{
    if (fuel > 0)
    {
        y += CELL_SIZE; 
        fuel--;
    }
}
	
void Car::moveDown() 
{
    if (fuel > 0)
    {
        y -= CELL_SIZE; 
        fuel--;
    }
	}
	
void Car::moveRight() 
{
    if (fuel > 0)
    {
        x += CELL_SIZE; 
        fuel--;
    }
}
	
void Car::moveLeft() 
{
    if (fuel > 0)
    {
        x -= CELL_SIZE; 
        fuel--;
    }
}

int Car::getX() const { return x; }
int Car::getY() const { return y; }
void Car::setX(int x) 
{
    if (fuel > 10)
    {
        this->x = x; 
        fuel--;
    }
}
void Car::setY(int y) 
{
    if (fuel > 10)
    {
        this->y = y; 
        fuel--;
    }
}
int Car::getFuel() const { return fuel; }
float *Car::getColor() const { return color; }

void Car::setFuel(int fuel) { this->fuel = fuel; }
void Car::setColor(float *color) { this->color = color; }

void Car::draw() const
{
    // Tyres
    DrawSquare(x + 5, y, 10, colors[BLACK]);
    DrawSquare(x + 25, y, 10, colors[BLACK]);

    // Body
    DrawRectangle(x + 5, y + 5, 30, 15, color);

    // Wind Screen
    DrawRectangle(x + 10, y + 15, 20, 15, color);
    DrawRectangle(x + 12, y + 20, 16, 8, colors[WHITE]);

    // Lights
    DrawCircle(x + 10, y + 13, 3, colors[WHITE]);
    DrawCircle(x + 28, y + 13, 3, colors[WHITE]);

    // Grill
    DrawLine(x + 15, y + 13, x + 23, y + 13, 1, colors[BLACK]);
    DrawLine(x + 15, y + 11, x + 23, y + 11, 1, colors[BLACK]);
}
