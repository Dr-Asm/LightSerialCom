#ifndef __LIGHTSERIALCOM_H
#define __LIGHTSERIALCOM_H
#include "LSC_functions.h"
#include "stdint.h"

//指令分隔符
extern const char LSC_COMMAND_GAP_CHAR[];

//指令信息结构体
typedef struct{
  char* command;
  char* helpInfo;
  char* fullInfo;
  char (*function)(char*);
}LSC_function_t;

//extern LSC_function_t* (LSC_functions[]);

//指令数量
uint16_t LSC_functions_getNum(void);

//查询指令对应的操作信息
const LSC_function_t* LSC_functions_find(char* cmd);

//添加LSC指令
int32_t LSC_functions_load(const LSC_function_t* function);

//LSC命令解算
//Call example(usbd_cdc):
//static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
//{
//  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
//  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
//  if (*Len<APP_RX_DATA_SIZE){      
//    Buf[*Len] = 0;
//    LSC_functions_solve(Buf);
//  }
//  return (USBD_OK);
//}
int32_t LSC_functions_solve(char* cmd);

//获取操作数
char* LSC_getNextArg(void);

//打印信息
void LSC_printf(const char *fmt,...);
void LSC_transmit(void);



#endif
