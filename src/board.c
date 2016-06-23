/*
 * board.c
 *
 *  Created on: Feb 16, 2015
 *      Author: ebold
 */

#include "board.h"
#include "time.h"
#include "display.h"

uint8_t btnHigh, btnLow;

/* SPI interface initialization */
void STM_SPI1_Init(void)
{
	/* Enable SPI, Master, MSB first, SPI mode 1 */
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI1_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIOs clock */
  RCC_APB2PeriphClockCmd(SPI1_MOSI_GPIO_CLK | SPI1_MISO_GPIO_CLK | SPI1_SCK_GPIO_CLK, ENABLE);

  /* Enable SPI1 clock */
  RCC_APB2PeriphClockCmd(LED_EXT_SPI1_CLK, ENABLE);

  /* Configure SPI1 SCK pin */
  GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SPI1 MOSI pin */
  GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_PIN;
  GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SPI1 MISO pin */
  GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI1_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI1 configuration -------------------------------------------------------*/

  //    Configure SPI1 in Mode 1 (ARM-base):
  //        CPOL = 0 --> clock is low when idle
  //        CPHA = 0 --> data is sampled at the first edge

  SPI1_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;

  /* SPI1 baudrate is set to 1 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 64/64 = 1 MHz)
     to verify these constraints:
				- LED external module SPI interface max baudrate is 25MHz for write/read
				- PCLK2 max frequency is 64 MHz
	 */
  SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI1_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(LED_EXT_SPI, &SPI1_InitStructure);

  //    Configure the SPI interrupt priority
  //
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // the lowest priority
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable SPI1 */
  SPI_Cmd(LED_EXT_SPI, ENABLE);
}

void STM_SPI2_Init(void)
{
	/* Enable SPI, Master, MSB first, SPI mode 1 */
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI2_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIOs clock */
  RCC_APB2PeriphClockCmd(SPI2_MOSI_GPIO_CLK | SPI2_MISO_GPIO_CLK | SPI2_SCK_GPIO_CLK, ENABLE);

  /* Enable SPI2 clock */
  RCC_APB1PeriphClockCmd(LED_EXT_SPI2_CLK, ENABLE);

  /* Configure SPI2 SCK pin */
  GPIO_InitStructure.GPIO_Pin = SPI2_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SPI2 MOSI pin */
  GPIO_InitStructure.GPIO_Pin = SPI2_MOSI_PIN;
  GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SPI2 MISO pin */
  GPIO_InitStructure.GPIO_Pin = SPI2_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI2 configuration -------------------------------------------------------*/

  //    Configure SPI2 in Mode 1 (ARM-base):
  //        CPOL = 0 --> clock is low when idle
  //        CPHA = 0 --> data is sampled at the first edge

  SPI2_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;

  /* SPI2 baudrate is set to 1 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 64/64 = 1 MHz)
     to verify these constraints:
				- LED external module SPI interface max baudrate is 25MHz for write/read
				- PCLK2 max frequency is 64 MHz
	 */
  SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI2_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(LED_EXT_SPI, &SPI2_InitStructure);

  //    Configure the SPI interrupt priority
  //
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // the lowest priority
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable SPI2 */
  SPI_Cmd(LED_EXT_SPI, ENABLE);
}

/* Function to send a single byte over SPI */
void sendSPIData(uint8_t data)
{
  //uint8_t tmp = 0x00;

  /* Wait until the transmit buffer is empty */
  while(SPI_I2S_GetFlagStatus(LED_EXT_SPI, SPI_I2S_FLAG_TXE) != SET)
  {
  }

  /* Send the byte */
  SPI_I2S_SendData(LED_EXT_SPI, data);

  /**while(SPI_I2S_GetFlagStatus(LED_EXT_SPI, SPI_I2S_FLAG_RXNE) != SET)
  {
  }*/

  /* Return the byte read from the SPI bus */
  /*tmp = SPI_I2S_ReceiveData(LED_EXT_SPI);*/

  /* Wait until the transmit buffer is empty */
  while(SPI_I2S_GetFlagStatus(LED_EXT_SPI, SPI_I2S_FLAG_TXE) != SET)
  {
  }

  /* wait until the completion of the transfer */
  while(SPI_I2S_GetFlagStatus(LED_EXT_SPI, SPI_I2S_FLAG_BSY) != RESET)
  {
  }

  /* Return read data */
  //return tmp;
}
/* Configures control lines (LDO, OE) for external LED in Output Push-Pull mode */
void initCtrlLEDExt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clock*/
	RCC_APB2PeriphClockCmd(LED_EXT_LDO_GPIO_CLK | LED_EXT_OE_GPIO_CLK, ENABLE);

	/* Configure LDO in Output Push-Pull mode */
	GPIO_InitStructure.GPIO_Pin = LED_EXT_LDO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_EXT_LDO_GPIO_PORT, &GPIO_InitStructure);

	/* Configure OE in Output Push-Pull mode */
	GPIO_InitStructure.GPIO_Pin = LED_EXT_OE_PIN;
	GPIO_Init(LED_EXT_OE_GPIO_PORT, &GPIO_InitStructure);

	/* Set OE pin low */
	LED_EXT_LDO_LOW();
}

void configBoard(void)
{
	cnt[TIMER_1MS] = 0;
	cnt[TIMER_3MS] = CNT_3MS;
	cnt[TIMER_100MS] = CNT_100MS;
	spiTxCnt = 0;

	btnHigh = 0;
	btnLow = 0;
	btnDeb = BTN_RELEASED;

	initCtrlLEDExt();

	STM_SPI1_Init();	//STM_SPI2_Init();

  /* Initialize LEDs mounted on STM32NUCLEO board --------------------------*/
  //STM_EVAL_LEDInit(LED2);

  /* Initialize user button mounted on STM32NUCLEO board */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Configure RTC */
  //RTC_Config();

}

void sendDataLEDExt(uint8_t data)
{
	sendSPIData(data);
}

/**
 * SysTick interrupt handler. The handler is called every 1ms.
 */
void ISR_timer(void)
{

    cnt[TIMER_100MS]--;
    if (!(cnt[TIMER_100MS]))
    {
        cnt[TIMER_100MS] = CNT_100MS;
        if (!(isTimeSetInProgress()))
            event |= EVNT_TIME_ADVANCE; // 1/10 second is over
    }

	if (cnt[TIMER_3MS])
	{
		cnt[TIMER_3MS]--;
		if (!(cnt[TIMER_3MS]))
		{
			cnt[TIMER_3MS] = CNT_3MS;

			if (STM_EVAL_PBGetState(BUTTON_USER))	// button is released
			{
				btnLow = 0;
				btnHigh++;
				if (btnHigh > DELAY_20MS)
				{
					btnHigh = DELAY_20MS + 1;
					btnDeb = BTN_RELEASED;
				}
			}
			else	// button is pressed
			{
				btnHigh = 0;
				btnLow++;
				if (btnLow > DELAY_20MS)
				{
					btnLow = DELAY_20MS + 1;
					btnDeb = BTN_PRESSED;
				}
			}
			event |= EVNT_TIME_ADJUST;
		}
	}

	if (doLedDisplayRefresh) // do LED display refresh if previous refresh is completed
	{
		doLedDisplayRefresh = FALSE; // reset flag
		spiTxCnt = 0; // reset counter for SPI transmitted byte
		pSource = pPattern + ledColumn; // byte-pattern buffer + offset
		LED_OFF; // turn off LEDs
		SPI_I2S_ITConfig(LED_EXT_SPI, SPI_I2S_IT_TXE, ENABLE); // enable SPI interrupt
	}
}

/**
 * SPI interrupt handler.
 */
void ISR_txSPI(void)
{
	if (spiTxCnt < MAX_LED_BLK)	// send pattern bytes
	{
		sendSPIData(*pSource);	// send byte-pattern
		spiTxCnt++;							// count SPI transmission
		pSource += MAX_LED_COL;		// point to next byte-pattern
	}
	else if (spiTxCnt == MAX_LED_BLK)	// send column byte
	{
		sendSPIData(ledColumn);	// activate this column
		spiTxCnt++;								// count SPI transmission

		++ledColumn;					// select next column
		if (ledColumn == MAX_LED_COL)	// if all columns of the text display have been accessed
			ledColumn = 0;			// then start again from the first column
	}
	else	// stop SPI transmission
	{
		PRE_LOAD;		// prepare to load transmitted bytes to external latch
		LOAD_BYTE;	// latch transmitted bytes

		SPI_I2S_ITConfig(LED_EXT_SPI, SPI_I2S_IT_TXE, DISABLE);	// disable SPI interrupt
		SPI_I2S_ClearITPendingBit(LED_EXT_SPI, SPI_I2S_IT_TXE);	// clear the interrupt pending bit otherwise this interrupt will be regenerated

#ifdef ENABLE_DIMMER
		if (dimmer)	// if dimmer is activated
		{
			//restartTimer1();		// then start timer1
			//enableTimer1IRQ();		// and enable timer1 interrupt
			/* LEDs are turned on by timer1 ISR after a fixed time delay */
		}
		else		// otherwise LEDs are turned on immediately
#endif
		{
			LED_ON;				// turn on LEDs
			doLedDisplayRefresh = TRUE;	// set flag
		}
	}
}




