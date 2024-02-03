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


int mytime = 0x5957;
int timeoutcount;
/* Default values to get the game going
*/
int gameStart = 0;
int x = 20;
int y = 20;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
// This function is called when an interrupt is found
void user_isr( void )
{
  if(IFS(0) & 0x100){
    timeoutcount++;
    if(timeoutcount == 10){
      time2string( textstring, mytime );
      tick( &mytime );
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
  IPC(2) = 0x1f;    

  // Call function in labwork.S to enable interrupts globally
  enable_interrupt();




  return;
}

void play(void){
  head(y,x);

  int button = 0;
  if(getbtns() != 0)
  {
    button = getbtns();
  }
  else {
    button = getbtn1();
  }

  //Scenarios depending on which button was pressed
	switch(button)
	{
		case 4: //Button 4 (Left)
      x--;
			break;
		case 2: //Button 3 (Right)
      if(getbtns() == 0)
      {
        y++; // down
      }
      else {
        x++; // right
      }
			break;
		case 1: //Button 2 (Up)
      y--;
			break;
    default:
      break;
	} 

  fullscreen_display_update();
}

void startup(void){
	display_string(0, "  Welcome to   ");
	display_string(1, "Slithery Snake ");
	display_string(2, "Prepare to play");
  display_string(3, "               ");
  display_update();

  if(getbtns()){
    gameStart = 1;
  }
}

/* This function is called repetitively from the main program */
void labwork( void )
{  
  //clear screen
  int i,j;
	for(i = 0; i < 32; i++)
		for(j = 0; j < 128; j++)
			pixel[i][j] = 0;

  switch (gameStart)
  {
    case 0:
      startup();
      break;
    case 1:
      play();
      break;
    default:
      break;
  }
}
