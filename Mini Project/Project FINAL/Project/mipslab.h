/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
void fullscreen_display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
char * itoaconv( int num );
void labwork(void);
void quicksleep(int cyc);

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];
/* Declare pixel for display output of all pixels individually */
extern uint8_t pixel[32][128];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */

/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
void enable_interrupt(void);

/*Sprites */
void head(int y, int x);
void apple(int y, int x);
void map(void);
void bodyRight(int y, int x);
void bodyLeft(int y, int x);
void bodyDown(int y, int x);
void bodyUp(int y, int x);

/*Global variables*/
int inGame;
int gameOver;
int score;

/*Global functions*/
void dead(void);
void update_snake(int increment_y, int increment_x, int bt);
void add_body(void);


