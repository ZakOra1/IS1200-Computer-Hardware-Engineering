/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <standard.h> //Using the rand() function


#define colMax 128 //number of display columns
#define rowMax 32 //number of display rows
#define pagedMax 4 //number of display memory pages

int mytime = 0x5957;
int timeoutcount;
/* Default values to get the game going
*/

int snakeY;
int snakeX;
int foodX;
int foodY;
int score;
int foodAlive;
int direction;
int crash;

/* Interrupt Service Routine */
// This function is called when an interrupt is found
void user_isr( void )
{
  
  if(IFS(0) & 0x100){
    switch(direction){
      case 4:
        snakeX--;
        break;
      case 3:
        snakeX++;
        break;
      case 2:
        snakeY--;
        break;
      case 1:
        snakeY++;
        break;
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

  //Defining game-start values
  int snakeY = 16;
  int snakeX = 64;
  int foodX = 45;
  int foodY = 18;
  int score = 0;
  int foodAlive = 0;
  int direction = 0;
  int crash = 0;

  return;
}

//Spawns the map and the snake
void spawn(void){
  if(crash == 0){
    head(snakeY,snakeX);
    map();
  }
}

//Controlls the movement of the snake, moving in four directions
void movement(void){
  int button =  getbtns();
  //Scenarios depending on which button was pressed
	switch(button)
	{
		case 8: //Button 4 (Left)
      direction = 4;
			break;
		case 4: //Button 3 (Right)
      direction = 3;
			break;
		case 2: //Button 2 (Up)
      direction = 2;
			break;
    case 1: //Button 1 (Down)
      direction = 1;
      break;

    default:
      break;
	} 
}

//Looks if it has collided with anything (apple or wall)
void deathCollision(){
  //Check if snake is outside of the screen
  if(snakeX > colMax || snakeY > rowMax || snakeX < 4|| snakeY < 4)
  {
    crash = 1;
  }
}

void randomSeed(void) {
  return TMR2;
}

int foodPosX(void) {
  return rand() % 128;
}

int foodPosY(void) {
  return rand() % 32;
}

void spawnApple(void){
  //Generate apple if it has been eaten
  if(foodAlive == 0)
  {
    foodX = foodPosX();
    foodY = foodPosY();
    apple(foodX, foodY);
    foodAlive = 1;
  }
}

//Check if snake is on food
void appleCollision(){
  if(snakeX == foodX && snakeY == foodY)
  {    
    score++;
    foodAlive = 0;
    spawnApple();
  }
}


//When the snake hits the wall, the end screen pops up.
void end_screen(void)
{
	display_string(0, "   You Died!   ");
	display_string(1, "               ");
	display_string(2, "Press a button ");
  display_string(3, "  to restart   ");
  display_update();

  if(getbtns()){
    crash = 0;
  }
}


/* This function is called repetitively from the main program */
void labwork( void )
{    
  if(crash == 0){
    //clear screen
    int i,j;
	  for(i = 0; i < 32; i++){
      for(j = 0; j < 128; j++){
        pixel[i][j] = 0;
      }
    }
    srand(randomSeed());
    spawn();
    movement();
    deathCollision();
    appleCollision();
  }

  else{
    end_screen();
  }
  

  fullscreen_display_update();
}
