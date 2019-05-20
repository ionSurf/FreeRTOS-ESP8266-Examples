#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_common.h"
#include "gpio.h"
#include <stdio.h>

#define RLED_GPIO 17
#define BLED_GPIO 16
#define YLED_GPIO 15

//  Create 2 queue handles
xQueueHandle xQueue1, xQueue2;

//  Use QueueSetHandle_t xQueueSet;
xQueueSetHandle xQueueSet;

//. Define queue item size
#define QUEUE_ITEM_SIZE sizeof( char* )
#define QUEUE1_LENGTH 5
#define QUEUE2_LENGTH 3

// Init GPIO function ( using LEDs )
int gpio_config() {
}

// Function setup:
int setup () {
    //  Create 2 queues
    portBASE_TYPE xStatus1, xStatus2;

    xStatus1 = xQueueCreate( QUEUE1_LENGTH, QUEUE_ITEM_SIZE );
    xStatus2 = xQueueCreate( QUEUE2_LENGTH, QUEUE_ITEM_SIZE );

    //  Create one queue set xQueueSet= xQueueCreateSet( 1*2 );
    xQueueSet = xQueueSetCreate( QUEUE1_LENGTH + QUEUE2_LENGTH );

    //  Add queues to set : xQueueAddToSet(Queue ,QueueSet);
    xQueueAddToSetQueue( xQueue1, xQueueSet );

    //  Create 2 senders tasks for 2 functions ( try one function
    //  which receives a struct with all params )
}

//  Create one receiving task which get a queue from the set ( portMAX_DELAY )
