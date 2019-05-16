#include "esp_common.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"

#define   R_LED 15
#define   B_LED 14
#define   Y_LED 12

typedef int TaskProfiler;

TaskProfiler RLEDProfiler;
TaskProfiler BLEDProfiler;
TaskProfiler YLEDProfiler;

const uint16_t *r_led = ( uint16_t * ) R_LED;
const uint16_t *b_led = ( uint16_t * ) B_LED;
const uint16_t *y_led = ( uint16_t * ) Y_LED;

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

void initPins() {
    gpio16_output_conf();
    
    gpio16_output_set(1);
    GPIO_OUTPUT_SET(R_LED, 0 );
    GPIO_OUTPUT_SET(B_LED, 0 );
    GPIO_OUTPUT_SET(Y_LED, 0 );
}

void ledControllerTask( void* pvParameters ) {
    while( 1 ) {
        //digitalWrite( pvParameters, digitalRead( pvParameters )^1 );
        gpio16_output_set(0);
        GPIO_OUTPUT_SET(2, GPIO_INPUT_GET(2)^1 );
        vTaskDelay(500 / portTICK_RATE_MS);
        gpio16_output_set(1);
        GPIO_OUTPUT_SET(2, GPIO_INPUT_GET(2)^1 );
        vTaskDelay(500 / portTICK_RATE_MS);
        printf( "task from %d \n", pvParameters );

        // Switch through task handlers. See who called the task.
    }
}

void setup() {
    initPins();
    
    xTaskCreate( &ledControllerTask, "RED LED Task", 100, (void*)r_led, 1, NULL );
    xTaskCreate( &ledControllerTask, "BLUE LED Task", 100, (void*)b_led, 1, NULL );
    xTaskCreate( &ledControllerTask, "YELLOW LED Task", 100, (void*)y_led, 1, NULL );
}

// Name "user_init" required as so by 8266 RTOS
void user_init(void) {
    printf("\n");
    setup();
}