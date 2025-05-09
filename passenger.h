#ifndef PASS_H
#define PASS_H

#include "util.h"

class Passenger
{
private:
	int x;
	int y;
	int desX;
	int desY;
	bool pickedUp;
	bool reachedDes;
	float fair;

public:
	Passenger(int x = 40, int y = 40, int desX = 40, int desY = 40);
    
	int getX() const; 	  
	int getY() const;	  
	int getDesX() const;   
	int getDesY() const;   
	float getFair() const; 

	bool isPickedUp() const;

	void setPickedUp(bool status);
	void setReached(bool status);

	void draw();

	bool overlaps(int carX, int carY) const;
};

#endif