
#ifndef USB_TASK_H
#define USB_TASK_H
#include "main.h"


extern int usb_command_solve(uint8_t* Buf, uint32_t Len);
void usb_init(void);
#endif
