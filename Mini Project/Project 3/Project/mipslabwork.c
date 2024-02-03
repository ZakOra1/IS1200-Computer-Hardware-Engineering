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


#define colMax 128 //number of display columns
#define rowMax 32 //number of display rows
#define pagedMax 4 //number of display memory pages

int mytime = 0x5957;
int timeoutcount;
/* Default varibles to get the game going, we need to initilize
*  x and y preemptively to not get killed in the game */
  int snakeY;
  int snakeX;
  int foodX;
  int foodY;
  int score;
  int direction;

/* Interrupt Service Routine */
// This function is called when an interrupt is found
void user_isr( void )
{
  /* Use the Timer to move the snake */
  if(IFS(0) & 0x100){
    timeoutcount++;
    if(timeoutcount == 4){
      switch(direction){
        case 4:
          snakeX-= 4;
          break;
        case 3:
          snakeX += 4;
          break;
        case 2:
          snakeY -= 4;
          break;
        case 1:
          snakeY += 4;
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


  snakeY = 16;
  snakeX = 64;
  foodX = 45;
  foodY = 18;
  score = 0;
  direction = 0;

  return;
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

/* Looks if the snake has collided with the wall, and if so it jumps 
*  out of the main loop and sends us to the "game over" screen. */
void end_screen(){
  //Check if snake is outside of the screen
  if(snakeX > colMax || snakeY > rowMax || snakeX < 4|| snakeY < 4)
  {
    inGame = 0;
  }
}

void appleCollision(){
  //Check if snake is on food
  if(snakeX == foodX && snakeY == foodY)
  {
    //Grow snake by one length
    score++;
    foodX = randomNumber(127);
    foodY = randomNumber(31);
  }
}

int randomNumber(int interval)
{
  int flag = 1
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

//Spawns the map and the snake
void spawn(void){
  head(snakeY,snakeX);
  apple(foodY, foodX);
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
