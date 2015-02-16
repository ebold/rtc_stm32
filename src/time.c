/*
 * time.c
 *
 *  Created on: Feb 1, 2015
 *      Author: ebold
 */

#include "time.h"
#include "display.h"

uint8 msecond = 0; // 1/10 second
uint8 second = 0; // second (0..59)
uint8 minute = 0; // minute (0..59)
uint8 hour = 0; // hour (0..23)
uint8 dimmer = DIMMER_OFF; // disable dimmer

#define PERIOD_WAIT_LONG			1700	// 3ms * 1700 = 5100 ms
#define PERIOD_WAIT_SHORT			1000	// 3ms * 1000 = 1000 ms
uint32 stateTimeout = 0;					// 3ms-step

// states
#define PRG_STATE_IDLE				0	// idle state
#define PRG_STATE_START				1	// start of setting time
#define PRG_STATE_SET_HOUR		2	// setting hour
#define PRG_STATE_SET_MINUTE	3	// setting minute
#define PRG_STATE_END					4	// end of setting time

uint8 curState = PRG_STATE_IDLE;
uint8 nextState = PRG_STATE_IDLE;
uint8 lastButtonState = BTN_RELEASED;

/* update dimmer */
void updateDimmer(void)
{
	/*if ((TIME_DIMMER_START <= hour) || (hour <= TIME_DIMMER_END))
	{
		dimmer = DIMMER_ON;
	}
	else
	{
		dimmer = DIMMER_OFF;
	}*/
}
/* count datetime */
void countTime(void)
{
	msecond++;
	if (msecond == TIME_SECOND)
	{
		msecond = 0;
		second++;
		if (second == TIME_MINUTE)
		{
			event |= EVNT_TIME_DISPLAY;
			second = 0;
			minute++;
			if (minute == TIME_HOUR)
			{
				event |= EVNT_TIME_DISPLAY;
				minute = 0;
				hour++;
				if (hour == TIME_DAY)
				{
					hour = 0;
				}
				updateDimmer(); // apply dimming
			}
		}
		event |= EVNT_BLINK_ON;
	}
	else if (msecond == TIME_HALF_SECOND)
	{
		event |= EVNT_BLINK_OFF;
	}
}
/* adjust minute manually */
void adjustMinute(void)
{
	second = 0;
	minute++;
	if (minute == TIME_HOUR)
	{
		minute = 0;
	}
	event |= EVNT_TIME_DISPLAY;
}
/* adjust hour manually */
void adjustHour(void)
{
	hour++;
	if (hour == TIME_DAY)
	{
		hour = 0;
	}
	updateDimmer(); // apply dimming
	event |= EVNT_TIME_DISPLAY;
}

/**
 *  set/adjust time
 */
void setTime(uint8 button, uint8 *buf)
{
	uint8 currButtonState = button;

	if (stateTimeout <= PERIOD_WAIT_LONG)
		stateTimeout++;

	switch (curState)
	{
	case PRG_STATE_IDLE:
		if (nextState == PRG_STATE_IDLE)	// trigger next state
		{
			if (currButtonState == BTN_PRESSED)
			{
				nextState = PRG_STATE_START;
				stateTimeout = 0;
			}
		}
		else if (nextState == PRG_STATE_START)
		{
			if (currButtonState == BTN_PRESSED)
			{
				if (stateTimeout > PERIOD_WAIT_LONG)	// transit to PRG_STATE_START
				{
					curState = PRG_STATE_START;
					stateTimeout = 0;
					*(buf + 2) = 's';
					displayText(DISP_MODE_STATIC, buf, 2, 1); // display "XXsYY"
				}
			}
			else
				nextState = PRG_STATE_IDLE;	// reset trigger
		}
		break;

	case PRG_STATE_START:
	{
		if ((currButtonState == BTN_PRESSED) && (currButtonState ^ lastButtonState))	// transit to PRG_STATE_SET_HOUR
		{
			stateTimeout = 0;
			curState = PRG_STATE_SET_HOUR;
			nextState = PRG_STATE_SET_HOUR;
			*(buf + 2) = 'h';
			displayText(DISP_MODE_STATIC, buf, 2, 1); // display "XXhYY"
		}
		else if (stateTimeout > PERIOD_WAIT_SHORT)	// transit to PRG_STATE_END
		{
			curState = PRG_STATE_END;
			nextState = PRG_STATE_END;
			stateTimeout = 0;
		}
	}
	break;

	case PRG_STATE_SET_HOUR:
	{
		if ((currButtonState == BTN_PRESSED) && (currButtonState ^ lastButtonState))
		{
			stateTimeout = 0;
			adjustHour();
		}
		else if (stateTimeout > PERIOD_WAIT_SHORT)	// trigger & transit to PRG_STATE_SET_MINUTE
		{
			curState = PRG_STATE_SET_MINUTE;
			nextState = PRG_STATE_SET_MINUTE;
			stateTimeout = 0;
			*(buf + 2) = 'm';
			displayText(DISP_MODE_STATIC, buf, 2, 1); // display "XXmYY"
		}
	}
	break;

	case PRG_STATE_SET_MINUTE:
	{
		if ((currButtonState == BTN_PRESSED) && (currButtonState ^ lastButtonState))
		{
			stateTimeout = 0;
			adjustMinute();
		}
		if (stateTimeout > PERIOD_WAIT_SHORT)	// trigger & transit to PRG_STATE_END
		{
			curState = PRG_STATE_END;
			nextState = PRG_STATE_END;
			stateTimeout = 0;
			*(buf + 2) = 'e';
			displayText(DISP_MODE_STATIC, buf, 2, 1); // display "XXeYY"
		}
	}
	break;

	case PRG_STATE_END:
	{
		if ((currButtonState == BTN_PRESSED) && (currButtonState ^ lastButtonState))
		{
			nextState = PRG_STATE_START;
			curState = PRG_STATE_START;
			stateTimeout = 0;
			*(buf + 2) = 's';
			displayText(DISP_MODE_STATIC, buf, 2, 1); // display "XXsYY"
		}
		else if (stateTimeout > PERIOD_WAIT_LONG)	// trigger & transit to PRG_STATE_IDLE
		{
			curState = PRG_STATE_IDLE;
			nextState = PRG_STATE_IDLE;
			stateTimeout = 0;
			*(buf + 2) = ':';
			displayText(DISP_MODE_STATIC, buf, 2, 1); // display "XX:YY"
		}
	}
	break;

	default:
		break;

	}

	lastButtonState = currButtonState;

}

/**
 * return state for setting time
 */
uint8 isTimeSetInProgress()
{
	return curState;
}

