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
	bool holding;

public:
	Player(int role = 1);

	string getName() const;
	void setName(string name);
	float getCash() const; 
	void updateCash(float amount); 
	int getScore() const;  
	void updateScore(int points);
	int getRole() const;   
	void toggleRole();
	int getJobsCompleted() const;
	void jobCompleted();
	bool isHolding() const;
	void setHolding(bool state);
	void setRole(int role);
};

#endif