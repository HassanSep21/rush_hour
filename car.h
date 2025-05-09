#ifndef CAR_H
#define CAR_H

#include "util.h"

class Car
{
private:
	int x;
	int y;
	int fuel;
	float *color;

public:
	Car(int x = 40, int y = 40, float *color = nullptr, int fuel = MAX_FUEL);

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();

	int getX() const;  	 
	int getY() const;   	 
	int getFuel() const;	  	 
	void setFuel(int fuel);   	 
	float *getColor() const;      
	void setColor(float *color);

	void draw() const;
};

#endif // CAR_H