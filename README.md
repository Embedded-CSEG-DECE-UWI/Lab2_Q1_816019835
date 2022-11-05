# Lab2_Q1_816019835
P1
The /project/build/include/sdkconfig.h is a application-specific file which configures the esp01 variant to operare and allow certain functions to run if the paramemeter are set to have a respective outcome. For example if the baud rate for UART communication of the esp01 is configured at a different rate, then PuTTY must reflect that updated speed as well for view of communication over the port. #define CONFIG_ESP_CONSOLE_UART_BAUDRATE 74880 to 9600.
The /project/sdkconfig is also a application-specific file which configures the framework, C code libraries and header files specific to the esp01 OS for successful implementation of the code script.
The /sdk/components/freertos/port/esp8266/include/freertos/FreeRTOSConfig.h is a port specific configuration that configures how the the OS would operate and allow for assembly binding to occur.

P2 
The labels that provide task performance information in the ESP SDk are the following:
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_xTaskGetCurrentTaskHandle 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define TASK_SW_ATTR
#define taskYIELD
#define taskSCHEDULER_SUSPENDED		( ( BaseType_t ) 0 )
#define taskSCHEDULER_NOT_STARTED	( ( BaseType_t ) 1 )
#define taskSCHEDULER_RUNNING
