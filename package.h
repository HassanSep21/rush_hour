#ifndef PACK_H
#define PACK_H

#include "util.h"

class Package
{
private:
	int x;
	int y;
	int desX;
	int desY;
	bool pickedUp;
	bool droppedOff;
	float fee;

public:
	Package(int x = 40, int y = 40, int desX = 40, int desY = 40);

	int getX() const; 	
	int getY() const;	
	int getDesX() const; 
	int getDesY() const; 
	float getFee() const;

	bool isPickedUp() const;

	void setPickedUp(bool status);
	void setDroppedOff(bool status);

	void draw();

	bool overlaps(int carX, int carY) const;
};

#endif