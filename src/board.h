/*
 * board.h
 *
 *  Created on: Feb 16, 2015
 *      Author: ebold
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "main.h"
/**
  * @brief  External LED module control pins, OE = Output Enable, LDO = Load
  *
  * Arduino connector pins D7, D8 (PA.8, PA.9) are mapped to OE, LDO respectively.
  */

#define LED_EXT_OE_PIN						GPIO_Pin_8                  /* PA.8 */
#define LED_EXT_OE_GPIO_PORT			GPIOA                       /* GPIOA */
#define	LED_EXT_OE_GPIO_CLK				RCC_APB2Periph_GPIOA

#define LED_EXT_LDO_PIN						GPIO_Pin_9                  /* PA.9 */
#define LED_EXT_LDO_GPIO_PORT			GPIOA                       /* GPIOA */
#define	LED_EXT_LDO_GPIO_CLK			RCC_APB2Periph_GPIOA

#define LED_EXT_OE_HIGH() 				LED_EXT_OE_GPIO_PORT->BSRR = LED_EXT_OE_PIN
#define LED_EXT_OE_LOW()					LED_EXT_OE_GPIO_PORT->BRR = LED_EXT_OE_PIN

#define LED_EXT_LDO_HIGH()				LED_EXT_LDO_GPIO_PORT->BSRR = LED_EXT_LDO_PIN
#define LED_EXT_LDO_LOW()					LED_EXT_LDO_GPIO_PORT->BRR = LED_EXT_LDO_PIN

#define LED_OFF										LED_EXT_OE_LOW()
#define LED_ON										LED_EXT_OE_HIGH()
#define	PRE_LOAD									LED_EXT_LDO_HIGH()
#define	LOAD_BYTE									LED_EXT_LDO_LOW()


#define LED_EXT_SPI								SPI1


/**
  * @brief  External LED module SPI interface, SPI1
  *
  * Arduino connector pins D13, D11, D12 (PA.5, PA.7, PA.6)are mapped to SCK, MOSI, MISO
  */

#define LED_EXT_SPI1_CLK          RCC_APB2Periph_SPI1

#define SPI1_SCK_PIN              GPIO_Pin_5                  /* PA.5 */
#define SPI1_SCK_GPIO_PORT        GPIOA                       /* GPIOA */
#define SPI1_SCK_GPIO_CLK         RCC_APB2Periph_GPIOA
#define SPI1_SCK_SOURCE           GPIO_PinSource5
#define SPI1_SCK_AF               GPIO_AF_5

#define SPI1_MOSI_PIN             GPIO_Pin_7                  /* PA.7 */
#define SPI1_MOSI_GPIO_PORT       GPIOA                       /* GPIOA */
#define SPI1_MOSI_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SPI1_MOSI_SOURCE          GPIO_PinSource7
#define SPI1_MOSI_AF              GPIO_AF_5

#define SPI1_MISO_PIN             GPIO_Pin_6                  /* PA.6 */
#define SPI1_MISO_GPIO_PORT       GPIOA                       /* GPIOA */
#define SPI1_MISO_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SPI1_MISO_SOURCE          GPIO_PinSource6
#define SPI1_MISO_AF              GPIO_AF_5

/**
  * @brief  External LED module SPI interface, SPI2
  *
  * Arduino connector pins D2, D11, D12 (PA.10, PA.7, PA.6)are mapped to SCK, MOSI, MISO
  */

#define LED_EXT_SPI2_CLK          RCC_APB1Periph_SPI2

#define SPI2_SCK_PIN              GPIO_Pin_10                 /* PA.10 */
#define SPI2_SCK_GPIO_PORT        GPIOA                       /* GPIOA */
#define SPI2_SCK_GPIO_CLK         RCC_APB2Periph_GPIOA
#define SPI2_SCK_SOURCE           GPIO_PinSource10
#define SPI2_SCK_AF               GPIO_AF_5

#define SPI2_MOSI_PIN             GPIO_Pin_7                  /* PA.7 */
#define SPI2_MOSI_GPIO_PORT       GPIOA                       /* GPIOA */
#define SPI2_MOSI_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SPI2_MOSI_SOURCE          GPIO_PinSource7
#define SPI2_MOSI_AF              GPIO_AF_5

#define SPI2_MISO_PIN             GPIO_Pin_6                  /* PA.6 */
#define SPI2_MISO_GPIO_PORT       GPIOA                       /* GPIOA */
#define SPI2_MISO_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SPI2_MISO_SOURCE          GPIO_PinSource6
#define SPI2_MISO_AF              GPIO_AF_5

/**
 * @brief Functions to configure and manage the STM32F1xx resources (SPI2 and timers) used to drive
 *    		external LED module
 */

/* Configure board */
void configBoard(void);
/* Function to send a single byte over SPI */
void sendDataLEDExt(uint8_t data);
/* Turn on/off LED */
void turnOnLEDExt(void);
/* Start the main timer (900Hz) */
void startTimerLEDExt(void);

/* Function to send a single byte over SPI */
void sendSPIData(uint8_t data);

#define CNT_100MS			100	// 100 ms
#define CNT_3MS				3		// 3 ms
#define DELAY_20MS		CNT_3MS * 7  // 21 ms

enum
{
	TIMER_1MS	=	0,	// 1ms timer
	TIMER_3MS,			// 3ms timer
	TIMER_100MS,			// 100ms timer
	N_TIMER_MS
} Timers_ms;

// timer counters
uint32 cnt[N_TIMER_MS];

// counter for transmitted bytes over SPI
uint8 spiTxCnt;

uint8_t btnDeb;


#endif /* BOARD_H_ */
