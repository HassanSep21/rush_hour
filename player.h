#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include "car.h"

struct PlayerRoles
{
	static const int TAXI = 0;
	static const int DELIVERY = 1;
};

class Player : public Car
{
private:
	string name;
	float cash;
	int score; 
	int jobsCompleted;
	int role;

public:
	Player(int role = 0);

	string getName() const;
	void setName(string name);
	float getCash() const; 
	int getScore() const;  
	int getRole() const;   
	void toggleRole();

	void jobCompleted();

	void updateScore(int points);
	void updateCash(float amount); 
};

#endif