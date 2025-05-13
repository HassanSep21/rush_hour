#include "gamecontroller.h"

void GameController::handleFuelRefill()
{
    for (int i = 0; i < board.getFuelStationCount(); i++)
    {
        if (board.getFuelStation(i).overlaps(playerCar.getX(), playerCar.getY()))
            board.getFuelStation(i).reFuel(playerCar);
    }
}

void GameController::handleRoleChange()
{
    if (board.getRoleChangeStation().overlaps(playerCar.getX(), playerCar.getY()))
            board.getRoleChangeStation().changeRole(playerCar);
}

void GameController::handlePickupAndDropoff()
{
    if (playerCar.getRole() == PlayerRoles::TAXI)
    {
        for (int i = 0; i < board.getPassengerCount(); i++)
        {
            Passenger &p = board.getPassenger(i);
            if (!playerCar.isHolding() && !p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
            {
                p.setPickedUp(true);
                playerCar.setHolding(true);
                break;
            }

            if (p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
            {
                p.setReached(true);
                playerCar.updateScore(10);
                playerCar.updateCash(p.getFare());
                playerCar.jobCompleted();

                board.setRandomPos(p);
            }
        }
    }
    else if (playerCar.getRole() == PlayerRoles::DELIVERY)
    {
        for (int i = 0; i < board.getPackageCount(); i++)
        {
            Package &p = board.getPackage(i);
            if (!playerCar.isHolding() && !p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
            {
                p.setPickedUp(true);
                playerCar.setHolding(true);
                break;
            }

            if (p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
            {
                p.setDroppedOff(true);
                playerCar.updateScore(20);
                playerCar.updateCash(p.getFee());
                playerCar.jobCompleted();

                board.setRandomPos(p);
            }
        }
    }

    if (playerCar.getJobsCompleted() != 0 && playerCar.getJobsCompleted() % 2 == 0)
    {
        NPC::setSpeedDelay(NPC::getSpeedDelay() - 1);
        board.increaseNpcs();
    }
}

Board GameController::makeRandomBoard() 
{
    int randFuel = 2 + rand() % 2;
    int randObs = 4 + rand() % 3;
    int randPass = 2 + rand() % 3;
    int randPack = 2 + rand() % 2;
    return Board(randFuel, randObs, randPass, randPack);
}

GameController::GameController() : gameState(GameState::GAME_MENU), gameTime(GAME_TIME),
                                    menuSelection(0), board(makeRandomBoard()) {}

void GameController::checkGameStatus()
{
    if (playerCar.getScore() >= 100)
    {
        gameState = GameState::GAME_WIN;
        leaderboard.updateLeaderboard(playerCar.getName().c_str(), playerCar.getScore());
    }		
    
    if (playerCar.getScore() < 0 || playerCar.getFuel() <= 10 || gameTime <= 0)
    {
        gameState = GameState::GAME_OVER;
        leaderboard.updateLeaderboard(playerCar.getName().c_str(), playerCar.getScore());
    }	
}

void GameController::render() const
{
    switch (gameState)
    {
        case GameState::GAME_MENU: drawMenu(); break;
        case GameState::GAME_START: drawGame(); break;
        case GameState::GAME_WIN: drawWinScreen(); break;
        case GameState::GAME_OVER: drawGameOverScreen(); break;
        case GameState::GAME_INPUT_NAME: drawNameInputScreen(); break;
        case GameState::GAME_SHOW_LEADERBOARD: drawLeaderBoard(); break;
    }
}

int GameController::getGameState() const { return gameState; }

void GameController::handleNonPrintableKeys(int key)
{
    if (gameState != GameState::GAME_START) return;

    int oldX = playerCar.getX();
    int oldY = playerCar.getY();
    int newX = oldX;
    int newY = oldY;

    if 		(key == GLUT_KEY_LEFT)  newX -= CELL_SIZE;
    else if (key == GLUT_KEY_RIGHT) newX += CELL_SIZE;
    else if (key == GLUT_KEY_UP)    newY += CELL_SIZE;
    else if (key == GLUT_KEY_DOWN)  newY -= CELL_SIZE;

    // Convert to grid indices
    int i = (GRID_END_Y - newY) / CELL_SIZE;
    int j = (newX - GRID_START_X) / CELL_SIZE;

    // Check collision
    bool valid = board.isDrivable(i, j);
    for (int k = 0; k < board.getObstacleCount() && valid; k++)
        if (board.getObstacles(k).overlaps(newX, newY))
            valid = false;

    if (valid)
    {
        playerCar.setX(newX);
        playerCar.setY(newY);

        // Running Over Passenger Penelty
        for (int i = 0; i < board.getPassengerCount(); i++)
            if (playerCar.getRole() == PlayerRoles:: TAXI && 
                playerCar.isHolding() && (board.getPassenger(i).getX() == playerCar.getX() && 
                board.getPassenger(i).getY() == playerCar.getY()))
                playerCar.updateScore(-5);
        
        // Running Over Package Penelty		
        for (int i = 0; i < board.getPackageCount(); i++)
            if (playerCar.getRole() == PlayerRoles:: DELIVERY && 
                playerCar.isHolding() && (board.getPackage(i).getX() == playerCar.getX() && 
                board.getPackage(i).getY() == playerCar.getY()))
                playerCar.updateScore(-8);

        // If YOU Hit An NPC Car Penelty
        for (int i = 0; i < board.getNpcCount(); i++)
            if (playerCar.getX() == board.getNpc(i).getX() && playerCar.getY() == board.getNpc(i).getY())
                playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -3 : -5);
    }
    else
    {
        playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
    }
}

void GameController::handlePrintableKeys(char key)
{
    if (key == 27) { exit(1); }

    switch (gameState)
    {
        case GameState::GAME_MENU:
            if (key == 't' || key == 'T') playerCar.setRole(PlayerRoles::TAXI);
            else if (key == 'd' || key == 'D') playerCar.setRole(PlayerRoles::DELIVERY);
            else if (key == 'r' || key == 'R') playerCar.setRole(rand() % 2);
            else if (key == 'l' || key == 'L') { gameState = GameState::GAME_SHOW_LEADERBOARD; break; }
            else break;
            gameState = GameState::GAME_INPUT_NAME;
            break;

        case GameState::GAME_INPUT_NAME:
            if (key == '\r')
                gameState = GameState::GAME_START;
            else if (key == '\b' || key == 127)
            {
                string str = playerCar.getName();
                if (!str.empty())
                {
                    str.pop_back();
                    playerCar.setName(str);
                }
            }
            else
            {
                string str = playerCar.getName() + key;
                playerCar.setName(str);
            }
            break;

        case GameState::GAME_START:
            if (key == 'f' || key == 'F') handleFuelRefill();
            else if (key == 'p' || key == 'P') handleRoleChange();
            else if (key == ' ') handlePickupAndDropoff();
            break;

        case GameState::GAME_SHOW_LEADERBOARD:
            if (key == 'm' || key == 'M') gameState = GameState::GAME_MENU;
            break;
    }
}

void GameController::update() 
{
    if (gameState != GameState::GAME_START) return;
    
    // Game Timer
    updateTime();

    // NPC Movement
    updateNPCs();

    // Check Game Status Win or Over
    checkGameStatus();
}

void GameController::updateTime() { gameTime -= 0.1; }

void GameController::updateNPCs()
{
    for (int i = 0; i < board.getNpcCount(); i++)
    {
        board.getNpc(i).setFrameCounter(board.getNpc(i).getFrameCounter() + 1);
        if (board.getNpc(i).getFrameCounter() >= board.getNpc(i).getSpeedDelay())
        {
            board.getNpc(i).setFrameCounter(0);

            int attemps = 0;
            const int MAX_ATTEMPTS = 20;
            while (attemps++ < MAX_ATTEMPTS)
            {
                int newY = board.getNpc(i).getY();
                int newX = board.getNpc(i).getX();
        
                int randDir = rand() % 4;
                switch (randDir)
                {
                    // Left
                    case 0: newX -= CELL_SIZE; break; 
                    // Right
                    case 1: newY += CELL_SIZE; break;
                    // Up
                    case 2: newY -= CELL_SIZE; break;
                    // Down
                    case 3: newY += CELL_SIZE; break;
                }

                // Convert to grid indices
                int y = (GRID_END_Y - newY) / CELL_SIZE;
                int x = (newX - GRID_START_X) / CELL_SIZE;

                // Check collision
                bool valid = board.isDrivable(y, x);
                for (int k = 0; k < board.getObstacleCount() && valid; k++)
                    if (board.getObstacles(k).overlaps(newX, newY))
                        valid = false;

                if (valid)
                {
                    board.getNpc(i).setX(newX);
                    board.getNpc(i).setY(newY);
                    break;
                }	
            }
        }
    }
}

void GameController::drawMenu() const 
{
    const int titleBoxWidth = 500;
    const int titleBoxHeight = 80;
    const int buttonWidth = 400;
    const int buttonHeight = 50;
    const int buttonSpacing = 65;
    
    // Center coordinates
    const int centerX = WIDTH / 2;
    const int centerY = HEIGHT / 2;
    
    // Title Box
    DrawRoundRect(centerX - titleBoxWidth / 2, centerY + 260, titleBoxWidth, titleBoxHeight, colors[DARK_RED], 20);
    DrawRoundRect(centerX - titleBoxWidth / 2 + 5, centerY + 260 + 5, titleBoxWidth - 10, titleBoxHeight - 10, colors[RED], 15);

    // Game Title with shadow effect
    DrawString(centerX - 90 + 2, centerY + 295 + 2, "R U S H  H O U R", colors[BLACK]); // Shadow
    DrawString(centerX - 90, centerY + 295, "R U S H  H O U R", colors[WHITE]); // Text

    // Background panel for the entire menu
    DrawRoundRect(centerX - (buttonWidth + 40) / 2, centerY - 220, buttonWidth + 40, buttonSpacing * 4 + 40, colors[SLATE_GRAY], 15);
    
    int startY = centerY + 70;
    
    // Taxi
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing + 3, buttonWidth, buttonHeight, colors[DARK_GRAY], 12); // Shadow
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing, buttonWidth, buttonHeight, colors[ORANGE], 10);
    DrawString(centerX - buttonWidth / 2 + 30, startY - buttonSpacing + 15, "[ T ] A X I  D R I V E R", colors[BLACK]);

    // Delivery
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing * 2 + 3, buttonWidth, buttonHeight, colors[DARK_GRAY], 12); // Shadow
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing * 2, buttonWidth, buttonHeight, colors[BLUE], 10);
    DrawString(centerX - buttonWidth / 2 + 30, startY - buttonSpacing * 2 + 15, "[ D ] E L I V E R Y  D R I V E R", colors[WHITE]);

    // Random
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing * 3 + 3, buttonWidth, buttonHeight, colors[DARK_GRAY], 12); // Shadow
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing * 3, buttonWidth, buttonHeight, colors[GREEN], 10);
    DrawString(centerX - buttonWidth / 2 + 30, startY - buttonSpacing * 3 + 15, "[ R ] A N D O M  D R I V E R", colors[BLACK]);

    // Leaderboard
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing * 4 + 3, buttonWidth, buttonHeight, colors[DARK_GRAY], 12); // Shadow
    DrawRoundRect(centerX - buttonWidth / 2, startY - buttonSpacing * 4, buttonWidth, buttonHeight, colors[ROYAL_BLUE], 10);
    DrawString(centerX - buttonWidth / 2 + 30, startY - buttonSpacing * 4 + 15, "[ L ] E A D E R B O A R D", colors[BLACK]);
}

void GameController::drawGame() const
{
    // Format time as mm:ss
    int minutes = int(gameTime) / 60;
    int seconds = int(gameTime) % 60;
    string timeStr = "Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);

    // Score and cash formatting
    string scoreStr = "Score: " + to_string(playerCar.getScore());
    ostringstream stream;
    stream << fixed << setprecision(2) << playerCar.getCash();
    string cashStr = "Cash: " + stream.str() + "$";

    // Role name and color
    bool isTaxi = playerCar.getRole() == PlayerRoles::TAXI;
    string roleStr = (isTaxi ? "Taxi" : "Delivery") + string(" Driver");
    float* roleColor = colors[isTaxi ? ORANGE : BLUE];

    // Draw info panel background
    DrawRoundRect(20, 920, 1160, 60, colors[playerCar.getRole() == PlayerRoles::TAXI ? DARK_ORANGE : LIGHT_CYAN], 10);

    // Draw labels
    DrawString(50, 945, timeStr, colors[BLACK]);
    DrawString(350, 945, scoreStr, colors[BLACK]);
    DrawString(650, 945, cashStr, colors[BLACK]);
    DrawString(950, 945, "Role: ", colors[BLACK]);
    DrawString(1010, 945, roleStr, roleColor);

    // Fuel Indicator Box
    float* fuelColor;
    int fuel = playerCar.getFuel();
    if (fuel >= 160)
        fuelColor = colors[GREEN];
    else if (fuel >= 100)
        fuelColor = colors[YELLOW];
    else if (fuel >= 60)
        fuelColor = colors[ORANGE];
    else
        fuelColor = colors[RED];

    // Draw Fuel Bar
    int fuelX = 60, fuelY = 420;
    DrawString(fuelX, fuelY + 250, "Fuel", colors[BLACK]);
    DrawRoundRect(fuelX + 6, fuelY + 26, 58, 208, colors[BLACK], 5);    // Border
    DrawRoundRect(fuelX + 8, fuelY + 28, 54, 204, colors[WHITE], 5);    // Background
    DrawRoundRect(fuelX + 10, fuelY + 30, 50, 200 * (fuel / float(MAX_FUEL)), fuelColor, 5); // Fill

    // Draw Board and Player
    board.draw(playerCar);
    playerCar.draw();
}

void GameController::drawGameOverScreen() const
{
    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;
    
    // Draw flashing effect
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if ((currentTime / 1000) % 2 == 0) 
        DrawRoundRect(centerX - 240, centerY + 30, 480, 90, colors[RED], 20);
    else
        DrawRoundRect(centerX - 240, centerY + 30, 480, 90, colors[DARK_RED], 20);

    // Main title box
    DrawString(centerX - 100, centerY + 75, "G A M E  O V E R", colors[WHITE]);
    
    // Player stats panel
    DrawRoundRect(centerX - 180, centerY - 160, 360, 160, colors[SLATE_GRAY], 10);
    DrawRoundRect(centerX - 170, centerY - 150, 340, 140, colors[WHITE], 5);
    
    // Final Score
    DrawString(centerX - 100, centerY - 70, "F I N A L  S C O R E", colors[DARK_RED]);
    DrawString(centerX - 20, centerY - 110, Num2Str(playerCar.getScore()), colors[GOLD]);
    
    // Restart prompt with pulsing effect
    if ((currentTime / 500) % 2 == 0) 
    {
        DrawRoundRect(centerX - 180, centerY - 300, 360, 50, colors[DARK_RED], 10);
        DrawString(centerX - 110, centerY - 280, "[ E S C ]  T O  E X I T", colors[WHITE]);
    }
    
    DrawString(30, 30, "Better luck next time, driver...", colors[LIGHT_GRAY]);
}

void GameController::drawWinScreen() const
{
    
    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;
    
    // Victory banner with particle effect
    DrawRoundRect(centerX - 100, centerY - 170, 200, 70, colors[ROYAL_BLUE], 15);
    DrawString(centerX - 60, centerY - 140, "YOU WIN!", colors[WHITE]);
    
    // Draw stars around the screen
    for (int i = 0; i < 20; i++) 
    {
        int x = GetRandInRange(50, WIDTH - 50);
        int y = GetRandInRange(50, HEIGHT - 50);
        int size = GetRandInRange(2, 8);
        DrawCircle(x, y, size, colors[GetRandInRange(0, COLOR_COUNT)]);
    }
    
    // Player stats panel
    DrawRoundRect(centerX - 180, centerY + 90, 360, 160, colors[SLATE_GRAY], 10);
    DrawRoundRect(centerX - 170, centerY + 100, 340, 140, colors[WHITE], 5);
    
    // Final Score
    DrawString(centerX - 100, centerY + 180, "F I N A L  S C O R E", colors[DARK_BLUE]);
    DrawString(centerX - 20, centerY + 140, Num2Str(playerCar.getScore()), colors[DARK_BLUE]);
    
    // Press ESC
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if ((currentTime / 500) % 2 == 0) 
        DrawString(centerX - 130, 50, "Press ESC to Exit Screen", colors[DARK_GRAY]);
}

void GameController::drawNameInputScreen() const
{
    DrawRoundRect(WIDTH/2 - 250, HEIGHT/2 - 150, 500, 300, colors[WHITE], 15.0);
    DrawRoundRect(WIDTH/2 - 255, HEIGHT/2 - 155, 510, 310, colors[LIGHT_BLUE], 18.0);

    // Draw header
    DrawString(WIDTH/2 - 140, HEIGHT/2 + 80, "PLAYER REGISTRATION", colors[DARK_BLUE]);

    // Draw question text with better styling
    DrawString(WIDTH/2 - 130, HEIGHT/2 + 20, "WHAT'S YOUR NAME:", colors[SLATE_BLUE]);

    // Draw input text with shadow effect
    DrawString(WIDTH/2 - 127, HEIGHT/2 - 17, playerCar.getName(), colors[GAINSBORO]);
    DrawString(WIDTH/2 - 130, HEIGHT/2 - 20, playerCar.getName(), colors[DARK_BLUE]);

    // Add a blinking cursor if name input is active
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if ((currentTime / 500) % 2 == 0) 
    {
        int textWidth = playerCar.getName().length() * 12; // Approximate width based on character count
        DrawLine(WIDTH/2 - 130 + textWidth + 4, HEIGHT/2 + 5, WIDTH/2 - 130 + textWidth + 4, HEIGHT/2 - 25, 2, colors[DEEP_PINK]);
    }

    // Draw instruction text
    DrawRoundRect(WIDTH/2 - 120, HEIGHT/2 - 100, 240, 40, colors[LAVENDER], 8.0);
    DrawString(WIDTH/2 - 50, HEIGHT/2 - 85, "E N T E R", colors[DARK_BLUE]);
}

void GameController::drawLeaderBoard() const 
{   
    // Leaderboard header
    DrawRoundRect(WIDTH / 2 - 300, HEIGHT - 150, 600, 100, colors[ROYAL_BLUE], 15);
    DrawString(WIDTH / 2 - 130, HEIGHT - 110, "L E A D E R B O A R D", colors[WHITE]);
    
    // Display leaderboard box
    DrawRoundRect(WIDTH / 2 - 250, HEIGHT / 2 - 300, 500, 600, colors[LIGHT_STEEL_BLUE], 10);
    DrawRoundRect(WIDTH / 2 - 240, HEIGHT / 2 - 290, 480, 580, colors[WHITE], 8);
    
    // Display leaderboard
    leaderboard.showLeaderboard();
    
    DrawRoundRect(WIDTH / 2 - 180, 80, 360, 50, colors[FOREST_GREEN], 10);
    DrawString(WIDTH / 2 - 70, 100, "[ M ] E N U", colors[WHITE]);
}
