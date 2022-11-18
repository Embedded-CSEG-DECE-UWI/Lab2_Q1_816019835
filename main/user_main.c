/* gpio example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "main";

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO2: output
 *
 * Test:
 * Connect GPIO0 with GPIO2
 * Generate pulses on GPIO2, that triggers interrupt on GPIO0
 *
 */

#define GPIO_OUTPUT_IO_0     2
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_0) 

//Global variables
//static xQueueHandle gpio_evt_queue = NULL;
static SemaphoreHandle_t xSemaphore = NULL;

static TimerHandle_t one_shot_Timer = NULL;

static TaskHandle_t Handle1 = NULL;

static TaskHandle_t Handle2 = NULL;

static TaskHandle_t Handle3 = NULL;

static uint32_t timerCount = 0;

void vTimerCallback( TimerHandle_t xTimer )
{
    timerCount = 0;
    // The timer expired, therefore 0.5 seconds must have passed since the GPIO pin has been turned on. Do a task task Delay for 1 second. 
    printf("Actively waited for 0.5s...\n");
    timerCount ++;
   // vTaskDelay(1000 / portTICK_PERIOD_MS);
   // printf("Delayed for 1s...\n");
    
 }

static void gpio_task_1_sharingPin( void *arg )
{
    for (;;)
    {
    if ( xSemaphore != NULL )
    {
       /* See if we can obtain the semaphore.  If the semaphore is not available wait 10 ticks to see if it becomes free. */
       if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
       /* Accessing shared resources , GPIO pin 2 Turning on the LED connected to GPIO pin 2 */
        	printf("Timer for 0.5 s\n");
		gpio_set_level(GPIO_OUTPUT_IO_0, 1);
		printf("Level set high\n");
        	xTimerReset(one_shot_Timer, 10 );
                printf("Checking xTimerStart function 1\n");

        //Start timer and wait for 10 ticks if command queue is full;
       		if( xTimerReset( one_shot_Timer, 10 ) != pdPASS )
                 {
                printf("The timer could not be set into the Active state. high\n ");
                 }
                else
       	          {
	         printf("Timer was activated high\n");
                   }
		 //timerCount = ( uint32_t ) pvTimerGetTimerID( one_shot_Timer );
		 printf("%d\n",timerCount);
		 while ( timerCount != 1 )
		 {

		 }


           /*Finished accessing shared resource. Release the semaphore. */
           xSemaphoreGive( xSemaphore );
   	   printf("Giving semaphore\n");
	   vTaskDelay(1000 / portTICK_PERIOD_MS);
	   printf("Delayed for 1s after task 1...\n");

    }
    else
    {
	    /*Semaphore was not obtain and therefore connot access the shared resource safely. */
	    printf("Waiting for the semaphore to release resource from low\n");
    }
}
}
}

static void gpio_task_2_sharingPin( void *arg )
{
    for (;;)
    {
    if ( xSemaphore != NULL )
    {
       /* See if we can obtain the semaphore.  If the semaphore is not available wait 10 ticks to see if it becomes free. */
       if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
       /* Accessing shared resources , GPIO pin 2 Turning on the LED connected to GPIO pin 2 */
                printf("Timer for 0.5 s\n");
                gpio_set_level(GPIO_OUTPUT_IO_0, 0);
                printf("Level set low\n");
                xTimerReset(one_shot_Timer, 10 );
                printf("Checking xTimerStart function 2\n");
        //Start timer and wait for 10 ticks if command queue is full;
       		if( xTimerReset( one_shot_Timer, 10 ) != pdPASS )
                  {
                printf("The timer could not be set into the Active state. low\n");
                  }
       else
          {
               printf("Timer was activated low\n");
       }
           /*Finished accessing shared resource. Release the semaphore. */
           xSemaphoreGive( xSemaphore );
           printf("Giving semaphore low\n");
           vTaskDelay(1000 / portTICK_PERIOD_MS);
           printf("Delayed for 1s after task 2...\n");

    }
    else
    {
            /*Semaphore was not obtain and therefore connot access the shared resource safely. */
            printf("Waiting for the semaphore to release resource from high\n");
    }
}
}
}

/*static void gpio_task_3_message(void *arg)
{
    uint32_t io_num3;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num3, portMAX_DELAY)) {
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", io_num3, gpio_get_level(io_num3));
        }
    }
}*/

void app_main(void)
{
    one_shot_Timer = xTimerCreate
    (//Just a test name, not used by the RTOS kernel.
     "one-shot Timer",
     //The timer period in ms, must be greater than 0.
     (500 / portTICK_PERIOD_MS ),
     //The timer will not auto-reload itsef when it expires.
     pdFALSE,
     //The ID is used to store a count of the number of the times the timer has expired,
     (void*) 0,
     //Timer calls the callback function when it expires.
     vTimerCallback
    );
    if( one_shot_Timer == NULL )
    {
	    printf("The timer was not created. \n");
    }
    else
    {
	    if( xTimerStart( one_shot_Timer, 10 ) != pdPASS )
	    {
		    printf("checking if timer started\n");
	    }
	    //Waiting for 1 second to try starting the time
	    printf("Starting Timer\n");
    }

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to e.g GPIO 2
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config( &io_conf ) ;

    

    //Creating mutex
    xSemaphore = xSemaphoreCreateMutex();
    //start task1
    Handle1 = xTaskCreate(gpio_task_1_sharingPin, "gpio_task_1_sharingPin", 2048, NULL, 10, NULL);
    //start task2
    Handle2 = xTaskCreate(gpio_task_2_sharingPin, "gpio_task_2_sharingPin", 2048, NULL, 10, NULL);
    //start task3
   // Handle3 = xTaskCreate(gpio_task_3_message, "gpio_task_3_message", 2048, NULL, 10, NULL);
   /* Starting the RTOS scheduler will start the timer running as it has already been set into the active state. */
    int cnt = 0;
    while (1)
    {
	    ESP_LOGI(TAG, "cnt: %d\n", cnt++);
	    printf("Checker\n");
	    vTaskDelay( 1000/ portTICK_PERIOD_MS);

    }

    //vTaskStartScheduler();
}
