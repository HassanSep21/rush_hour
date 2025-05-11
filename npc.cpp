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
    DrawSquare(x + 5, y + 5, 30, colors[INDIGO]);
}
