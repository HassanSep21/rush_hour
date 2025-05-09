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

/*=======================================*//* MY HEADERS *//*=======================================*/

#include "car.h"
#include "passenger.h"
#include "package.h"
#include "fuelstation.h"
#include "rolechangestation.h"
#include "player.h"

/*=======================================*//* My Classes *//*=======================================*/

struct ObstacleType
{
	static const int TREE = 0;
	static const int BOX = 1;
};

class Obstacle
{
private:
	int x;
	int y;
	int type;

public:
	Obstacle(int x = 40, int y = 40, int type = ObstacleType::TREE) : x(x), y(y), type(type) {}

	int getX() const { return x; }
	int getY() const { return y; }
	int getType() const { return type; }
	void setType(int type) { this->type = type; }

	void draw()
	{
		if (type == ObstacleType::TREE)
		{
			// Trunk
			DrawRectangle(x + 15, y + 5, 10, 15, colors[BROWN]);

			// Leafs
			DrawCircle(x + 20, y + 30, 7, colors[DARK_GREEN]);
			DrawCircle(x + 15, y + 22, 7, colors[DARK_GREEN]);
			DrawCircle(x + 25, y + 22, 7, colors[DARK_GREEN]);

			// Roots
			DrawTriangle(x + 20, y + 15, x + 12, y, x + 28, y, colors[BROWN]);
		}
		else if (type == ObstacleType::BOX)
		{
			DrawSquare(x + 5, y + 5, 30, colors[BROWN]); // Box
			
			// Mark
			DrawLine(x + 10, y + 28, x + 30, y + 10, 5, colors[RED]);
			DrawLine(x + 10, y + 10, x + 30, y + 28, 5, colors[RED]);
		}
	}

	bool overlaps(int carX, int carY) const { return carX == x && carY == y; }
};

/*=======================================*//* My Classes *//*=======================================*/
using namespace std;

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */

float gameTime = 180;

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
Passenger passenger(400, 300, 400, 500);
Package package(480, 380, 480, 580);
FuelStation fuelStation(480, 500);
RoleChangeStation roleStation(480, 420);
Obstacle tree(320, 220, ObstacleType::TREE);
Obstacle box(360, 220, ObstacleType::BOX);

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
	
	DrawString(50, 950, timeStr, colors[BLACK]);
	DrawString(550, 950, scoreStr, colors[BLACK]);
	DrawString(1000, 950, cashStr, colors[BLACK]);

	// Fuel Indicator
	float *fuelColor;

	if (playerCar.getFuel()  >= 80)
		fuelColor = colors[GREEN];
	else if (playerCar.getFuel()  >= 50)
		fuelColor = colors[YELLOW];
	else if (playerCar.getFuel()  >= 30)
		fuelColor = colors[ORANGE];
	else
		fuelColor = colors[RED];

	DrawRoundRect(66, 446, 58, 208, colors[BLACK], 5);
	DrawRoundRect(68, 448, 54, 204, colors[WHITE], 5);
	DrawRoundRect(70, 450, 50, 200 * (playerCar.getFuel() / float(MAX_FUEL)) + 1, fuelColor, 5);

	// Testing Objects
	if (playerCar.getRole() == PlayerRoles::TAXI)
		passenger.draw();
	else
		package.draw();

	fuelStation.draw();
	roleStation.draw();
	playerCar.draw();
	tree.draw();
	box.draw();

	// Grid
	for (int i = 0; i < 21; i++)
	{
		DrawLine(200, 900 - 40 * i, 1000, 900 - 40 * i, 2, colors[BLACK]);
		DrawLine(200 + 40 * i, 900, 200 + 40 * i, 100, 2, colors[BLACK]);
	}
	
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
	if (key == GLUT_KEY_LEFT)
    {
		playerCar.moveLeft();
		if (tree.overlaps(playerCar.getX(), playerCar.getY()) || box.overlaps(playerCar.getX(), playerCar.getY()))
		{
			playerCar.moveRight();
			playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
		}
	}
    else if (key == GLUT_KEY_RIGHT)
    {
		playerCar.moveRight();
		if (tree.overlaps(playerCar.getX(), playerCar.getY()) || box.overlaps(playerCar.getX(), playerCar.getY()))
		{
			playerCar.moveLeft();
			playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
		}
	}
    else if (key == GLUT_KEY_UP)
    {
		playerCar.moveUp();
		if (tree.overlaps(playerCar.getX(), playerCar.getY()) || box.overlaps(playerCar.getX(), playerCar.getY()))
		{
			playerCar.moveDown();
			playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
		}
	}
	else if (key == GLUT_KEY_DOWN)
    {
		playerCar.moveDown();
		if (tree.overlaps(playerCar.getX(), playerCar.getY()) || box.overlaps(playerCar.getX(), playerCar.getY()))
		{
			playerCar.moveUp();
			playerCar.updateScore(playerCar.getRole() == PlayerRoles::TAXI ? -2 : -4);
		}
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
		if (fuelStation.overlaps(playerCar.getX(), playerCar.getY())) 
			fuelStation.reFuel(playerCar);
	}

	if (key == 'p' || key == 'P')
	{
		if (roleStation.overlaps(playerCar.getX(), playerCar.getY()))
			roleStation.changeRole(playerCar);
	}

	if (key == ' ')
    {
		if (playerCar.getRole() == PlayerRoles::TAXI)
		{
			if (!passenger.isPickedUp() && passenger.overlaps(playerCar.getX(), playerCar.getY())) 
				passenger.setPickedUp(true);

			if (passenger.isPickedUp() && passenger.overlaps(playerCar.getX(), playerCar.getY())) 
			{
				passenger.setReached(true);
				playerCar.updateScore(10);
				playerCar.updateCash(passenger.getFair());
				playerCar.jobCompleted();
			}
		}
		else if (playerCar.getRole() == PlayerRoles::DELIVERY)
		{
			if (!package.isPickedUp() && package.overlaps(playerCar.getX(), playerCar.getY())) 
				package.setPickedUp(true);

			if (package.isPickedUp() && package.overlaps(playerCar.getX(), playerCar.getY())) 
			{
				package.setDroppedOff(true);
				playerCar.updateScore(20);
				playerCar.updateCash(package.getFee());
				playerCar.jobCompleted();
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
	srand(time(0));

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
