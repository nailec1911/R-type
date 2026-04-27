# R-Type Superior

**\[EDIT] This project was previously on a gitlab repo. Hence, the project wiki might not be accessible.**

## Table of contents

- [R-Type Superior](#r-type-superior)
  - [Table of contents](#table-of-contents)
  - [Overview](#overview)
    - [The game](#the-game)
    - [Features](#features)
  - [Getting Started](#getting-started)
    - [Installation](#installation)
      - [Run the Game](#run-the-game)
    - [Level edition](#level-edition)
  - [Documentation](#documentation)
  - [Contributing](#contributing)
  - [Authors](#authors)
  - [License](#license)

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
    ./r-type_server PORT
    ```

- Start a client instance:

    ```bash
    ./r-type_client SERVER_IP PORT
    ```

To understand how to play, go check our [wiki](https://gitlab.com/epitech-it-2027/rts/-/wikis/home/Getting-started/Usage).

### Level edition

You can modify configuration files to create your own levels.

## Documentation

For detailed informations, don't forget to check our [wiki](https://gitlab.com/epitech-it-2027/rts/-/wikis/home) !

## Contributing

if you want to contribute, you can create a new branch `feat-<what you want to add>` push, and make a pull request.
For more details, check our [Contributing page](https://gitlab.com/epitech-it-2027/rts/-/wikis/home/Getting-started/Contributing).

## Authors

* [Célian Lombardot](https://github.com/nailec1911)
* [Théophile Combes](https://github.com/Zetsumeii)
* [Adam Levy](https://github.com/adamdelacheisserie)
* [Marius Marolleau](https://github.com/Ciikorrr)

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](./LICENSE.md)
