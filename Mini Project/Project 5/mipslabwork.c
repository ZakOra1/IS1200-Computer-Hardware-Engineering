/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdio.h>
#include <stdlib.h>   /* For NULL */
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define SNAKE_X_START 64
#define SNAKE_Y_START 16
#define SIZE_OF_BODY 4

#define SNAKE_MAX_SIZE 217

#define LEFT 4
#define RIGHT 3
#define UP 2
#define DOWN 1

#define colMax 125 //number of display columns (3 less to compensate for the snake being 4 pixels large)
#define rowMax 29 //number of display rows
#define pagedMax 4 //number of display memory pages

int mytime = 0x5957;
int timeoutcount;

/* Default varibles to get the game going, we need to initilize the game*/
int foodX;
int foodY;
int score;
int foodAlive;
int direction;
int startupDelay;

// Defines type snake_body
typedef struct snake_body{
  int x;
  int y;
  //char* form;
}snake_body;

struct snake_body snake[SNAKE_MAX_SIZE];
int size_of_snake;

/* Interrupt Service Routine */
// This function is called when an interrupt is found
void user_isr( void )
{
  /* Use the Timer to move the snake */
  if(IFS(0) & 0x100){
    timeoutcount++;
    if(timeoutcount == 4){
      switch(direction){
        case LEFT:
          update_snake(0, -SIZE_OF_BODY);
          break;
        case RIGHT:
          update_snake(0, SIZE_OF_BODY);
          break;
        case UP:
          update_snake(-SIZE_OF_BODY, 0);
          break;
        case DOWN:
          update_snake(SIZE_OF_BODY, 0);
          break;
      }
      timeoutcount = 0;
    }

    // bit 8 (Flag) (Acknowledges the interrupt)
    // Therefor we clear the flag
    IFSCLR(0) = 0x100;
  }
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* E = (volatile int*) 0xbf886100;
  *E = *E & 0xffffff00;
  //Clock
  // and btn 2-4 (D 0xe0)
  TRISDSET = 0b111111100000;
  TMR2 = 0;
  PR2 = 31250;
  T2CON = 0x8070; 

  //btn 1
  TRISFSET = 0b10;
  
  // Initialization of interrupts
  // bit 8 (Enable)
  IEC(0) = 0x100;
  // bit 4:2 (Priority) och 1:0 (Subpriority)
  IPC(2) = 0x10;    

  // Call function in labwork.S to enable interrupts globally
  enable_interrupt();

  // Makes room in the heap for a "snake_body"
  snake[0].x = SNAKE_X_START;
  snake[0].y = SNAKE_Y_START; // Creates snake, pointing att an empty body
  snake[1].x = SNAKE_X_START-4;
  snake[1].y = SNAKE_Y_START; // Creates snake, pointing att an empty body
  snake[2].x = SNAKE_X_START-8;
  snake[2].y = SNAKE_Y_START; // Creates snake, pointing att an empty body
  size_of_snake = 3;

  foodX = 88;
  foodY = 24;
  score = 0;
  foodAlive = 0;
  direction = 0;
  startupDelay = 0;

  return;
}

// Controlls the movement of the snake, moving in four directions
void movement(void){
  int button =  getbtns();    // Recives which button was pressed
  startupDelay++;             // Delay timer to not start the game instantly

  if(startupDelay > 50)
  {
    // Scenarios depending on which button was pressed
    switch(button)
    {
      case 8: //Button 4 (Left)
        if(direction != RIGHT)
          direction = LEFT;
        break;
      case 4: //Button 3 (Right)
        if(direction != LEFT)
          direction = RIGHT;
        break;
      case 2: //Button 2 (Up)
        if(direction != DOWN)
          direction = UP;
        break;
      case 1: //Button 1 (Down)
        if(direction != UP)
          direction = DOWN;
        break;

      default:
        break;
    } 
  }
}

/* Looks if the snake has collided with the wall, and if so it jumps 
*  out of the main loop and sends us to the "game over" screen. */
void end_screen(){
  //Check if snake is outside of the screen
  if(snake[0].x > colMax || snake[0].y > rowMax || snake[0].x < 4|| snake[0].y < 4)
  {
    inGame = 0;
  }
}

/* Collects the apple and removes it from the playing field spawning a new one */
void appleCollision(){
  //Check if snake is on food
  if(snake[0].y == foodY && snake[0].x == foodX)
  {
    //Grow snake by one length
    foodX = randomNumber(121) + 4;
    foodY = randomNumber(25) + 4;
    add_body();
  }
}

/* Generats a sudo-randum number withing the interval 0-28 for Y and 0-124 for X. 
*  They shall also be divisible by four */
int randomNumber(int interval)
{
  int flag  = 1;
  int rand;
  while(flag)
  {
    rand = TMR2%interval;
    if(rand%4 == 0)
    {
      flag = 0;
    }
  }
  return rand; 
}

// Updates the position of every snake_body in snake array, also moves the head.
void update_snake(int increment_y, int increment_x){
  int i;
  for(i = size_of_snake; i > 0; i--){       // Moves the snake_bodies to their parents position
    snake[i].x = snake[i-1].x;
    snake[i].y = snake[i-1].y;
  }

  snake[0].x += increment_x;                // Moves the snake_head
  snake[0].y += increment_y;
}

// Adds another snake_body (used when an apple )
void add_body(void){
  snake[size_of_snake].x = snake[size_of_snake-1].x;     
  snake[size_of_snake].y = snake[size_of_snake-1].y;    

  size_of_snake++;        // Adds 1 to the number of snake_bodies
  
}

// Sets the snake_bodies as pixels in the pixel arry defined in mipslabdata.c
void spawn_body(void){
  int i;
  for(i = 0; i < size_of_snake; i++){
    head(snake[i].y,snake[i].x);
  }  
}

//Spawns in the key essentials of the game, such as the map, the snake and the apples
void spawn(void){
  spawn_body();
  apple(foodY,foodX);
  map();
}

/* This function is called repetitively from the main program */
void labwork( void )
{    
  //clear screen
  int i,j;
	for(i = 0; i < 32; i++){
    for(j = 0; j < 128; j++){
      pixel[i][j] = 0;
    }
  }

  spawn();
  movement();
  end_screen();
  appleCollision();
  fullscreen_display_update();
}
