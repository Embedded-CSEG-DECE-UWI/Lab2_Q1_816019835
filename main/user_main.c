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
 * GPIO0:  input, pulled up, interrupt from falling edge
 *
 * Test:
 * Connect GPIO0 with GPIO2
 * Generate pulses on GPIO2, that triggers interrupt on GPIO0
 *
 */

#define GPIO_OUTPUT_IO_0    0
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_IO_0)
#define GPIO_INPUT_IO_0     2
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_0) 

static xQueueHandle gpio_evt_queue = NULL;

/*A mutex must be created in a task because the task that owns the mutex 'inherits' the priority of the task attempting to 'take' the same mutex.*/

static void gpio_task_1_sharingPin( void * pvParameters )
{
    if ( xSemaphore != NULL ){
       /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
       if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
       /* Accessing shared resources , GPIO pin 2
	* Turning on the LED connected to GPIO pin 2 */
       io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
       gpio_config(&io_conf);

       long long int Tbgein = esp_timer_get_time();
       print("Timer for 0.5 s\n");
       long long int Tend = esp_timer_get_time();
       float diff = ( Tend - Tbegin )/1000;
       while (diff <= 500 ){
	       Tend = esp_timer_get_time();
	       diff = ( Tend - Tbegin )/1000;
       }
       Tbegin = esp_timer_get_time();
       printf("Actively waited for %u ms...\n", diff);
       printf("Time for 1s\n");
       Tend = esp_timer_get_time();
       diff = ( Tend - Tbegin )/1000;
       while (diff <= 1000 ){
	       Tend = esp_timer_get_time();
	       diff = ( Tend - Tbegin )/100;
       }
       printf("Delayed for %d ms...\n", diff);

       /*Finished accessing shared resource. Release the semaphore. */ 
       xSemaphoreGive( xSemaphore );
        }
       else{
       /*Semaphore was not obtain and therefore 
       * cannot access the shared resoucre safely. */
       printf("Waiting for the semaphore to release resource");

    }
}

static void gpio_task_2_sharingPin( void *pvParameters )
{
    if (xSemaphore != NULL ){
       /*See if we can obtain the semaphore. If the semaphore is not 
	* available wait 10 ticks to see if it becomes free. */
       if ( xSemaphore( xSemaphore, ( TickType_t _ 10 ) == pdTrue )
          /*Accessing shared resources , GPIO pin 2
	   * Turning off the LED connected to GPIO pin 2 */
          //io_conf.pin_bit mask = 

}

static void gpio_task_3_sharingPin(void *arg)
{
    uint32_t io_num3;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num3, portMAX_DELAY)) {
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", io_num3, gpio_get_level(io_num3));
        }
    }
}

void app_main(void)
{
    gpio_config_t io_conf;

    //Creating mutex
    xSemaphore = xSemaphoreCreateMutex();
    //start task1
    xTaskCreate(gpio_task_1_sharingPin, "gpio_task_1_sharingPin", 2048, NULL, 10, NULL);
    //start task2
    xTaskCreate(gpio_task_2_sharingPin, "gpio_task_2_sharingPin", 2048, NULL, 10, NULL);
    //start task3
    xTaskCreate(gpio_task_3_sharingPin, "gpio_task_3_sharingPin", 2048, NULL, 10, NULL);

}


