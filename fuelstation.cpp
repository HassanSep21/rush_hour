#include "fuelstation.h"


FuelStation::FuelStation(int x, int y) : x(x), y(y), fuelPrice(20.0) {}

int FuelStation::getX() const { return x; }
int FuelStation::getY() const { return y; }
float FuelStation::getFuelPrice() const { return fuelPrice; }

int FuelStation::draw()
{
    DrawRectangle(x + 10, y + 5, 20, 30, colors[RED]); // Body
    DrawRectangle(x + 12, y + 22, 16, 10, colors[GHOST_WHITE]); // Screen
    DrawRectangle(x + 30, y + 18, 5, 10, colors[BLACK]); // Handle
}

bool FuelStation::overlaps(int carX, int carY) const { return carX == x && carY == y; }

void FuelStation::reFuel(Player &p)
{
    float priceForFull = fuelPrice * (1 - p.getFuel() / float(MAX_FUEL));
    if (p.getCash() >= priceForFull && p.getFuel() < MAX_FUEL)
    {
        p.updateCash(-priceForFull);
        p.setFuel(MAX_FUEL);
    }
}
