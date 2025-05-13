//============================================================================
// Name        : i242521project.cpp
// Author      : Hassan Ahmed
// Version     : v1.00
// Copyright   : (c) Reserved
// Description : Rush Hour Game
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_

#include "util.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>

// seed the random numbers generator by current time (see the documentation of srand for further help)...
struct RandomSeeder
{
	RandomSeeder() { srand(time(0)); }
};
RandomSeeder randSeed;

/*========================================*//* MY HEADERS *//*======================================*/

#include "board.h"
#include "car.h"
#include "fuelstation.h"
#include "gamecontroller.h"
#include "leaderboard.h"
#include "npc.h"
#include "obstacle.h"
#include "package.h"
#include "passenger.h"
#include "player.h"
#include "rolechangestation.h"

using namespace std;

/*=======================================*//* Game Object *//*=======================================*/

GameController game;

/*====================================================================================================*/

void SetCanvasSize(int width, int height)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void GameDisplay()
{
	glClearColor(1, 1, 1, 0);
	glClear (GL_COLOR_BUFFER_BIT);

	game.render();
	
	glutSwapBuffers();
}

void NonPrintableKeys(int key, int x, int y)
{
	game.handleNonPrintableKeys(key);

	glutPostRedisplay();
}

void PrintableKeys(unsigned char key, int x, int y)
{
	game.handlePrintableKeys(key);

	glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 * */
void Timer(int m)
{
	game.update();

	glutPostRedisplay();

	// calls Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

int main(int argc, char*argv[])
{
	int width = WIDTH, height = HEIGHT;
    
	InitRandomizer(); 

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(230, 50); 
	glutInitWindowSize(width, height); 
	glutCreateWindow("Hassan Ahmed | 24i-2521");
	SetCanvasSize(width, height); 

	glutDisplayFunc(GameDisplay); 
	glutSpecialFunc(NonPrintableKeys);
	glutKeyboardFunc(PrintableKeys);

	glutTimerFunc(1000.0, Timer, 0);

	glutMainLoop();
	return 1;
}

#endif /* RushHour_CPP_ */
