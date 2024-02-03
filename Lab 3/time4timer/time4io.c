#include <stdint.h>
#include <pic32x.h>
#include "mipslab.h"

int getsw( void ) {
    return ((PORTD>>8) & 0xf;
}

int getbtns( void ) {
    return ((PORTD>>5) & 0b111);
}