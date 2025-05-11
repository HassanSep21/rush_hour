#ifndef BOARD_H
#define BOARD_H

#include "util.h"
#include "car.h"
#include "fuelstation.h"
#include "npc.h"
#include "obstacle.h"
#include "package.h"
#include "passenger.h"
#include "player.h"
#include "rolechangestation.h"

struct BoardObjects
{
	static const int ROAD = 0;
	static const int BUILDING = 1;
	static const int FUEL_STATION = 2;
	static const int ROLE_CHANGE_STATION = 3;
	static const int OBSTACLE = 4;
};

class Board
{
private:
	static const int SIZE = 20;
	int grid[SIZE][SIZE];

	RoleChangeStation roleChangeStation;

	FuelStation *fuelStations;
	int fuelStationCount;

	Obstacle *obstacles;
	int obstacleCount;

	Passenger *passengers;
	int passengerCount;

	Package *packages;
	int packageCount;

	NPC *npcs;
	int npcCount;

public:
	Board(int fuelStationCount = 2, int obstacleCount = 4, int passengerCount = 2, int packageCount = 2, int npcCount = 3);
	Board(const Board &b);
	~Board();

	int getFuelStationCount() const;
	FuelStation getFuelStation(int i);

	int getObstacleCount() const;
	Obstacle getObstacles(int i);

	int getPassengerCount() const;
	Passenger &getPassenger(int i);

	int getPackageCount() const;
	Package &getPackage(int i);

	int getNpcCount() const;
	NPC &getNpc(int i);

	void increaseNpcs();

	RoleChangeStation getRoleChangeStation();

	void generateFuelStations();
	void generateObstacles();
	void generatePassengers();
	void generatePackages();
	void generateNpcs();

	void setRandomPos(Passenger &p);
	void setRandomPos(Package &p);

	bool isRoad(int i, int j) const;
	bool isDrivable(int i, int j) const;

	void generateGrid();

	void draw(Player playerCar) const;
};

#endif