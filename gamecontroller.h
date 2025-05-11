#ifndef GAME_H
#define GAME_H

#include "util.h"

#include "board.h"
#include "car.h"
#include "fuelstation.h"
#include "leaderboard.h"
#include "npc.h"
#include "obstacle.h"
#include "package.h"
#include "passenger.h"
#include "player.h"
#include "rolechangestation.h"

struct GameState
{
	static const int GAME_MENU = 0;
	static const int GAME_START = 1;
	static const int GAME_OVER= 2;
	static const int GAME_WIN = 3;
	static const int GAME_INPUT_NAME = 4;
	static const int GAME_SHOW_LEADERBOARD = 5;
};

class GameController
{
private:
	Player playerCar;
	Board board;

	int gameState;
	float gameTime;
	int menuSelection;

	Leaderboard leaderboard;
    
	void handleFuelRefill();
    void handleRoleChange();
    void handlePickupAndDropoff();

public:
    static Board makeRandomBoard();

    GameController();

    void render() const;
    void checkGameStatus();
    int getGameState() const;

    void handleNonPrintableKeys(int key);
    void handlePrintableKeys(char key);

    void update();
    void updateTime();
    void updateNPCs();

    void drawMenu() const;
    void drawGame() const;
    void drawGameOverScreen() const;
    void drawWinScreen() const;
    void drawNameInputScreen() const;
    void drawLeaderBoard() const;
};

#endif