/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    11-April-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "board.h"
#include "display.h"
#include "time.h"

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t showtime[50] = {0};

/* Time Structure definition */
struct time_t
{
  uint8_t sec_l;
  uint8_t sec_h;
  uint8_t min_l;
  uint8_t min_h;
  uint8_t hour_l;
  uint8_t hour_h;
};
struct time_t time_struct;

// event flag
uint16 event;

RCC_ClocksTypeDef RCC_Clocks;

// text buffer
uint8 text[MAX_CHAR] = "Hello";
uint8 *pText = text;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
static void Show_Time(uint32_t TimeVar);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8 i;

	event = 0;

	initDisplay(); // initialize display variables

  displayText(DISP_MODE_STATIC, pText, 0, 5); // display hello message

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_md.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */

  /* configure board */
  configBoard();

  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);	//2000 for 0,5ms

  doLedDisplayRefresh = TRUE;	// enable to display hello msg

  btnDeb = BTN_RELEASED;

  while (BTN_RELEASED == btnDeb);	// wait until BUTTON_USER is pressed

  event = EVNT_ENABLED | EVNT_TIME_DISPLAY | EVNT_BLINK_ON;

  while (1)
  {
  	if (event & EVNT_ENABLED)
  	{
  		/* advance current datetime */
  		if (event & EVNT_TIME_ADVANCE)
  		{
  			countTime();
  			event &= ~EVNT_TIME_ADVANCE;
  		}
  		/* update to display datetime */
  		else if (event & EVNT_TIME_DISPLAY)
  		{
  			i = hour / 10;
  			if (i)
  				*pText = i | ASCII_DIGIT;
  			else
  				*pText = ' ';
  			*(pText + 1) = (hour % 10) | ASCII_DIGIT;
  			i = minute / 10;
  			if (i)
  			{
  				*(pText + 3) = i | ASCII_DIGIT;
  				*(pText + 4) = (minute % 10) | ASCII_DIGIT;
  			}
  			else
  			{
  				*(pText + 3) = (minute % 10) | ASCII_DIGIT;
  				*(pText + 4) = ' ';
  			}
  			displayText(DISP_MODE_STATIC, pText, 0, 5);
  			event &= ~EVNT_TIME_DISPLAY;
  		}
  		/* blink on double points */
  		else if (event & EVNT_BLINK_ON)
  		{
  			*(pText + 2) = ':';
  			displayText(DISP_MODE_STATIC, pText, 2, 1); // display ':'
  			event &= ~EVNT_BLINK_ON;
  		}
  		/* blink off double points */
  		else if (event & EVNT_BLINK_OFF)
  		{
  			*(pText + 2) = '.';
  			displayText(DISP_MODE_STATIC, pText, 2, 1); // display '.'
  			event &= ~EVNT_BLINK_OFF;
  		}
  		/* set/adjust datetime */
  		else if (event & EVNT_TIME_ADJUST)
  		{
  			setTime(btnDeb, pText);
  			event &= ~EVNT_TIME_ADJUST;
  		}

  		/* Show Time */
  		//Show_Time(RTC_GetCounter());

  	}
  }
}


/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Displays the current time.
  * @param  None
  * @retval None
  */
static void Show_Time(uint32_t TimeVar)
{
  /* Time hours */
  time_struct.hour_h=(uint8_t)( TimeVar / 3600)/10;
  time_struct.hour_l=(uint8_t)(((TimeVar)/3600)%10);

  /* Time minutes */
  time_struct.min_h=(uint8_t)(((TimeVar)%3600)/60)/10;
  time_struct.min_l=(uint8_t)(((TimeVar)%3600)/60)%10;

  /* Time seconds */
  time_struct.sec_h=(uint8_t)(((TimeVar)%3600)%60)/10;
  time_struct.sec_l=(uint8_t)(((TimeVar)%3600)%60)%10;

  /* Display Time */
  sprintf((char*)showtime, "      %d%d:%d%d:%d%d       ",time_struct.hour_h,time_struct.hour_l,time_struct.min_h,time_struct.min_l,time_struct.sec_h,time_struct.sec_l);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
