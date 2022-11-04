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
SemaphoreHandle_t xSemaphore = NULL;
//An array to hold handles to the created timers.
TimerHandle_t one_shot_Timer = NULL;

void vTimerCallback( TimerHandle_t xTimer )
 {
    /* The timer expired, therefore 0.5 seconds must have passed since the GPIO pin has been turned on. Do a task task Delay for 1 second. */
    printf("Actively waited for 0.5s...\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("Delayed for 1s...\n");
    
 }

static void gpio_task_1_sharingPin( void * pvParameters )
{
    /* Create then start some timer.  Starting the timer before the RTOS scheduler has been started means the timers will start running immediately that the RTOS scheduler starts.*/

    one_shot_Timer = xTimerCreate
    ( /* Just a text name, not used by the RTOS kernel. */
        "one-shot Timer",
     /* The timer period in ms, must be greater than 0. */
        pdMS_TO_TICKS( 500 ),
     /* The timer will not auto-reload itself when it expires. */
        pdFALSE,
     /* The ID is used to store a count of the number of times the timer has expired, which is initialised to 0. */
        ( void * ) 0,
     /* Timer calls the callback when it expires. */
        vTimerCallback
    );
    if( one_shot_Timer == NULL )
    {
        printf(" The timer was not created.\n");
    }
    else
    {
        //Waiting for 1 second to try starting timer
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Starting Timer...\n");
    }

    if ( xSemaphore != NULL ){
       /* See if we can obtain the semaphore.  If the semaphore is not available wait 10 ticks to see if it becomes free. */
       if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
       /* Accessing shared resources , GPIO pin 2 Turning on the LED connected to GPIO pin 2 */
	    gpio_config_t io_conf;
        print("Timer for 0.5 s\n");
       	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
       	gpio_config(&io_conf);
        xTimerStart(one_shot_Timer, 20 );
        //Start timer and wait for 20 ticks if command queue is full;
        if( xTimerReset( one_shot_Timer, 20 ) != pdPASS )
            {
                printf("The timer could not be set into the Active state. ");
             }
       }
       	
    }

    /*Finished accessing shared resource. Release the semaphore. */ 
    xSemaphoreGive( xSemaphore );
    else{
        /*Semaphore was not obtain and therefore cannot access the shared resoucre safely. */
        printf("Waiting for the semaphore to release resource");
    }
}

/*static void gpio_task_2_sharingPin( void *pvParameters )
{
    if (xSemaphore != NULL ){
       /*See if we can obtain the semaphore. If the semaphore is not 
	* available wait 10 ticks to see if it becomes free. 
       if ( xSemaphore( xSemaphore, ( TickType_t _ 10 ) == pdTrue ) ){
          /*Accessing shared resources , GPIO pin 2
	     * Turning off the LED connected to GPIO pin 2 
          //io_conf.pin_bit mask = 
        }
    #define GPIO_INPUT_IO_0   0
    #define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0) 
    gpio_config_t io_conf;

    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    gpio_config(&io_conf);
}
}*/

/*static void gpio_task_3_sharingPin(void *arg)
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
    
    //Creating mutex
    xSemaphore = xSemaphoreCreateMutex();
    //start task1
    xTaskCreate(gpio_task_1_sharingPin, "gpio_task_1_sharingPin", 2048, NULL, 10, NULL);
    //start task2
   // xTaskCreate(gpio_task_2_sharingPin, "gpio_task_2_sharingPin", 2048, NULL, 10, NULL);
    //start task3
   // xTaskCreate(gpio_task_3_sharingPin, "gpio_task_3_sharingPin", 2048, NULL, 10, NULL);
   /* Starting the RTOS scheduler will start the timer running as it has already been set into the active state. */
    vTaskStartScheduler();
	int cnt = 0;

	while (1) {
	    ESP_LOGI(TAG, "cnt: %d\n", cnt++);
        vTaskDelay(1000 / portTICK_RATE_MS);
	    printf("Check !");
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
    }
}
