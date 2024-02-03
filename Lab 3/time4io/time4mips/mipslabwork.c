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

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return; 
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

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  display_image(96, icon);
  //Using the pic32mx library for PORT E to add 1 
  PORTE++; 
  //Calling buttons
  int getbtns = getbtns();

  if(getbtns)
  {
    int getsw = getsw();

    //Switch-statement with getsw
    switch (getbtns)
    {
    case getbtns & 4: //if button 4 is pressed
      mytime = (mytime & 0x0ffffffff) | (getsw << 12));
      break;
    case getbtns & 3: //if button 3 is pressed
      mytime = (mytime & 0xfffff0ff) | (getsw << 8));
      break;
    case getbtns & 2: //if button 2 is pressed
      mytime = (mytime & 0xffffff0f) | (getsw << 4));
      break;
    default:
      break;
    }

    }
  }
}
