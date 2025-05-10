#ifndef OBS_H
#define OBS_H

#include "util.h"

struct ObstacleType
{
	static const int TREE = 0;
	static const int BOX = 1;
};

class Obstacle
{
private:
	int x;
	int y;
	int type;

public:
	Obstacle(int x = 40, int y = 40, int type = ObstacleType::TREE);

	int getX() const;
	int getY() const;
	int getType() const;
	void setType(int type);

	void draw();

	bool overlaps(int carX, int carY) const;
};

#endif