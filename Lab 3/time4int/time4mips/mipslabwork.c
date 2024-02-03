/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   This file modified 2022-02-14 by Zak Ora

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int counter = 0;
int prime = 1234567;


char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
  //Interupt flag for timer 2
  if (IFS(0)
  {
    IFSCLR(0) 0x100;
  })

  asm volatile("ei");

  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
}


/* Lab-specific initialization goes here */
void labinit( void )
{
  //Initialize volatile int* because 
  //Casting the address so we can check byte-wise 
  volatile int* pE = (volatile int*) 0xbf886100;
  //Changes all values to 1 except for the 8lsb which will be 0, since they will change later.
  *pE = *pE & 0xffffff00;

  TRISDSET = 0b111111100000;

  //This resets the control registers and stops the timer
  T2CON = 0x0;
  //Sets the timer (register) to 0 
  TMR2 = 0x0;
  //Sets T2CON as hexadecimal for 0b1000000001110000, which sets it to the prescale value 1:256
  T2CON = 0x8070;
  //What the timer should count up to (31250) (Period register)
  //7A12 = 0111 1010 0001 0010
  PR2 = 31250;
  //Starts the timer
  T2CONSET = 0x8000

  return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
 prime = nextprime( prime );
 display_string( 0, itoaconv( prime ) );
 display_update();
}
