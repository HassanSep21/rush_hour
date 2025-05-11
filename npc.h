#ifndef NPC_H
#define NPC_H

#include "util.h"

class NPC
{
private:
	int x;
	int y;
	static int frameCounter;
	static int speedDelay;

public:
	NPC(int x = 40, int y = 40, int frameCount = 0, int speedDelay = 20);

	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);
	static int getFrameCounter();
	static int getSpeedDelay();
	static void setFrameCounter(int count);
	static void setSpeedDelay(int delay);

	void draw() const;
};

#endif