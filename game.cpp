//============================================================================
// Name        : .cpp
// Author      : Dr. Sibt Ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include <string>
#include <cmath>

// seed the random numbers generator by current time (see the documentation of srand for further help)...
struct RandomSeeder
{
	RandomSeeder() { srand(time(0)); }
};
RandomSeeder randSeed;

/*=======================================*//* MY HEADERS *//*=======================================*/

#include "car.h"
#include "passenger.h"
#include "package.h"
#include "fuelstation.h"
#include "rolechangestation.h"
#include "obstacle.h"
#include "player.h"

using namespace std;

/*=======================================*//* My Class *//*=======================================*/

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

public:
	Board(int fuelStationCount = 2, int obstacleCount = 4, int passengerCount = 2, int packageCount = 2) : 
			fuelStationCount(fuelStationCount), obstacleCount(obstacleCount), roleChangeStation(GRID_START_X, GRID_START_Y),
			passengerCount(passengerCount), packageCount(passengerCount)
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
	}

	~Board()
	{
		// Delete Fuel Station
		delete[] fuelStations;

		// Delete Obstacles
		delete[] obstacles;

		// Delete Passengers
		delete[] passengers;

		// Delete Packages
		delete[] packages;
	}

	int getFuelStationCount() const { return fuelStationCount; }
	FuelStation getFuelStation(int i) { return fuelStations[i]; }

	int getObstacleCount() const { return obstacleCount; }
	Obstacle getObstacles(int i) { return obstacles[i]; }

	int getPassengerCount() const { return passengerCount; }
	Passenger &getPassenger(int i) { return passengers[i]; }

	int getPackageCount() const { return packageCount; }
	Package &getPackage(int i) { return packages[i]; }

	RoleChangeStation getRolecChangeStation() { return roleChangeStation; }

	void generateFuelStations()
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
	void generateObstacles()
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
	void generatePassengers()
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

	void generatePackages()
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

	void setRandomPos(Passenger &p)
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

	void setRandomPos(Package &p)
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

	bool isRoad(int i, int j) const { return i >= 0 && i < SIZE && j >= 0 && j < SIZE && grid[i][j] == BoardObjects::ROAD; }
	bool isDrivable(int i, int j) const 
	{ 
		return i >= 0 && i < SIZE && j >= 0 && j < SIZE && (grid[i][j] == BoardObjects::ROAD || 
															grid[i][j] == BoardObjects::FUEL_STATION || 
															grid[i][j] == BoardObjects::ROLE_CHANGE_STATION); 
	}

	void generateGrid()
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

		for (int i = 0; i < 100; i++)
		{
			int randX = rand() % 20;
			int randY = rand() % 20;

			if (grid[randX][randY] == BoardObjects::BUILDING)
				grid[randX][randY] = BoardObjects::ROAD;
		}
	}

	void draw(Player &playerCar) const
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

		// Grid Lines
		for (int i = 0; i <= SIZE; i++)
		{
			DrawLine(200, 900 - 40 * i, 1000, 900 - 40 * i, 2, colors[BLACK]);
			DrawLine(200 + 40 * i, 900, 200 + 40 * i, 100, 2, colors[BLACK]);
		}
	}

};

/*=======================================*//* My Classes *//*=======================================*/

float gameTime = 180;

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

/*=======================================*//* Class Objects *//*=======================================*/

Player playerCar(rand() % 2);
// Package package(480, 380, 480, 580);

int randFuel = 2 + rand() % 2;
int randObs = 4 + rand() % 3;
int randPass = 2 + rand() % 3;
int randPack = 2 + rand() % 2;

Board board(randFuel, randObs, randPass, randPack);

/*=======================================*//* Class Objects *//*=======================================*/

/*
 * Main Canvas drawing function.
 */
void GameDisplay()/**/
{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(1/*Red Component*/, 1/*Green Component*/, 1/*Blue Component*/, 0/*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

	//Display Player Stats
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

	if (playerCar.getFuel()  >= 160)
		fuelColor = colors[GREEN];
	else if (playerCar.getFuel()  >= 100)
		fuelColor = colors[YELLOW];
	else if (playerCar.getFuel()  >= 60)
		fuelColor = colors[ORANGE];
	else
		fuelColor = colors[RED];

	DrawRoundRect(66, 446, 58, 208, colors[BLACK], 5);
	DrawRoundRect(68, 448, 54, 204, colors[WHITE], 5);
	DrawRoundRect(70, 450, 50, 200 * (playerCar.getFuel() / float(MAX_FUEL)), fuelColor, 5);

	// Testing Objects
	board.draw(playerCar);
	playerCar.draw();
	
	glutSwapBuffers(); // do not modify this line..
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */
void NonPrintableKeys(int key, int x, int y)
{
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
	}
	else
	{
		playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/
	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y)
{
	if (key == 27/* Escape key ASCII*/)
    {
		exit(1); // exit the program when escape key is pressed.
	}

	if (key == 'f' || key == 'F')
	{
		for (int i = 0; i < board.getFuelStationCount(); i++)
		{
			if (board.getFuelStation(i).overlaps(playerCar.getX(), playerCar.getY()))
				board.getFuelStation(i).reFuel(playerCar);
		}
	}

	if (key == 'p' || key == 'P')
	{
		if (board.getRolecChangeStation().overlaps(playerCar.getX(), playerCar.getY()))
			board.getRolecChangeStation().changeRole(playerCar);
	}

	if (key == ' ')
    {
		if (playerCar.getRole() == PlayerRoles::TAXI)
		{
			for (int i = 0; i < board.getPassengerCount(); i++)
			{
				Passenger &p = board.getPassenger(i);
				if (!p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
				{
					p.setPickedUp(true);
					break;
				}

				if (p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
				{
					board.setRandomPos(p);

					p.setReached(true);
					playerCar.updateScore(10);
					playerCar.updateCash(p.getFair());
					playerCar.jobCompleted();
				}
			}
		}
		else if (playerCar.getRole() == PlayerRoles::DELIVERY)
		{
			for (int i = 0; i < board.getPackageCount(); i++)
			{
				Package &p = board.getPackage(i);
				if (!p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
				{
					p.setPickedUp(true);
					break;
				}

				if (p.isPickedUp() && p.overlaps(playerCar.getX(), playerCar.getY())) 
				{
					p.setDroppedOff(true);
					playerCar.updateScore(20);
					playerCar.updateCash(p.getFee());
					playerCar.jobCompleted();
				}
			}
		}
	}

	glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m)
{
	// implement your functionality here
	gameTime -= 0.1;
	glutPostRedisplay();

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y)
{
	glutPostRedisplay();
}

void MouseMoved(int x, int y)
{
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
    {
		// cout << GLUT_DOWN << " " << GLUT_UP << endl;
	}
    else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
    {
        // cout << GLUT_DOWN << " " << GLUT_UP << endl;
	}
    
	glutPostRedisplay();
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[])
{
	int width = WIDTH, height = HEIGHT;
    
	InitRandomizer(); // seed the random number generator...
	// srand(time(0));

	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Hassan Ahmed | 24i-2521"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters

	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}

#endif /* RushHour_CPP_ */
