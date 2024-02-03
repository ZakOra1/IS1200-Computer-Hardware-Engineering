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

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  // Volatile, because the compiler might try to optimize the pointer and define the pointer value, which is not good since it can get updated on the way.
  // Casting int just to be certain that we check byte-wise
  // Point to the address TRISE
  volatile int* E = (volatile int*) 0xbf886100;
  // Changes the 8 lsb to 0 (output value)so that they're outputs, while maintaining the values of the other bits 
  *E = *E & 0xffffff00;

  // Using predermined definitions to set bits 5-11 to 1.
  TRISDSET = 0b111111100000;


  // Want to initilize from 0
  TMR2 = 0;

  // Since we need one tenth of a second, the perioed register needs to fit 1/10 of the frequency.
  // We have to prescale the clock by 256, since there's no way to prescale it by 128, and prescaling it to 64 is too little, 1.25 miljon can't fit a 16-bit number.
  PR2 = 31250;
  
  // Bit 15 to enable timer, bit 6-4 to prescale 1:256 (Time-out period)
  T2CON = 0x8070;    // 80Mhz/256 = 312500 Hz frequency (Meaning that the system will read the oscillator after it oscillations 256 times, thereof lowering the frequency by 256, i.e 312 500 oscillations per second.)
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{  
  // T2IF(Timer 2 interrupt flag) is found at bit 8 in IF(0) 
  if(((IFS(0) >> 8) & 0b1) == 1){
    //Reset the flag
    IFSCLR(0) = 0x100;
    timeoutcount++;
    if(timeoutcount == 10)
    {
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    timeoutcount = 0;
    }
  }
  display_image(96, icon);

  // PORTE is a Predetermined address to use the already determined outputs/inputs on the UNO32 board.
  // Since we determined the LSB we will add +1 to increment by one bit, thereof making the light light up in a binary fashion.
  //(0000 0001) -> (0000 0010) -> (0000 0011) .....
  PORTE += 1;
  //Once we increment above 0x000000ff, it will restart the light, but the value is actually 0x00000100

  int getbuttons = getbtns();

  //If any button is pressed
  if(getbuttons != 0){
    int getswitch = getsw();

    //If button 4 is pressed (bitwise and)
    if(getbuttons & 0b100){
      // Mask all bits except bits 15-12 (0000 1111 1111 1111), 
      // so that we can OR with "getswitch" after it's shifted 12 bits left (yyyy 0000 0000 0000).
      mytime = (mytime & 0xffff0fff ) | (getswitch << 12);
    }

    //If button 3 is pressed
    if(getbuttons & 0b10){
      mytime = (mytime & 0xfffff0ff ) | (getswitch << 8);
    }

    //If button 2 is pressed
    if(getbuttons & 0b1){
      mytime = (mytime & 0xffffff0f ) | (getswitch << 4);
    }
  }
}
