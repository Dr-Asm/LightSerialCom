/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       usb_task.c/h
  * @brief      usb outputs the error message.usb ‰≥ˆ¥ÌŒÛ–≈œ¢
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */
#include "usb_task.h"


#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "string.h"
#include "cmsis_armcc.h"
#include "LightSerialCom.h"

#define LIMIT 128

extern uint8_t UserTxBufferFS[];

void usb_init(void)
{
  LSC_Init();
}
int usb_command_solve(uint8_t* rxBuf, uint32_t rxLen)
{
  return 0;
}
