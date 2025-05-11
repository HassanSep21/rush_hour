#include "player.h"

Player::Player(int role) : role(role), Car(GRID_START_X, GRID_END_Y, colors[(role == PlayerRoles::TAXI) ? ORANGE : BLUE]), 
                name(""), cash(STARTING_CASH), score(0), jobsCompleted(0), holding(false) {}

string Player::getName() const { return name; }
void Player::setName(string name) { this->name = name; }
float Player::getCash() const { return cash; }
void Player::updateCash(float amount) { cash += amount; }
int Player::getScore() const { return score; }
void Player::updateScore(int points) { score += points; }
int Player::getRole() const { return role; }
void Player::toggleRole() { this->role = role == PlayerRoles::TAXI ? PlayerRoles::DELIVERY : PlayerRoles::TAXI; }
bool Player::isHolding() const { return holding; }
void Player::setHolding(bool state) { holding = state; }
int Player::getJobsCompleted() const { return jobsCompleted; }
void Player::setRole(int role) { this->role = role; }
void Player::jobCompleted() 
{ 
    jobsCompleted++;
    holding = false;
}
