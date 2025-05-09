#ifndef ROLE_H
#define ROLE_H

#include "util.h"
#include "player.h"

class RoleChangeStation
{
private:
	int x;
	int y;

public:
    RoleChangeStation(int x = 40, int y = 40);

	int getX() const;
	int getY() const;

	int draw();

	bool overlaps(int carX, int carY) const;
	void changeRole(Player &p);
};

#endif
