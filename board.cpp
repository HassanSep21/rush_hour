#include "board.h"

Board::Board(int fuelStationCount, int obstacleCount, int passengerCount, int packageCount, int npcCount) : 
        fuelStationCount(fuelStationCount), obstacleCount(obstacleCount), roleChangeStation(GRID_START_X, GRID_START_Y),
        passengerCount(passengerCount), packageCount(passengerCount), npcCount(npcCount)
{
    // Grid
    generateGrid();

    // Fuel Sations
    generateFuelStations();
    
    // Obstacles
    generateObstacles();

    // Passengers
    generatePassengers();

    // Packages
    generatePackages();

    // NPCs
    generateNpcs();
}

Board::~Board()
{
    // Delete Fuel Station
    delete[] fuelStations;

    // Delete Obstacles
    delete[] obstacles;

    // Delete Passengers
    delete[] passengers;

    // Delete Packages
    delete[] packages;

    // Delete NPCs
    delete[] npcs;;
}

int Board::getFuelStationCount() const { return fuelStationCount; }
FuelStation Board::getFuelStation(int i) { return fuelStations[i]; }

int Board::getObstacleCount() const { return obstacleCount; }
Obstacle Board::getObstacles(int i) { return obstacles[i]; }

int Board::getPassengerCount() const { return passengerCount; }
Passenger &Board::getPassenger(int i) { return passengers[i]; }

int Board::getPackageCount() const { return packageCount; }
Package &Board::getPackage(int i) { return packages[i]; }

int Board::getNpcCOunt() const { return npcCount; }
NPC &Board::getNpc(int i) { return npcs[i]; }

RoleChangeStation Board::getRolecChangeStation() { return roleChangeStation; }

void Board::generateGrid()
{
    int temp[20][20] = 
    {
        {0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0},
        {0,1,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0},
        {0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0},
        {0,1,0,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0},
        {0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0},
        {0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0},
        {0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0},
        {1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0},
        {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0},
        {1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0},
        {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0},
        {0,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0},
        {0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0},
        {0,1,0,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0},
        {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0},
        {0,1,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0},
        {1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}
    };

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = temp[i][j];

    int count = 0;
    while (count < 80)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;

        if (grid[randX][randY] == BoardObjects::BUILDING)
        {
            grid[randX][randY] = BoardObjects::ROAD;
            count++;
        }
    }
}
void Board::generateFuelStations()
{
    fuelStations = new FuelStation[fuelStationCount];

    int index = 0;
    while (index < fuelStationCount)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;

        if (isRoad(randY, randX) && !(randX == 0 && randY == 0))
        {
            fuelStations[index++] = FuelStation(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY);
            grid[randY][randX] = BoardObjects::FUEL_STATION;
        }
    }
}
void Board::generateObstacles()
{
    obstacles = new Obstacle[obstacleCount];

    int index = 0;
    while (index < obstacleCount)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;

        if (isRoad(randY, randX) && !(randX == 0 && randY == 0))
        {
            obstacles[index++] = Obstacle(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY, rand() % 2);
            grid[randY][randX] = BoardObjects::OBSTACLE;
        }
    }
}
void Board::generatePassengers()
{
    passengers = new Passenger[passengerCount];

    int index = 0;
    while (index < passengerCount)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;
        int randDesX = rand() % 20;
        int randDesY = rand() % 20;

        if (isRoad(randY, randX) && isRoad(randDesY, randDesX) && !(randX == 0 && randY == 0) && !(randX == randDesX && randY == randDesY))
            passengers[index++] = Passenger(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY,
                                            GRID_START_X + CELL_SIZE * randDesX, GRID_END_Y - CELL_SIZE * randDesY);
    }
}
void Board::generatePackages()
{
    packages = new Package[packageCount];

    int index = 0;
    while (index < packageCount)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;
        int randDesX = rand() % 20;
        int randDesY = rand() % 20;

        if (isRoad(randY, randX) && isRoad(randDesY, randDesX) && !(randX == 0 && randY == 0) && !(randX == randDesX && randY == randDesY))
            packages[index++] = Package(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY,
                                        GRID_START_X + CELL_SIZE * randDesX, GRID_END_Y - CELL_SIZE * randDesY);
    }
}
void Board::generateNpcs()
{
    npcs = new NPC[npcCount];

    int index = 0;
    while (index < npcCount)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;

        if (isRoad(randY, randX) && !(randX == 0 && randY == 0))
            npcs[index++] = NPC(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY);
    }
}

void Board::increaseNpcs()
{
    NPC *tmp = new NPC[npcCount + 1];

    for (int i = 0; i < npcCount; i++)
        tmp[i] = npcs[i];

    while (true)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;

        if (isRoad(randY, randX) && !(randX == 0 && randY == 0))
        {
            tmp[npcCount] = NPC(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY);
            break;
        }
    }

    delete[] npcs;
    npcs = tmp;
    npcCount++;
}

void Board::setRandomPos(Passenger &p)
{
    while (true)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;
        int randDesX = rand() % 20;
        int randDesY = rand() % 20;

        if (isRoad(randY, randX) && isRoad(randDesY, randDesX) && !(randX == 0 && randY == 0) && !(randX == randDesX && randY == randDesY))
        {
            p = Passenger(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY,
                            GRID_START_X + CELL_SIZE * randDesX, GRID_END_Y - CELL_SIZE * randDesY);
            break;
        }
    }
}
void Board::setRandomPos(Package &p)
{
    while (true)
    {
        int randX = rand() % 20;
        int randY = rand() % 20;
        int randDesX = rand() % 20;
        int randDesY = rand() % 20;

        if (isRoad(randY, randX) && isRoad(randDesY, randDesX) && !(randX == 0 && randY == 0) && !(randX == randDesX && randY == randDesY))
        {
            p = Package(GRID_START_X + CELL_SIZE * randX, GRID_END_Y - CELL_SIZE * randY,
                        GRID_START_X + CELL_SIZE * randDesX, GRID_END_Y - CELL_SIZE * randDesY);
            break;
        }
    }
    
}

bool Board::isRoad(int i, int j) const { return i >= 0 && i < SIZE && j >= 0 && j < SIZE && grid[i][j] == BoardObjects::ROAD; }
bool Board::isDrivable(int i, int j) const 
{ 
    return i >= 0 && i < SIZE && j >= 0 && j < SIZE && (grid[i][j] == BoardObjects::ROAD || 
                                                        grid[i][j] == BoardObjects::FUEL_STATION || 
                                                        grid[i][j] == BoardObjects::ROLE_CHANGE_STATION); 
}

void Board::draw(Player &playerCar) const
{
    // Background
    DrawSquare(GRID_START_X, GRID_START_Y, 800, colors[GRAY]);

    // Buildings
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (grid[i][j] == BoardObjects::BUILDING)
                DrawSquare(GRID_START_X + CELL_SIZE * j, GRID_END_Y - CELL_SIZE * i, CELL_SIZE, colors[BLACK]);

    // Fuel Stations
    for (int i = 0; i < fuelStationCount; i++)
        fuelStations[i].draw();

    // Obstactes
    for (int i = 0; i < obstacleCount; i++)
        obstacles[i].draw();

    // Role Change Station
    roleChangeStation.draw();

    // Passengers & Packages
    if (playerCar.getRole() == PlayerRoles::TAXI)
        for (int i = 0; i < passengerCount; i++)
            passengers[i].draw();
    else if (playerCar.getRole() == PlayerRoles::DELIVERY)
        for (int i = 0; i < packageCount; i++)
            packages[i].draw();

    // NPCs
    for (int i = 0; i < npcCount; i++)
        npcs[i].draw();

    // Grid Lines
    for (int i = 0; i <= SIZE; i++)
    {
        DrawLine(200, 900 - 40 * i, 1000, 900 - 40 * i, 2, colors[BLACK]);
        DrawLine(200 + 40 * i, 900, 200 + 40 * i, 100, 2, colors[BLACK]);
    }
}
