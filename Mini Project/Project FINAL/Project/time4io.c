#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getbtns( void ){
    // Shift PORTD (Read input/write output) 5 steps to make previous bits 7-5 the 3 lsb, also mask them and make everything else 0
    // Button 1 is located at PORTF
    return (( (PORTD >> 4) & 0b1110 ) | ( (PORTF >> 1) & 0b1) );
}