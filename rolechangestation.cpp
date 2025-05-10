#include "rolechangestation.h"

RoleChangeStation::RoleChangeStation(int x, int y) : x(x), y(y) {}

int RoleChangeStation::getX() const { return x; }
int RoleChangeStation::getY() const { return y; }

void RoleChangeStation::draw() const
{
    DrawSquare(x + 5, y, 30, colors[LIGHT_SLATE_GRAY]); // Garage
    DrawLine(x + 8, y + 20, x + 30, y + 20, 3, colors[BLACK]); // Shutters
    DrawLine(x + 8, y + 15, x + 30, y + 15, 3, colors[BLACK]); // Shutters
    DrawLine(x + 8, y + 10, x + 30, y + 10, 3, colors[BLACK]); // Shutters
    DrawTriangle(x + 20, y + 40, x, y + 25, x + 40, y + 25, colors[DARK_SLATE_GRAY]); // Roof
}

bool RoleChangeStation::overlaps(int carX, int carY) const { return carX == x && carY == y; }

void RoleChangeStation::changeRole(Player &p)
{
    p.toggleRole();
    p.setColor(colors[p.getRole() == PlayerRoles::TAXI ? ORANGE : BLUE]);
}