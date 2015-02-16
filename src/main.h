/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    11-April-2014
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_nucleo.h"
//#include "calendar.h"
#include <stdio.h>
#include <math.h>

/* Exported types ------------------------------------------------------------*/
#define uint8	uint8_t
#define int8	int8_t
#define uint16	uint16_t
#define uint32	uint32_t

/* Exported constants --------------------------------------------------------*/

#define TRUE	1
#define FALSE	0

#define BTN_RELEASED		1
#define BTN_PRESSED			0

/* Exported macro ------------------------------------------------------------*/

extern uint16 event;

#define EVNT_TIME_ADVANCE	0x01
#define EVNT_TIME_DISPLAY	0x02
#define EVNT_TIME_ADJUST	0x04
#define EVNT_BLINK_ON			0x08
#define EVNT_BLINK_OFF		0x10
#define EVNT_USART				0x40
#define EVNT_ENABLED			0x80

/* Exported functions ------------------------------------------------------- */

void ISR_timer(void);
void ISR_txSPI(void);
/* Function to display text */
int8 displayText(uint8 mode, uint8 *text, uint8 offset, uint8 length);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
