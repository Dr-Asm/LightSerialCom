#include "LSC_functions.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

extern uint8_t UserTxBufferFS[];

static uint16_t LSC_TxBufLen;

void LSC_printf(const char *fmt,...)
{
  static va_list ap;
  va_start(ap, fmt);
  LSC_TxBufLen += vsprintf((char *)(UserTxBufferFS + LSC_TxBufLen), fmt, ap);
  va_end(ap);
}
void LSC_transmit(void)
{
  if (LSC_TxBufLen == 0 )return;
  CDC_Transmit_FS(UserTxBufferFS, LSC_TxBufLen);
  LSC_TxBufLen=0;
}

static char help_function(char* cmd){
  extern LSC_function_t* (LSC_functions[]);
  if (cmd == NULL){
    cmd = LSC_getNextArg();
  }
  if (cmd==NULL){
    int i;
    LSC_printf("\r\n***------commands-help-list-----***\r\n");
    for(i=0;i<LSC_functions_getNum();i++){
      LSC_printf("%s\t--%s\r\n",LSC_functions[i]->command,LSC_functions[i]->helpInfo);
    }
    LSC_printf("***--------help-list-end--------***\r\n");
    LSC_transmit();
    return 0;
  }else{
    const LSC_function_t* tmp;
    tmp = LSC_functions_find(cmd);
    if(tmp == NULL){
      LSC_printf("HELP:Command not found.Try \"help\" to view all commands.\r\n");
    }else{
      LSC_printf("HELP:%s\r\n%s\r\n",tmp->helpInfo,tmp->fullInfo);
    }
    LSC_transmit();
  }
  return 0;
}

static const LSC_function_t help={
  .command = "help",
  .helpInfo = "Get help infomation",
  .fullInfo = "--Usage: help [command]\r\n",
  .function = &help_function
};

void LSC_Init()
{
  LSC_functions_load(&help);
}







