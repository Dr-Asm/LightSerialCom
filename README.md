# LightSerialCom
* 本项目提供一种轻量化的命令行交互方案，可供片上系统使用。为了适配ST的内置usb串口驱动以及其他芯片中的类似驱动，项目中命令行的输入输出均使用带有缓冲区的方案，故无法实现类似树莓派的串口控制台的同步显示，需要一次传输整串命令才能触发。
* 样例使用STM32F103C8T6单片机，使用HAL库编程。命令行使用USB虚拟串口收发，可根据需要修改为物理串口收发。

## 使用方法
命令行的交互格式为 指令+参数*n 的形式，使用空格或换行符进行分隔。其中不同的指令对应各自独立的方法，而参数由该方法内部进行处理。
创建一个新的方法需要在`LSC_functions.c`文件中新建一个结构体。结构体的格式在`LightSerialCom.h`中描述如下：
```
typedef struct{
  char* command;
  char* helpInfo;
  char* fullInfo;
  char (*function)(char*);
}LSC_function_t;
```
声明结构体可以参考自带的help方法：
```
static const LSC_function_t help={
  .command = "help",
  .helpInfo = "Get help infomation",
  .fullInfo = "--Usage: help [command]\r\n",
  .function = &help_function
};
```
创建新方法后，需要注册该方法的初始化，在`LSC_Init()`函数中添加一行`LSC_functions_load(&方法名)`
* 其中，command为该方法对应的指令字符串，在命令行中输入这一字符串就可以触发这一方法。
* helpInfo为该方法的简略描述，fullInfo为详细的使用方法描述。这两个参数是可选的，如果设置了，则可以通过内置的help命令查看其使用说明。
* function为该方法调用的函数，同样需要自行编写。函数结构要求传入参数为字符指针，返回值为字符。

编写函数时需要注意，传入函数的字符指针并非命令行中指令后附带的参数，而是标记函数由谁调用。在命令行调用这一函数时，该字符指针为NULL。
函数中若要获取传入参数，可以使用`LSC_getNextArg()`方法。这一方法是使用标准库的strtok实现的，每次调用会获取下一个附带的参数，若返回NULL则表示参数已经全部读完。
具体的函数编写可以参考`LSC_functions.c`中的`static char help_function(char* cmd)`函数。