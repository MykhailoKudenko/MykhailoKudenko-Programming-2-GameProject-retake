<a name="readme-top"></a>

<!-- GENERAL GAME INFO -->
<br />
<div align="center">

  <h2 align="center">Ghosts 'n Goblins (nes)</h2>

  <p align="center">
    Short description of the game.
    <br />
    <strong>Original game : Ghosts 'n Goblins (NES) </strong>
    <a href="https://ghostsngoblins.fandom.com/wiki/Ghosts_%27n_Goblins"><strong>General info »</strong></a>
    ·
    <a href="https://www.youtube.com/watch?v=m2iehSbgkto&t=255s"><strong>Youtube video »<strong></a>
    <br />
    <br />
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#my-version">My version</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#how-to-play">How To Play</a></li>
    <li><a href="#class-structure">Class structure</a></li>
    <li><a href="#checklist">Checklist</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

![alt text](image.png)

Here's why:
* I personaly Liked this game
* This game met all criteria

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## My version

This section gives a clear and detailed overview of which parts of the original game I planned to make.

### The minimum I will most certainly develop:
* Player Movement
* Player Lance
* Two enemy type (Zombie/Plant)
* Miniboss (Flying Demon)
* Sounds
* MovingPlatforms

### What I will probably make as well:
* Final boss (Cyclops)
* All enemy types (Crows, Flying Knights, Flying Monsters)
* Different weapons
* Respawn System


### What I plan to create if I have enough time left:
* Easter eggs

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started
You would need to open as a project in Visual Studio (preferably 2022);

### Prerequisites

You would need to install Visual Studio from Official Website.
* Visual Studio 2022

### How to run the project

Open the solution in Visual Studio 2022 and run the project in Debug x64 mode.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- HOW TO PLAY -->
## How to play

Purpouse of the game is to get to the end of the game, and beat the final boss, and get as many points as possible.

### Controls

* Enter - Start the game

* A/D - move left/right
* W - jump
* S - Crouch
* E - Shoot your current Projectile


Debug:
* F1 - Toggle Collider Visibility 
* F2 - Toggle Immortality
* F3 - Toggle Flying

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CLASS STRUCTURE -->
## Class structure 

### Object composition 
Game
Game is composed of:

Camera
Player
Level
EntityManager
SoundManager
HUD
Menu textures

Purpose:
Main controller class that manages all game systems.

Player

Player is composed of:

Multiple Animation objects
Multiple Texture objects
Collider (Rectf)
State variables
Weapon/state enums

Purpose:
To Store movement, animations, combat state, and rendering data inside one object.

Level

Level is composed of:

Vertices
Ladders
Platforms
Enemy spawn points
Enemy spawn areas
Drop spawn points
Textures

Purpose:
Store all level geometry and spawn data.

EntityManager

EntityManager is composed of:

std::vector<Enemy*>
std::vector<Projectile*>
std::vector<Drop*>
std::vector<Effect*>

Purpose:
Responsible for spawning, updating, drawing, and deleting gameplay entities.

SoundManager

SoundManager is composed of:

SoundStream
Multiple SoundEffect objects

Purpose:
Handles music and sound effects.

Animation

Animation is composed of:

Texture
Frame data
Timing/playback variables

Purpose:
Handles sprite animation playback.

Other class relationships / design choices
Shared static resources

Some classes use static shared resources to avoid loading the same asset multiple times.

Examples:

Zombie
Effect
Drop

These classes use shared static textures/animations and an instance counter system.

Purpose:

Reduce memory usage
Prevent loading duplicate textures

### Inheritance 
Enemy hierarchy
Enemy
 ->Zombie
 -> Bird
 -> FlyingKnight
 -> Ghost
 -> Plant
 -> Demon
 -> Troll

Projectile hierarchy
Projectile
 -> Lance
 -> Knife
 -> Torch
 -> PlantProjectile
 -> DemonProjectile

Purpose:
All projectiles share movement/collision/death logic while each projectile has its own behavior and rendering.

Purpose:
All enemies share common enemy behavior while still having unique AI/attacks/animations.



<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CHECKLIST -->
## Checklist

- [x] Accept / set up github project
- [x] week 01 topics applied
    - [x] const keyword applied proactively (variables, functions,..)
    - [x] static keyword applied proactively (class variables, static functions,..)
    - [x] object composition (optional)
- [x] week 02 topics applied
- [x] week 03 topics applied
- [x] week 04 topics applied
- [x] week 05 topics applied
- [x] week 06 topics applied
- [x] week 07 topics applied
- [x] week 08 topics applied
- [x] week 09 topics applied (optional)
- [x] week 10 topics applied (optional)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Mykhailo Kudenko - Mykhailo.KUDENKO@student.howest.be

Project Link: https://github.com/HowestDAE/gd14-MykhailoKudenko

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments


* [Sprites](https://www.spriters-resource.com/nes/ghostsngoblins/)
* [Sounds](https://www.101soundboards.com/boards/11222-ghosts-n-goblins-sounds)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

