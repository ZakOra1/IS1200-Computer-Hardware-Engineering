#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw( void ){
    // Shift PORTD (Read input/write output) 8 steps to make the previous bits 11-8 the 4 lsb, also mask them and make everything else 0
    return (PORTD >> 8) & 0xf;
}

int getbtns( void ){
    // Shift PORTD (Read input/write output) 5 steps to make previous bits 7-5 the 3 lsb, also mask them and make everything else 0
    return (PORTD >> 5) & 0b111;
}