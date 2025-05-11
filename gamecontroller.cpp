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
            if (playerCar.isHolding() && (board.getPassenger(i).getX() == playerCar.getX() && 
                                            board.getPassenger(i).getY() == playerCar.getY()))
                playerCar.updateScore(-5);
        
        // Running Over Package Penelty		
        for (int i = 0; i < board.getPackageCount(); i++)
            if (playerCar.isHolding() && (board.getPackage(i).getX() == playerCar.getX() && 
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

    string str;
    switch (gameState)
    {
        case GameState::GAME_MENU:
            if (key == 't' || key == 'T') playerCar.setRole(PlayerRoles::TAXI);
            else if (key == 'd' == key == 'D') playerCar.setRole(PlayerRoles::DELIVERY);
            else if (key == 'r' || key == 'R') playerCar.setRole(rand() % 2);
            else if (key == 'l' || key == 'L') { gameState = GameState::GAME_SHOW_LEADERBOARD; break; }
            gameState = GameState::GAME_INPUT_NAME;
            break;

        case GameState::GAME_INPUT_NAME:
            if (key == '\r')
                gameState = GameState::GAME_START;

            str = playerCar.getName() + key;
            playerCar.setName(str);
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
    // Title Box
    DrawRoundRect(WIDTH / 2 - 200, HEIGHT - 210, 400, 60, colors[RED], 20);
    DrawString(WIDTH / 2 - 90, HEIGHT - 190, "RUSH HOUR", colors[WHITE]);

    // Option Button Settings
    int buttonWidth = 360;
    int buttonHeight = 40;
    int startY = HEIGHT / 2 + 80;
    int gap = 55;

    // Button 1: Taxi Driver
    DrawRoundRect(WIDTH / 2 - buttonWidth / 2, startY, buttonWidth, buttonHeight, colors[ORANGE], 10);
    DrawString(WIDTH / 2 - 140, startY + 10, "[T] Play as Taxi Driver", colors[BLACK]);

    // Button 2: Delivery Driver
    DrawRoundRect(WIDTH / 2 - buttonWidth / 2, startY - gap, buttonWidth, buttonHeight, colors[BLUE], 10);
    DrawString(WIDTH / 2 - 140, startY - gap + 10, "[D] Play as Delivery Driver", colors[WHITE]);

    // Button 3: Random Role
    DrawRoundRect(WIDTH / 2 - buttonWidth / 2, startY - 2 * gap, buttonWidth, buttonHeight, colors[GREEN], 10);
    DrawString(WIDTH / 2 - 140, startY - 2 * gap + 10, "[R] Random Role", colors[BLACK]);

    // Button 4: Leaderboard
    DrawRoundRect(WIDTH / 2 - buttonWidth / 2, startY - 3 * gap, buttonWidth, buttonHeight, colors[YELLOW], 10);
    DrawString(WIDTH / 2 - 140, startY - 3 * gap + 10, "[L] Leaderboard", colors[BLACK]);    
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
    DrawRoundRect(20, 920, 1160, 60, colors[LIGHT_GRAY], 10);

    // Draw labels
    DrawString(50, 950, timeStr, colors[BLACK]);
    DrawString(350, 950, scoreStr, colors[BLACK]);
    DrawString(650, 950, cashStr, colors[BLACK]);
    DrawString(950, 950, "Role: ", colors[BLACK]);
    DrawString(1010, 950, roleStr, roleColor);

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
    // Full white background
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[WHITE]);

    // Draw a centered red banner for "GAME OVER"
    DrawRoundRect(WIDTH / 2 - 200, HEIGHT / 2 + 30, 400, 60, colors[RED], 15);
    DrawString(WIDTH / 2 - 80, HEIGHT / 2 + 50, "GAME OVER", colors[WHITE]);

    // Final Score Box
    DrawRoundRect(WIDTH / 2 - 180, HEIGHT / 2 - 10, 360, 50, colors[LIGHT_GRAY], 10);
    string scoreStr = "Final Score: " + to_string(playerCar.getScore());
    DrawString(WIDTH / 2 - 100, HEIGHT / 2 + 10, scoreStr, colors[BLACK]);
}

void GameController::drawWinScreen() const
{
    // Full white background
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[WHITE]);

    // Green banner for "CONGRATULATIONS!"
    DrawRoundRect(WIDTH / 2 - 240, HEIGHT / 2 + 30, 480, 60, colors[GREEN], 15);
    DrawString(WIDTH / 2 - 120, HEIGHT / 2 + 50, "CONGRATULATIONS!", colors[WHITE]);

    // YOU WIN message
    DrawRoundRect(WIDTH / 2 - 160, HEIGHT / 2 - 20, 320, 50, colors[FOREST_GREEN], 10);
    DrawString(WIDTH / 2 - 60, HEIGHT / 2, "YOU WIN!", colors[WHITE]);

    // Final Score Box
    DrawRoundRect(WIDTH / 2 - 180, HEIGHT / 2 - 90, 360, 40, colors[LIGHT_GRAY], 10);
    string scoreStr = "Final Score: " + to_string(playerCar.getScore());
    DrawString(WIDTH / 2 - 100, HEIGHT / 2 - 70, scoreStr, colors[BLACK]);
}

void GameController::drawNameInputScreen() const
{
    // Draw a gradient background
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[ALICE_BLUE]);
    for (int i = 0; i < HEIGHT; i += 2) {
        float alpha = 1.0f - (i / (float)HEIGHT) * 0.3f;
        float color[3] = {
            colors[ROYAL_BLUE][0] * alpha,
            colors[ROYAL_BLUE][1] * alpha,
            colors[ROYAL_BLUE][2] * alpha
        };
        DrawLine(0, i, WIDTH, i, 1, color);
    }

    // Draw a rounded rectangle panel
    DrawRoundRect(WIDTH/2 - 250, HEIGHT/2 - 150, 500, 300, colors[WHITE], 15.0);
    DrawRoundRect(WIDTH/2 - 255, HEIGHT/2 - 155, 510, 310, colors[LIGHT_BLUE], 18.0);

    // Draw header
    float headerColor[3] = {0.2, 0.3, 0.6}; // Dark blue
    DrawString(WIDTH/2 - 180, HEIGHT/2 + 80, "PLAYER REGISTRATION", headerColor);

    // Draw question text with better styling
    DrawString(WIDTH/2 - 180, HEIGHT/2 + 20, "WHAT'S YOUR NAME:", colors[SLATE_BLUE]);

    // Draw input text with shadow effect
    DrawString(WIDTH/2 - 177, HEIGHT/2 - 17, playerCar.getName(), colors[GAINSBORO]);
    DrawString(WIDTH/2 - 180, HEIGHT/2 - 20, playerCar.getName(), colors[DARK_BLUE]);

    // Draw an underline with gradient effect
    float lineColors[3][3] = {
        {colors[HOT_PINK][0], colors[HOT_PINK][1], colors[HOT_PINK][2]},
        {colors[DEEP_PINK][0], colors[DEEP_PINK][1], colors[DEEP_PINK][2]},
        {colors[VIOLET][0], colors[VIOLET][1], colors[VIOLET][2]}
    };

    for (int i = 0; i < 3; i++) {
        DrawLine(WIDTH/2 - 180, HEIGHT/2 - 10 + i, WIDTH/2 + 120, HEIGHT/2 - 10 + i, 1, lineColors[i]);
    }

    // Add a blinking cursor if name input is active
    static int blinkCounter = 0;
    blinkCounter = (blinkCounter + 1) % 60; // Blink every 60 frames
    if (blinkCounter < 30) 
    { // Show cursor for half the time
        int textWidth = playerCar.getName().length() * 12; // Approximate width based on character count
        DrawLine(WIDTH/2 - 180 + textWidth, HEIGHT/2 - 20, WIDTH/2 - 180 + textWidth, HEIGHT/2 - 40, 2, colors[DEEP_PINK]);
    }

    // Draw instruction text
    DrawRoundRect(WIDTH/2 - 120, HEIGHT/2 - 100, 240, 40, colors[LAVENDER], 8.0);
    DrawString(WIDTH/2 - 110, HEIGHT/2 - 80, "ENTER", colors[DARK_BLUE]);

    // Draw decorative elements
    DrawCircle(WIDTH/2 - 240, HEIGHT/2 - 140, 10, colors[LIGHT_CORAL]);
    DrawCircle(WIDTH/2 + 240, HEIGHT/2 - 140, 10, colors[LIGHT_CORAL]);
    DrawCircle(WIDTH/2 - 240, HEIGHT/2 + 130, 10, colors[LIGHT_CORAL]);
    DrawCircle(WIDTH/2 + 240, HEIGHT/2 + 130, 10, colors[LIGHT_CORAL]);

    // Draw a small car icon
    int carX = WIDTH/2 - 220;
    int carY = HEIGHT/2 + 20;
    // Car body
    DrawRoundRect(carX, carY, 40, 20, colors[CORAL], 5.0);
    // Car top
    DrawRoundRect(carX + 10, carY - 10, 20, 15, colors[CORAL], 3.0);
    // Wheels
    DrawCircle(carX + 10, carY + 25, 5, colors[BLACK]);
    DrawCircle(carX + 30, carY + 25, 5, colors[BLACK]);
}

void GameController::drawLeaderBoard() const
{
    DrawRectangle(0, 0, WIDTH, HEIGHT, colors[WHITE]);
    
    leaderboard.showLeaderboard();
}
