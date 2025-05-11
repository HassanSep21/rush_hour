//============================================================================
// Name        : game.cpp
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

/*========================================*//* MY HEADERS *//*=======================================*/

#include "board.h"
#include "car.h"
#include "fuelstation.h"
#include "npc.h"
#include "obstacle.h"
#include "package.h"
#include "passenger.h"
#include "player.h"
#include "rolechangestation.h"

using namespace std;
/*====================================================================================================*/

/*=========================================*//* My Class *//*=========================================*/

/*=====================================================================================================*/

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

int randFuel = 2 + rand() % 2;
int randObs = 4 + rand() % 3;
int randPass = 2 + rand() % 3;
int randPack = 2 + rand() % 2;

Board board(randFuel, randObs, randPass, randPack);

/*====================================================================================================*/

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
		for (int i = 0; i < board.getNpcCOunt(); i++)
			if (playerCar.getX() == board.getNpc(i).getX() && playerCar.getY() == board.getNpc(i).getY())
				playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -3 : -5);
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
					playerCar.updateCash(p.getFair());
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

		if (playerCar.getjobsCompleted() != 0 && playerCar.getjobsCompleted() % 2 == 0)
		{
			NPC::setSpeedDelay(NPC::getSpeedDelay() - 1);
			board.increaseNpcs();
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
	// Game Timer
	gameTime -= 0.1;

	// NPC Movement
	for (int i = 0; i < board.getNpcCOunt(); i++)
	{
		board.getNpc(i).setFrameCounter(board.getNpc(i).getFrameCounter() + 1);
		if (board.getNpc(i).getFrameCounter() >= board.getNpc(i).getSpeedDelay())
		{
			board.getNpc(i).setFrameCounter(0);

			while (true)
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
