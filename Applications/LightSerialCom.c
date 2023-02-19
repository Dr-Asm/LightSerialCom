#include "main.h"
#include "LightSerialCom.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define LSC_FUNCTIONS_MOUNT_MAX 32
const char LSC_COMMAND_GAP_CHAR[] = " \t\r\n";


const LSC_function_t* (LSC_functions[LSC_FUNCTIONS_MOUNT_MAX]);
static uint16_t LSC_functions_num = 0;
static const LSC_function_t* OccupiedFunc;

uint16_t LSC_functions_getNum()
{
  return LSC_functions_num;
}

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

char* LSC_getNextArg()
{
  return strtok(NULL,LSC_COMMAND_GAP_CHAR);
}
                       

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






