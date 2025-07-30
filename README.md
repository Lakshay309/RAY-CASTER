# 🎮 RayCaster

A basic raycasting engine in C++ using **SFML**, inspired by early 90s shooters like Wolfenstein 3D.

---

## ⚡ Features

* Real-time 3D-style rendering using 2D raycasting
* Simple wall collision handling
* Distance-based wall shading
* Player movement + rotation
* FPS counter
* ESC key exits cleanly

---

## 📸 Screenshots & Recordings

You can showcase your game by adding screenshots or short recordings here.
Example:

![Screenshot](https://github.com/Lakshay309/RAY-CASTER/blob/master/Screenshot%202025-07-30%20184254.png)
🎥 [Watch Gameplay](https://github.com/Lakshay309/RAY-CASTER/blob/master/Screen%20Recording%202025-07-30%20185032.mp4)

---

## 🕹 Controls

* `W` - Move forward
* `S` - Move backward
* `A` - Rotate left
* `D` - Rotate right

---

## 🛠 Build Instructions

### Requirements

* C++17 or higher
* [SFML 3.0](https://www.sfml-dev.org/download/)

### Steps (Visual Studio)

1. Clone this repo
2. Create a new empty project in Visual Studio
3. Add these source files to your project:

   * `main.cpp`
   * `raycaster.cpp`
   * `player.cpp`
   * `player.h`
4. Link SFML libraries:

   * `sfml-graphics`
   * `sfml-window`
   * `sfml-system`
5. Place `arial.ttf` in the working directory. In project settings → set it to **"Copy if newer"**
6. Build and run (`F5`)

---

Made with ❤️ using C++ and SFML
