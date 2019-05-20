// setup() function
// Create queue w/handler
// Create 2 tasks referencing one same function for sending
// Create 1 task to receive, wait 100 ticks to receive

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_common.h"
#include <stdio.h>

uint32 user_rf_cal_sector_set(void) {
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

#define TicksTOSEND    0
#define TicksTORECEIVE 100

#define DELAYTOSEND    1
#define DELAYTORECEIVE 1

xQueueHandle myQueue;

void setup () {
    myQueue = xQueueCreate( 5, sizeof( int32_t ) );

    xTaskCreate( &TxTask, "TxTask1", 100, NULL, 1, NULL );
    xTaskCreate( &TxTask, "TxTask2", 100, NULL, 1, NULL );
    xTaskCreate( &RxTask, "RxTask", 100, NULL, 1, NULL );
}

void TxTask ( void* pvParams ) {
    portBASE _TYPE status;
    uint8_t message = 1990;

    while( 1 ) {
        status = xQueueSend( myQueue, &message, TicksTOSEND );
        if ( status != pdPASS )
            printf( "Could not write to queue" );
        
        printf( "Sent %d to queue from ", message );
        vTaskDelay( DELAYTOSEND );
    }
}

void RxTask ( void* pvParams ) {
    portBASE _TYPE status;
    uint8_t message;

    while( 1 ) {
        status = xQueueReceive( myQueue, &message, TicksTORECEIVE );        
        if ( status != pdPASS )
            printf( "Could not read from queue" );
        
        printf( "Received %d from queue from ", message );
        vTaskDelay( DELAYTORECEIVE );
    }

}

void user_init ( void ) {
    printf("\nUdemy FreeRTOS Course - 13 Writing to Queue from 2 tasks\n");

    setup();
}
