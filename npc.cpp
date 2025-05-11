#include "npc.h"

int NPC::frameCounter = 0;
int NPC::speedDelay = 10;

NPC::NPC(int x, int y, int frameCount, int speedDelay) : x(x), y(y) {}

int NPC::getX() const { return x; }
int NPC::getY() const { return y; }
void NPC::setX(int x) { this->x = x; }
void NPC::setY(int y) { this->y = y; }
int NPC::getFrameCounter() { return frameCounter; }
int NPC::getSpeedDelay() { return speedDelay; }
void NPC::setFrameCounter(int count) { frameCounter = count; }
void NPC::setSpeedDelay(int delay) { speedDelay = delay; }

void NPC::draw() const
{
    // Tyres
    DrawSquare(x + 5, y, 10, colors[BLACK]);
    DrawSquare(x + 25, y, 10, colors[BLACK]);

    // Body
    DrawRectangle(x + 5, y + 5, 30, 15, colors[VIOLET]);

    // Wind Screen
    DrawRectangle(x + 10, y + 15, 20, 15, colors[VIOLET]);
    DrawRectangle(x + 12, y + 20, 16, 8, colors[WHITE]);

    // Lights
    DrawCircle(x + 10, y + 13, 3, colors[WHITE]);
    DrawCircle(x + 28, y + 13, 3, colors[WHITE]);

    // Grill
    DrawLine(x + 15, y + 13, x + 23, y + 13, 1, colors[BLACK]);
    DrawLine(x + 15, y + 11, x + 23, y + 11, 1, colors[BLACK]);
}
