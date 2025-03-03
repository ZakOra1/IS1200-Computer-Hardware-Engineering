# Slithery Snake - The Game  

## Authors  
Samuel Sendek, Zak Ora  

## Overview  
This project is a basic implementation of the classic **Snake** game, designed to run on a **chipKIT Uno32** with a **Basic I/O shield**. The objective is to collect apples, grow the snake, and avoid collisions to achieve the highest score.  

## Features  
- **Controlled via chipKIT interface** using push buttons.  
- **Two-dimensional movement** for the snake.  
- **Apple collection** increases snake length and score.  
- **Collision detection** with walls and itself to end the game.  

## Hardware and Implementation  
- **Platform:** chipKIT Uno32 with Basic I/O Shield  
- **Display:** OLED Graphic Display  
- **Programming Language:** C (using MCB32tools)  
- **User Input:** Push buttons for movement  

### Game Logic  
#### 1. **Interface & Display**  
- `fullscreen_display_update()`: Updates the game display every iteration.  
- `display_update()` & `display_string()`: Displays game messages like "You died" and "Welcome".  
- Game objects are stored in a **32x128 uint8_t array** representing OLED pixels.  

#### 2. **Movement**  
- Snake moves by updating its **x and y** coordinates.  
- Movement logic follows a **struct array**, where each block follows the previous one, creating a tail effect.  
- Snake growth is handled by adding new elements to the array.  

#### 3. **Collision Detection**  
- Detects collisions with:  
  - **Walls** → Ends the game.  
  - **Apples** → Increases length, adds points, and spawns a new apple at a random location.  
  - **Itself** → Ends the game.  

## Verification  
- Extensive testing was performed to ensure functionality, including edge cases like **simultaneous button presses**.  
- Additional user testing revealed usability improvements and minor bug fixes.  

## Contributions  
- Zak focused on **game logic** and movement.  
- Samuel worked on **graphics and display integration**.  
- Final debugging and refinements were a **collaborative effort**.  

## Reflections  
This project provided valuable hands-on experience with **C programming**, **chipKIT hardware**, and **memory management**. Despite challenges, it was an engaging and rewarding learning experience.  

---
