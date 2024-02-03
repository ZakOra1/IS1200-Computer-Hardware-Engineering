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

int prime = 1234567;
int mytime = 0x5957;
int timeoutcount;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
// This function is called when an interrupt is found
void user_isr( void )
{
  if(IFS(0) & 0x100){
    timeoutcount++;
    if(timeoutcount == 10){
      time2string( textstring, mytime );
      display_string( 3, textstring );
      display_update();
      tick( &mytime );
      timeoutcount = 0;
    }

    // bit 8 (Flag) (Acknowledges the interrupt)
    // Therefor we clear the flag
    IFSCLR(0) = 0x100;
  }

  //*********Suprise assignment*******
  if(IFS(0) & 0x800){
    tick( &mytime );
    tick( &mytime );
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );

  

    // bit 11 (Flag) Switch 2 
    IFSCLR(0) = 0x800;
  }
  //**********************************
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
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update(); 
}
