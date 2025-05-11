#include "gamecontroller.h"

static Board makeRandomBoard() {
    int randFuel = 2 + rand() % 2;
    int randObs = 4 + rand() % 3;
    int randPass = 2 + rand() % 3;
    int randPack = 2 + rand() % 2;
    return Board(randFuel, randObs, randPass, randPack);
}

GameController::GameController() : 
    playerCar(0),  // Default role (will be set in initializeGame)
    state(GameState::MENU),
    gameTime(180),
    menuSelection(0),
    boardInitialized(false),
    board(makeRandomBoard())
{}

void GameController::initializeGame(int role) {
    // Ensure random seed is properly set
    if (!boardInitialized) {
        srand(time(NULL));
        boardInitialized = true;
    }
    
    // -1 means random role
    if (role == -1) {
        role = rand() % 2;
    }
    
    // Initialize player with selected role
    playerCar = Player(role);
    
    // Reset game timer
    gameTime = 180;
    
    // Initialize board with random values
    int randFuel = 2 + rand() % 2;
    int randObs = 4 + rand() % 3;
    int randPass = 2 + rand() % 3;
    int randPack = 2 + rand() % 2;
    
    // Change state to playing
    state = GameState::PLAYING;
}

void GameController::update() {
    if (state != GameState::PLAYING) {
        return;
    }
    
    // Update game timer
    gameTime -= 0.1;
    
    // Update NPCs
    updateNPCs();
    
    // Check if game is over or won
    checkGameStatus();
}

void GameController::updateNPCs() {
    for (int i = 0; i < board.getNpcCount(); i++) {
        board.getNpc(i).setFrameCounter(board.getNpc(i).getFrameCounter() + 1);
        if (board.getNpc(i).getFrameCounter() >= board.getNpc(i).getSpeedDelay()) {
            board.getNpc(i).setFrameCounter(0);

            int attempts = 0;
            const int MAX_ATTEMPTS = 10;  // Prevent infinite loops
            
            while (attempts < MAX_ATTEMPTS) {
                attempts++;
                int newY = board.getNpc(i).getY();
                int newX = board.getNpc(i).getX();
        
                int randDir = rand() % 4;
                switch (randDir) {
                    case 0: newX -= CELL_SIZE; break; // Left
                    case 1: newX += CELL_SIZE; break; // Right
                    case 2: newY += CELL_SIZE; break; // Up
                    case 3: newY -= CELL_SIZE; break; // Down
                }

                // Ensure we stay within grid bounds
                if (newX < GRID_START_X || newX >= GRID_END_X || 
                    newY < GRID_START_Y || newY >= GRID_END_Y) {
                    continue;
                }

                // Convert to grid indices
                int y = (GRID_END_Y - newY) / CELL_SIZE;
                int x = (newX - GRID_START_X) / CELL_SIZE;

                // Check collision
                bool valid = board.isDrivable(y, x);
                
                for (int k = 0; k < board.getObstacleCount() && valid; k++) {
                    if (board.getObstacles(k).overlaps(newX, newY)) {
                        valid = false;
                    }
                }

                if (valid) {
                    board.getNpc(i).setX(newX);
                    board.getNpc(i).setY(newY);
                    break;
                }    
            }
        }
    }
}

void GameController::checkGameStatus() {
    // Check win condition (score >= 100)
    if (playerCar.getScore() >= 100) {
        state = GameState::WIN;
    }
    
    // Check lose condition (score < 0 or time up)
    if (playerCar.getScore() < 0 || gameTime <= 0) {
        state = GameState::GAMEOVER;
    }
}

void GameController::togglePause() {
    if (state == GameState::PLAYING) {
        state = GameState::PAUSED;
    } else if (state == GameState::PAUSED) {
        state = GameState::PLAYING;
    }
}

void GameController::resetGame() {
    state = GameState::MENU;
    menuSelection = 0;
    boardInitialized = false;
}

void GameController::handleKeyPress(unsigned char key) {
    switch (state) {
        case GameState::MENU:
            if (key == '1') {
                // Taxi driver
                initializeGame(0);
            } else if (key == '2') {
                // Delivery driver
                initializeGame(1);
            } else if (key == '3') {
                // Random role
                initializeGame(-1);
            }
            break;
            
        case GameState::PLAYING:
            if (key == 27) { // Escape key
                togglePause();
            } else if (key == 'f' || key == 'F') {
                handleFuelRefill();
            } else if (key == 'p' || key == 'P') {
                handleRoleChange();
            } else if (key == ' ') {
                handlePickupAndDropoff();
            } else if (key == 'd' || key == 'D') {
                // Debug key - print positions
                debugPosition();
            }
            break;
            
        case GameState::PAUSED:
            if (key == 27) { // Escape key
                togglePause();
            } else if (key == 'r' || key == 'R') {
                resetGame();
            }
            break;
            
        case GameState::WIN:
        case GameState::GAMEOVER:
            if (key == 'r' || key == 'R') {
                resetGame();
            }
            break;
    }
}

void GameController::debugPosition() {
    // Print player position
    cout << "Player position: (" << playerCar.getX() << ", " << playerCar.getY() << ")" << endl;
    
    // Print grid bounds
    cout << "Grid bounds: (" << GRID_START_X << ", " << GRID_START_Y << ") to (" 
              << GRID_END_X << ", " << GRID_END_Y << ")" << endl;
    
    // Print first NPC position if available
    if (board.getNpcCount() > 0) {
        cout << "NPC 0 position: (" << board.getNpc(0).getX() << ", " << board.getNpc(0).getY() << ")" << endl;
    }
}

void GameController::handleSpecialKeyPress(int key) {
    if (state != GameState::PLAYING) {
        return;
    }
    
    int oldX = playerCar.getX();
    int oldY = playerCar.getY();
    int newX = oldX;
    int newY = oldY;

    if (key == GLUT_KEY_LEFT)       newX -= CELL_SIZE;
    else if (key == GLUT_KEY_RIGHT) newX += CELL_SIZE;
    else if (key == GLUT_KEY_UP)    newY += CELL_SIZE;
    else if (key == GLUT_KEY_DOWN)  newY -= CELL_SIZE;

    // Ensure we stay within grid bounds
    if (newX < GRID_START_X || newX >= GRID_END_X || 
        newY < GRID_START_Y || newY >= GRID_END_Y) {
        return;
    }

    // Convert to grid indices
    int i = (GRID_END_Y - newY) / CELL_SIZE;
    int j = (newX - GRID_START_X) / CELL_SIZE;

    // Check collision
    bool valid = board.isDrivable(i, j);
    for (int k = 0; k < board.getObstacleCount() && valid; k++) {
        if (board.getObstacles(k).overlaps(newX, newY)) {
            valid = false;
        }
    }

    if (valid) {
        playerCar.setX(newX);
        playerCar.setY(newY);
        
        // Reduce fuel with movement
        playerCar.setFuel(playerCar.getFuel() - 1);

        // Running Over Passenger Penalty
        for (int i = 0; i < board.getPassengerCount(); i++) {
            if (!playerCar.isHolding() && (board.getPassenger(i).getX() == playerCar.getX() && 
                                         board.getPassenger(i).getY() == playerCar.getY())) {
                playerCar.updateScore(-5);
            }
        }
        
        // Running Over Package Penalty        
        for (int i = 0; i < board.getPackageCount(); i++) {
            if (!playerCar.isHolding() && (board.getPackage(i).getX() == playerCar.getX() && 
                                        board.getPackage(i).getY() == playerCar.getY())) {
                playerCar.updateScore(-8);
            }
        }

        // If YOU Hit An NPC Car Penalty
        for (int i = 0; i < board.getNpcCount(); i++) {
            if (playerCar.getX() == board.getNpc(i).getX() && playerCar.getY() == board.getNpc(i).getY()) {
                playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -3 : -5);
            }
        }
    } else {
        playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
    }
}

void GameController::handleFuelRefill() {
    for (int i = 0; i < board.getFuelStationCount(); i++) {
        if (board.getFuelStation(i).overlaps(playerCar.getX(), playerCar.getY())) {
            board.getFuelStation(i).reFuel(playerCar);
        }
    }
}

void GameController::handleRoleChange() {
    if (board.getRoleChangeStation().overlaps(playerCar.getX(), playerCar.getY())) {
        board.getRoleChangeStation().changeRole(playerCar);
    }
}

void GameController::handlePickupAndDropoff() {
    if (playerCar.getRole() == PlayerRoles::TAXI) {
        for (int i = 0; i < board.getPassengerCount(); i++) {
            Passenger &p = board.getPassenger(i);
            if (!playerCar.isHolding() && !p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) {
                p.setPickedUp(true);
                playerCar.setHolding(true);
                break;
            }

            if (playerCar.isHolding() && p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) {
                p.setPickedUp(false);
                p.setReached(true);
                playerCar.setHolding(false);
                playerCar.updateScore(10);
                playerCar.updateCash(p.getFare());
                playerCar.jobCompleted();

                board.setRandomPos(p);
                break;
            }
        }
    } else if (playerCar.getRole() == PlayerRoles::DELIVERY) {
        for (int i = 0; i < board.getPackageCount(); i++) {
            Package &p = board.getPackage(i);
            if (!playerCar.isHolding() && !p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) {
                p.setPickedUp(true);
                playerCar.setHolding(true);
                break;
            }

            if (playerCar.isHolding() && p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) {
                p.setPickedUp(false);
                p.setDroppedOff(true);
                playerCar.setHolding(false);
                playerCar.updateScore(20);
                playerCar.updateCash(p.getFee());
                playerCar.jobCompleted();

                board.setRandomPos(p);
                break;
            }
        }
    }

    if (playerCar.getJobsCompleted() != 0 && playerCar.getJobsCompleted() % 2 == 0) {
        NPC::setSpeedDelay(NPC::getSpeedDelay() - 1);
        board.increaseNpcs();
    }
}

void GameController::drawGame() {
    // Display Player Stats
    int minutes = int(gameTime) / 60;
    int seconds = int(gameTime) % 60;
    string timeStr = "Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);

    string scoreStr = "Score: " + to_string(playerCar.getScore());

    ostringstream stream;
    stream << fixed << setprecision(2) << playerCar.getCash();
    string cashStr = "Cash: " + stream.str() + "$";

    string roleStr = string(playerCar.getRole() == PlayerRoles::TAXI ? "Taxi" : "Delivery") + " Driver";
    
    DrawString(50, 950, timeStr, colors[BLACK]);
    DrawString(550, 950, scoreStr, colors[BLACK]);
    DrawString(1000, 950, cashStr, colors[BLACK]);
    DrawString(500, 40, "Role: ", colors[BLACK]);
    DrawString(560, 40, roleStr, colors[(playerCar.getRole() == PlayerRoles::TAXI ? ORANGE : BLUE)]);

    // Fuel Indicator
    float *fuelColor;

    if (playerCar.getFuel() >= 160)
        fuelColor = colors[GREEN];
    else if (playerCar.getFuel() >= 100)
        fuelColor = colors[YELLOW];
    else if (playerCar.getFuel() >= 60)
        fuelColor = colors[ORANGE];
    else
        fuelColor = colors[RED];

    DrawRoundRect(66, 446, 58, 208, colors[BLACK], 5);
    DrawRoundRect(68, 448, 54, 204, colors[WHITE], 5);
    DrawRoundRect(70, 450, 50, 200 * (playerCar.getFuel() / float(MAX_FUEL)), fuelColor, 5);

    // Drawing game objects
    board.draw(playerCar);
    playerCar.draw();
}

void GameController::drawMenu() {
    // Draw title
    DrawString(WIDTH/2 - 100, HEIGHT - 200, "RUSH HOUR", colors[RED]);
    
    // Draw options
    DrawString(WIDTH/2 - 150, HEIGHT/2 + 50, "1. Play as Taxi Driver", colors[ORANGE]);
    DrawString(WIDTH/2 - 150, HEIGHT/2, "2. Play as Delivery Driver", colors[BLUE]);
    DrawString(WIDTH/2 - 150, HEIGHT/2 - 50, "3. Random Role", colors[GREEN]);
    
    // Draw instructions
    DrawString(WIDTH/2 - 200, HEIGHT/2 - 150, "Use arrow keys to move", colors[BLACK]);
    DrawString(WIDTH/2 - 200, HEIGHT/2 - 180, "Space to pickup/dropoff", colors[BLACK]);
    DrawString(WIDTH/2 - 200, HEIGHT/2 - 210, "F to refuel at fuel stations", colors[BLACK]);
    DrawString(WIDTH/2 - 200, HEIGHT/2 - 240, "P to change role at role change stations", colors[BLACK]);
    DrawString(WIDTH/2 - 200, HEIGHT/2 - 270, "ESC to pause game", colors[BLACK]);
}

void GameController::drawPauseScreen() {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[WHITE]);
    
    // Draw pause text
    DrawString(WIDTH/2 - 80, HEIGHT/2 + 50, "GAME PAUSED", colors[RED]);
    DrawString(WIDTH/2 - 120, HEIGHT/2, "Press ESC to continue", colors[BLACK]);
    DrawString(WIDTH/2 - 120, HEIGHT/2 - 50, "Press R to return to menu", colors[BLACK]);
}

void GameController::drawGameOverScreen() {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[WHITE]);
    
    // Draw game over text
    DrawString(WIDTH/2 - 80, HEIGHT/2 + 50, "GAME OVER", colors[RED]);
    
    // Show final score
    string scoreStr = "Final Score: " + to_string(playerCar.getScore());
    DrawString(WIDTH/2 - 100, HEIGHT/2, scoreStr, colors[BLACK]);
    
    // Show instruction to return to menu
    DrawString(WIDTH/2 - 140, HEIGHT/2 - 50, "Press R to return to menu", colors[BLACK]);
}

void GameController::drawWinScreen() {
    // Draw semi-transparent overlay
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[WHITE]);
    
    // Draw win text
    DrawString(WIDTH/2 - 120, HEIGHT/2 + 50, "CONGRATULATIONS!", colors[GREEN]);
    DrawString(WIDTH/2 - 80, HEIGHT/2, "YOU WIN!", colors[GREEN]);
    
    // Show final score
    string scoreStr = "Final Score: " + to_string(playerCar.getScore());
    DrawString(WIDTH/2 - 100, HEIGHT/2 - 50, scoreStr, colors[BLACK]);
    
    // Show instruction to return to menu
    DrawString(WIDTH/2 - 140, HEIGHT/2 - 100, "Press R to return to menu", colors[BLACK]);
}