#ifndef FUEL_H
#define FUEL_H

#include "util.h"
#include "player.h"

class FuelStation
{
private:
	int x;
	int y;
	float fuelPrice;

public:
	FuelStation(int x = 40, int y = 40);

	int getX() const;
	int getY() const;
	float getFuelPrice() const;

	int draw();

	bool overlaps(int carX, int carY) const;
	void reFuel(Player &p);
};

#endif