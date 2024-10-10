# R-Type Superior

## Table of contents

1. [Overview](#overview)
    1. [The game](#the-game)
    2. [Features](#features)
2. [Getting started](#getting-started)
    1. [Installation](#installation)
    2. [Run the Game](#run-the-game)
    3. [Controls](#controls)
    4. [Multiplayer](#multiplayer)
    5. [Level edition](#level-edition)
3. [Documentation](#documentation)
4. [Contributing](#contributing)
5. [Authors](#authors)

## Overview

### The game

This project is a multiplayer side-scrolling shooter game inspired by the classic arcade game [R-Type](https://en.wikipedia.org/wiki/R-Type). The goal of this project is to create a fully functional game using **C++**, while learning about game development, networking, and software architecture.

Players control a spaceship, navigating through various levels, defeating enemies, and avoiding obstacles. The game supports multiplayer mode, where players can join a game session hosted on a server and play together.

### Features

- **2D Graphics Rendering**: Real-time sprite-based graphics, animations, and particle effects.
- **Multiplayer Support**: Join and play with other players via a networked client-server architecture.
- **Classic R-Type Gameplay**: Side-scrolling shooter with player ship control, enemies, power-ups, and boss fights.

## Getting Started

### Installation

1. **Clone the Repository**:

   ```bash
   git clone git@gitlab.com:epitech-it-2027/rts.git
   cd rts
   ```

2. **Build the Project**:

#### Run the Game

- Start the server:

    ```bash
    ./rtype_server
    ```

- Start a client instance:

    ```bash
    ./rtype_client
    ```

### Controls

- Arrow Keys: Move the spaceship (Up, Down, Left, Right)
- Space: Shoot
- P: Pause the game
- Esc: Exit the game

### Multiplayer

- Ensure the server is running before clients connect.
- Clients connect to the server using the server's IP address and designated port.
- Players join a lobby and can start the game once all players are ready.

### Level edition

You can modify configuration files to create your own levels.

## Documentation

check the doc

## Contributing

see the doc
Create a new branch `feat-<what you want to add>` push, and make a pull request.

## Authors

* [Célian Lombardot](https://github.com/nailec1911)
* [Théophile Combes](https://github.com/Zetsumeii)
* [Adam Levy](https://github.com/adamdelacheisserie)
* [Marius Marolleau](https://github.com/Ciikorrr)

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE.md)
