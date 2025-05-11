#ifndef LEADER_H
#define LEADER_H

#include "util.h"

struct Entry 
{
	char name[50];
	int score;
};

class Leaderboard 
{
private:
	static const int MAX_ENTRIES = 10;
	Entry entries[MAX_ENTRIES + 1];

public:
	void updateLeaderboard(const char* playerName, int playerScore);

	void showLeaderboard() const;
};

#endif
