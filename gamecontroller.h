#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "util.h"
#include "board.h"
#include "player.h"
#include "car.h"
#include "npc.h"
#include "obstacle.h"
#include "passenger.h"
#include "package.h"
#include "fuelstation.h"
#include "rolechangestation.h"

// Game states
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    WIN,
    GAMEOVER
};

class GameController {
private:
    // Game objects
    Player playerCar;
    Board board;
    
    // Game state
    GameState state;
    float gameTime;
    
    // Menu selection
    int menuSelection;
    
    // Random seed
    bool boardInitialized;
    
    // Private methods
    void updateNPCs();
    void checkGameStatus();
    void handleFuelRefill();
    void handleRoleChange();
    void handlePickupAndDropoff();
    
public:
    // Constructor
    GameController();
    
    // Game state management
    void initializeGame(int role);
    void update();
    void togglePause();
    void resetGame();
    
    // Input handling
    void handleKeyPress(unsigned char key);
    void handleSpecialKeyPress(int key);
    
    // Drawing methods
    void drawGame();
    void drawMenu();
    void drawPauseScreen();
    void drawGameOverScreen();
    void drawWinScreen();
    
    // Debug methods
    void debugPosition();
    
    // Getters
    GameState getState() const { return state; }
    Player& getPlayer() { return playerCar; }
    Board& getBoard() { return board; }
    float getGameTime() const { return gameTime; }
};

#endif
