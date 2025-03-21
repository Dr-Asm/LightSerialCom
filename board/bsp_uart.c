#include "bsp_uart.h"
#include "LightSerialCom.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == LSC_UART)
  {
    LSC_rxQueue.update = 1;
    LSC_rxQueue.length = LSC_rxQueue.length >= LSC_QUEUE_MAX_LENGTH? LSC_QUEUE_MAX_LENGTH:LSC_rxQueue.length;
    LSC_UART_RECEIVE();
  }
}

