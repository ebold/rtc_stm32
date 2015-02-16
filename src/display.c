/*
 * display.c
 *
 *  Created on: Feb 1, 2015
 *      Author: ebold
 */

#include "display.h"

extern const uint8 charset6x8[96][BYTE_PAT_COL]; // 8x6 byte-pattern for ASCII characters

// display buffer for ASCII text
uint8 bufDisplay[MAX_DISP_MODE][MAX_CHAR];

// pattern buffer for 8x6 byte-patterns of text in display buffer
uint8_t bytePattern[MAX_DISP_MODE][MAX_CHAR * BYTE_PAT_COL];

// offset for pattern buffer
uint8_t next_char;

/* Initialize variables for display function */
void initDisplay(void)
{
	/* initialize display variables */
	pPattern = &bytePattern[0][0];
	ledColumn = 0;
	doLedDisplayRefresh = FALSE;

}
/* Refresh display, driven by HW interrupt */
void refreshDisplay(void)
{
/*
	uint8 i;
	if (doLedDisplayRefresh) // if previous refresh is completed, then initiate next refresh
	{
		doLedDisplayRefresh = 0;
		pSource = pPattern + ledColumn; // byte-pattern buffer + offset
		LED_OFF; // turn off LEDs
		 performs SPI transmission
		for (i = 0; i < MAX_LED_BLK; ++i)
		{
			sendSPIData(~(*pSource)); // send INVERTED byte-pattern over SPI
			pSource += MAX_LED_COL; // select byte-pattern for next LED block
		}
		sendSPIData(ledColumn); // send column selection data (last byte)
		PRE_LOAD; // prepare to load bytes transmitted over SPI
		LOAD_BYTE; // load transmitted bytes
		LED_ON; // turn on LEDs
		++ledColumn;
		if (ledColumn == MAX_LED_COL) // if all columns of the text display have been accessed
			ledColumn = 0; // then start again from the first column
		doLedDisplayRefresh = 1;
	}
*/
}
/* Convert an ASCII character to an 8x6 byte-pattern */
int8 charToPattern(uint8 mode, uint8 ascii_char)
{
	uint8 i, j;
	if ((ascii_char < ' ') || (ascii_char > 0x7F))
	{
		return -1;
	}
	ascii_char -= ' ';
	j = next_char * BYTE_PAT_COL;
	for (i = 0; i < BYTE_PAT_COL; i++)
		bytePattern[mode][i+j] = charset6x8[ascii_char][i];
	return 0;
}
/* Copy text char by char to the byte-pattern buffer */
void writeChar(uint8 mode, uint8 *source)
{
	uint8 i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		charToPattern(mode, *(source + i));
		next_char++;
	}
	pPattern = &bytePattern[mode][0];
}

/* Start to copy text to the byte-pattern buffer */
void generatePattern(uint8 mode)
{
	next_char = 0;
	writeChar(mode, &bufDisplay[mode][0]);
}

/* Function to display text */
int8 displayText(uint8 mode, uint8 *text, uint8 offset, uint8 length)
{
	uint8 i, len;
	if (offset + length > MAX_CHAR)
		return -1;
	if (mode > DISP_MODE_BLINK)
		return -2;
	len = 0;
	for (i = offset; len < length; i++, len++)
		bufDisplay[mode][i] = *(text + i);

	generatePattern(mode);

	return len;
}
