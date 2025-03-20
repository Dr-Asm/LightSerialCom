/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
	BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR UART0.
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "bsp_uart.h"

/* Library includes. */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

/* Demo application includes. */
#include "serial.h"
#include "bsp_uart.h"
/*-----------------------------------------------------------*/

/* Misc defines. */
#define serINVALID_QUEUE				( ( QueueHandle_t ) 0 )
#define serNO_BLOCK						( ( TickType_t ) 0 )
#define serTX_BLOCK_TIME				( 40 / portTICK_PERIOD_MS )

/*-----------------------------------------------------------*/

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static QueueHandle_t xCharsForTx;

uint8_t xRxedCharsBuffer[ 50 * sizeof( uint16_t ) ];
StaticQueue_t xRxedCharsStaticQueue;
uint8_t xCharsForTxBuffer[ 51 * sizeof( uint16_t ) ];
StaticQueue_t xCharsForTxStaticQueue;

/*-----------------------------------------------------------*/

/* UART interrupt handler. */
void vUARTInterruptHandler( void );

/*-----------------------------------------------------------*/

/*
 * See the serial2.h header file.
 */
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
xComPortHandle xReturn;
	/* Create the queues used to hold Rx/Tx characters. */
  
	xRxedChars = xQueueCreateStatic( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) , xRxedCharsBuffer,&xRxedCharsStaticQueue);
	xCharsForTx = xQueueCreateStatic( uxQueueLength + 1, ( unsigned portBASE_TYPE ) sizeof( signed char ) ,xCharsForTxBuffer,&xCharsForTxStaticQueue);
	
	/* If the queue/semaphore was created correctly then setup the serial port
	hardware. */
	if( ( xRxedChars != serINVALID_QUEUE ) && ( xCharsForTx != serINVALID_QUEUE ) )
	{
    //bsp_CLIUsartInit();	
    __CLI_DEBUG_LOG("Init\r\n", 6); 
    __CLI_RECEIVE();
	}
	else
	{
    __CLI_DEBUG_LOG("Fail\r\n", 6); 
		xReturn = ( xComPortHandle ) 0;
	}

	/* This demo file only supports a single port but we have to return
	something to comply with the standard demo header file. */
	return xReturn;
}

/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* The port handle is not required as this driver only supports one port. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available, or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
signed char *pxNext;

	/* A couple of parameters that this port does not use. */
	( void ) usStringLength;
	( void ) pxPort;

	/* NOTE: This implementation does not handle the queue being full as no
	block time is used! */

	/* The port handle is not required as this driver only supports UART1. */
	( void ) pxPort;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) pcString;
	while( *pxNext )
	{
		xSerialPutChar( pxPort, *pxNext, serNO_BLOCK );
		pxNext++;
	}
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) == pdPASS )
	{
		xReturn = pdPASS;
    uint8_t cChar;
    if(xQueueReceive(xCharsForTx, &cChar, 0) == pdTRUE) {
      if((HAL_UART_GetState(CLI_UART) & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX) {
        HAL_UART_Transmit(CLI_UART, &cChar, 1, 1000); 
      }
    }
	}
	else
	{
		xReturn = pdFAIL;
	}

	return xReturn;
}

/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
	/* Not supported as not required by the demo application. */
}
/*-----------------------------------------------------------*/

void vUARTInterruptHandler( void )
{
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
  for(uint16_t i = 0; i < CLI_UsartRxBuffer.len; ++i) {
    xQueueSendFromISR(xRxedChars, &CLI_UsartRxBuffer.data[i], &pxHigherPriorityTaskWoken);
  }  
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
  HAL_GPIO_TogglePin(LED_DISP_GPIO_Port,LED_DISP_Pin);
}






