#include "bsp_uart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "FreeRTOS_CLI.h"
#include "serial.h"

static uint8_t CLIUsartRxArray[CLI_USART_RX_BUFFER_SIZE];
usartBuffer_t CLI_UsartRxBuffer={0,0,0,(uint8_t*)&CLIUsartRxArray};
void vUARTInterruptHandler( void );

//#define configUSE_TRACE_FACILITY 1
//#ifndef configUSE_STATS_FORMATTING_FUNCTIONS
//#define configUSE_STATS_FORMATTING_FUNCTIONS 1
//#endif
//#define configSUPPORT_DYNAMIC_ALLOCATION 1

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART3)//?????
  {
    CLI_UsartRxBuffer.finish_flag = 1;
    //CLI_UsartRxBuffer.len = CLI_USART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart3.hdmarx);//hdma_usart1_rx.Instance->NDTR;
    CLI_UsartRxBuffer.len = 1;
    vUARTInterruptHandler();
    __CLI_RECEIVE();
  }
}
