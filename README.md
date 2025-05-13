# RUSH HOUR OOP

![Project Screenshot](.assets/gameprev.png)

**Rush Hour OOP** is a traffic management simulation game built using Object-Oriented Programming principles. Players navigate through city traffic, picking up passengers, delivering packages, and managing fuel while avoiding obstacles.

---

## Table of Contents

1. [Features](#features)
2. [Setup Instructions](#setup-instructions)
3. [How to Play](#how-to-play)
4. [Code Structure](#code-structure)
5. [Gameplay Preview](#gameplay-preview)

---

## Features

* Complete object-oriented design with multiple classes
* Dynamic traffic simulation with NPCs
* Role-changing mechanics (switch between passenger delivery and package delivery)
* Fuel management system
* Real-time leaderboard tracking
* Multiple obstacle types to navigate around

---

## Setup Instructions

### Prerequisites

* C++ compiler (G++)
* Required libraries (list your specific dependencies here)

### Steps

1. Clone the repository:

```bash
git clone https://github.com/yourusername/rush-hour-oop.git
cd rush-hour-oop
make
./rush-hour
```

## How to Play

1. Control your vehicle through traffic using arrow keys or WASD
2. Pick up passengers/packages by moving over them
3. Deliver to highlighted destinations to earn points
4. Visit fuel stations to refill your tank
5. Use role-change stations to switch between passenger and package delivery modes
6. Avoid obstacles to prevent damage and slowdowns
7. Aim for a high score on the leaderboard

## Code Structure

The game is built using the following classes:

* **GameController**: Manages overall game flow and state
* **Player**: Handles player vehicle movement and status
* **Car**: Base class for all vehicles in the game
* **NPC**: Controls non-player vehicle behavior
* **Passenger**: Represents passengers to be picked up and delivered
* **Package**: Represents packages to be collected and delivered
* **FuelStation**: Allows the player to refill their vehicle
* **RoleChangeStation**: Enables switching between different gameplay modes
* **Obstacle**: Represents various obstacles in the game world
* **Board**: Manages the game board/map layout
* **Leaderboard**: Tracks and displays high scores

## Gameplay Preview

![Gameplay Demo](.assets/gameplay.gif)
