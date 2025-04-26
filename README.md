# Project 3 - Projektowanie i analiza algorytmów
>[!NOTE]
>This is not a user-friendly app and does require some programming knowledge to use.

This project was created as part of university coursework for the subject "Design and Analysis of Algorithms". It's a implementation of simple chess bot based on Alpha-Beta Pruning. It has visualizations using OpenGL and ability to test yourself against the bot.

## Installation
>[!WARNING]
>Project was originally built and tested on Windows without intent of usage on other platforms.
1. Clone the repository:
```bash
git clone https://github.com/mikolaj-paz/Projekt-3-projektowanie-i-analiza-algorytmow.git
```
2. Navigate to the project directory:
```bash
cd Projekt-3-projektowanie-i-analiza-algorytmow
```
3. Create a build directory and run CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
>[!NOTE]
>Running right after install let's you play a game against the bot as **white**.

By modyfying the code in **src/game_manager.cpp** you can change the behaviour of the app:
- change the macro to ```BLACK``` if you want to start as **black** (this also affects perspective)
```cpp
  ...
#if PLAYER_COLOR == WHITE  // <-- line 4; change from WHITE to BLACK
  ...
```
- comment one line to watch two instances of bots playing each other
```cpp
  ...
    if (gameState == GAME_ACTIVE && botState == IDLING
        && board->toMove() == BOT_COLOR  // <-- line 155; comment the whole line
    )
  ...
```

## Project Structure
- ```src/``` - Source code (.cpp, .h files)
- ```lib/``` - Additional libraries
- ```shaders/``` - Vertex and fragment shaders
- ```assets/``` - Additional assets (textures, data files, etc.)

## Used Technologies and Libraries
- **OpenGL**
- **GLFW** - Window and context creation
- **GLAD** - For loading OpenGL functions
- **CMake** - For project building
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- project also uses code from [Learn OpenGL](https://learnopengl.com/) by Joey de Vries

## What could be improved
- most of the code requires **refactoring** for readability
- make an app **user-friendly** - you have to change code and rebuild to access functionality
- **Chess Bot** improvements
  - **speed** - for today's standards, bot works pretty slow
  - **depth** - currently tree search doesn't go very deep
  - **end game** - even after taking huge advantages in early and mid-game, bot struggles to secure a win
