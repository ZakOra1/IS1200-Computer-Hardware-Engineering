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
#include "getButtons.c"

/*
  - Defines global variables for the OLED-Monitor.
  - It is 128x32, but the horizontal pixels are grouped into arrays of 8 pixels.
  - It is also seperated into 4 pages
*/
#define colMax 128 //number of display columns
#define rowMax 32 //number of display rows
#define pagedMax 4 //number of display memory pages

int timeoutcount;

/*Defining global variables that is used throughout the program
*- Written by Zak Ora & Samuel Sendek*/
int snakeX = 15;
int snakeY = 15;
int foodX = 45;
int foodY = 18;
int score = 0;
int foodAlive = 0;
int direction = 0;

int mytime = 0x5957;

/* Interrupt Service Routine */
// This function is called when an interrupt is found
void user_isr( void )
{
  timeoutcount++;
  if(timeoutcount == 10){
    /*time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    timeoutcount = 0;
*/
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
  TRISDSET = 0b111111100000;
  TMR2 = 0;
  PR2 = 31250;
  T2CON = 0x8070; 
  
  // Initialization of interrupts
  // bit 8 (Enable)
  IEC(0) = 0x100;
  // bit 4:2 (Priority) och 1:0 (Subpriority)
  IPC(2) = 0x1f;

  //*********Suprise assignment*******
  // bit 11 (Enable) Switch 2 
  IECSET(0) = 0x800;

  IPCSET(2) = 0x1f000000;
  //**********************************

  // Call function in labwork.S to enable interrupts globally
  enable_interrupt();
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{  

  //Scenarios depending on which button was pressed
	switch(getbtns())
	{
		case 4: //Button 4 (Left)
      direction = -1;
			break;
		case 3: //Button 3 (Right)
      direction = 1;
			break;
		case 2: //Button 2 (Up)
      direction = 10;
			break;
		case 1: //Button 1 (Down)
      direction = -10;
    default:
      break;
	} 

  //moveSnake(direction);

  //Check if snake is outside of the screen
  if(snakeX > colMax || snakeY > rowMax)
  {
    //end_game();
  }

  //Check if snake is on food
  if(snakeX == foodX && snakeY == foodY)
  {
    //Grow snake by one length
    score++;
    foodAlive = 0;
  }

  //Generate apple if it has been eaten
  if(foodAlive == 0)
  {
    foodX = foodPosX();
    foodY = foodPosY();
    //spawnFood(foodX, fooxY)
    foodAlive = 1;
  }
}

//Generates a random position for the food to 
int randomSeed ()
{
    return TMR2;
}

int foodPosX ()
{
    //Random number between 0 - 128
    return srand(randomSeed()%128);
}

int foodPosY ()
{
    //Random number between 0 - 32
    return srand(randomSeed()%32);
}

