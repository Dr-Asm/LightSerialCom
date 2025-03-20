#pragma once
#include "stdint.h"
#include "usart.h"

#define CLI_UART (&huart3)
#define __CLI_RECEIVE() (HAL_UART_Receive_IT(CLI_UART,(CLI_UsartRxBuffer.data),1))
#define __CLI_DEBUG_LOG(text, length) (HAL_UART_Transmit(CLI_UART, (const uint8_t*)(text), (length), 1000))


#define bsp_CLIUsartInit MX_USART3_UART_Init

#define CLI_USART_RX_BUFFER_SIZE 16

typedef struct{
  uint16_t len;
  uint8_t finish_flag;
  uint8_t unuse;
  uint8_t *data;
}usartBuffer_t;

extern usartBuffer_t CLI_UsartRxBuffer;


