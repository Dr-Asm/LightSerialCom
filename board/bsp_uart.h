#pragma once
#include "stdint.h"
#include "usart.h"
#include "LightSerialCom.h"

#define LSC_UART (&huart3)

static void LSC_UART_RECEIVE(void)
{
  HAL_UART_Receive_IT(LSC_UART,(uint8_t*)(LSC_rxQueue.data+LSC_rxQueue.tail),1);
  LSC_rxQueue.tail = LSC_rxQueue.tail>=LSC_QUEUE_MAX_LENGTH?0:LSC_rxQueue.tail+1;
}

#define __LSC_DEBUG_LOG(text, length) (HAL_UART_Transmit(CLI_UART, (const uint8_t*)(text), (length), 1000))
  


