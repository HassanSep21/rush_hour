#include "player.h"

Player::Player(int role) : role(role ? role : rand() % 2), Car(200, 100, colors[role == PlayerRoles::TAXI ? ORANGE : BLUE]), cash(STARTING_CASH), score(0), jobsCompleted(0) {}

string Player::getName() const { return name; }
void Player::setName(string name) { this->name = name; }
float Player::getCash() const { return cash; }
int Player::getScore() const { return score; }
int Player::getRole() const { return role; }
void Player::toggleRole() { this->role = role == PlayerRoles::TAXI ? PlayerRoles::DELIVERY : PlayerRoles::TAXI; }

void Player::jobCompleted() { jobsCompleted++; }

void Player::updateScore(int points) { score += points; }
void Player::updateCash(float amount) { cash += amount; }
