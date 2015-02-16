/*
 * display.h
 *
 *  Created on: Feb 1, 2015
 *      Author: ebold
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "main.h"

/* LED display definitions */
#define MAX_LED_BLK	3	// number of LED blocks on the LED display (1 block = 8x10 LEDs)
#define MAX_LED_COL	10	// columns of an LED block
#define MAX_CHAR	5	// number of characters on the LED display

/* Byte pattern definitions */
#define BYTE_PAT_COL	6	// number of columns per 8x6 byte-pattern

/* Display mode definitions */
/* display mode */
#define DISP_MODE_STATIC	0
#define DISP_MODE_BLINK	1
#define MAX_DISP_MODE	2

#define ASCII_DIGIT 0x30

/* Initialize variables for display function */
void initDisplay(void);

/* Refresh display */
void refreshDisplay(void);
/* Start to copy text to the byte-pattern buffer */

/* Function to display text */
int8 displayText(uint8 mode, uint8 *text, uint8 offset, uint8 length);

uint8_t ledColumn; // column index of 8x10 LED block
uint8_t doLedDisplayRefresh; // flag that indicates display refresh completion
uint8_t *pPattern; // pointer to pattern buffer
uint8_t *pSource; // pointer to pattern buffer + offset

#endif /* DISPLAY_H_ */
