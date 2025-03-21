#include "main.h"
#include "LightSerialCom.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

const char LSC_COMMAND_GAP_CHAR[] = " \t\r\n";

//LSC shell输入输出队列
char LSC_rxQueueBuf[LSC_QUEUE_MAX_LENGTH], LSC_txQueueBuf[LSC_QUEUE_MAX_LENGTH];
LSC_queue_t LSC_rxQueue={0,0,0,0,LSC_rxQueueBuf};
LSC_queue_t LSC_txQueue={0,0,0,0,LSC_txQueueBuf};

//LSC应用列表初始化
const LSC_function_t* (LSC_functions[LSC_FUNCTIONS_MOUNT_MAX]);
static uint16_t LSC_functions_num = 0;

static const LSC_function_t* OccupiedFunc;//当前处理应用


//继续下一参数
char* LSC_getNextArg()
{
  return strtok(NULL,LSC_COMMAND_GAP_CHAR);
}

//返回LSC应用数量
uint16_t LSC_functions_getNum()
{
  return LSC_functions_num;
}


//查询LSC应用
const LSC_function_t* LSC_functions_find(char* cmd)
{
  uint16_t i;
  int16_t tmp,l=0,r=LSC_functions_num-1;
  while(l<=r){
    i = (l + r)/2;
    tmp = strcmp(cmd,LSC_functions[i]->command);
    if (tmp == 0){
      return LSC_functions[i];
    }else if (tmp > 0){
      l=i+1;
    }else{
      r=i-1;
    }
  }
  return NULL;
}

//注册LSC应用（无检查）
static int LSC_functions_add(const LSC_function_t* function)
{
  uint16_t i=0,j=0;
  int16_t tmp;
  while(i<LSC_functions_num){
    tmp = strcmp(LSC_functions[i]->command,function->command);
    if (tmp == 0){
      return 1;
    }else if (tmp < 0){
      break;
    }else{
      i++;
    }
  }
  for(j=LSC_functions_num;j>i;j--){
    LSC_functions[j]=LSC_functions[j-1];
  }
  LSC_functions_num++;
  LSC_functions[i] = function;
  return 0;
}

//注册LSC应用
int32_t LSC_functions_load(const LSC_function_t* function)
{
  if (function->command == NULL || function->fullInfo == NULL || function->helpInfo == NULL || function->function == NULL){
    return 1;
  }
  if(LSC_functions_add(function)!=0){
    return 2;
  }
  return 0;
}

//处理命令输入
int32_t LSC_functions_solve(char* cmd)
{
  const LSC_function_t* function;
  char* cmd_head;
  if (OccupiedFunc == NULL){
    cmd_head = strtok(cmd,LSC_COMMAND_GAP_CHAR);
    if (cmd_head == NULL) return 1;
    function = LSC_functions_find(cmd_head);
    if(function == NULL){
      LSC_printf("Command not found, use \"help\" to view commands.");
      LSC_transmit();
      return 1;
    }
    return (*(function->function))(NULL);
  }else{
    return 0;
  }
}

                       

//弱定义输入输出实现
__weak void LSC_printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;
    va_start(ap, fmt);
    len = vprintf(fmt, ap);
    (void)len;
    va_end(ap);
}

__weak void LSC_transmit(void)
{
}






