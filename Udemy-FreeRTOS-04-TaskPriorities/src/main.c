#include "esp_common.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"

#define     R_LED 15
#define     B_LED 14
#define     Y_LED 12

#define     D_LOW 0
#define     D_HIGH 1

typedef int TaskProfiler;

TaskProfiler RLEDProfiler = 0;
TaskProfiler BLEDProfiler = 0;
TaskProfiler YLEDProfiler = 0;

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

void redLedControllerTask(void *pvParameters) {
    while(1) {
        gpio16_output_set( GPIO_INPUT_GET( R_LED ) );
        GPIO_OUTPUT_SET(R_LED, GPIO_INPUT_GET( R_LED )^1 );
        vTaskDelay( 500 / portTICK_RATE_MS );
    }
}

void blueLedControllerTask(void *pvParameters) {
    while(1) {
        gpio16_output_set( GPIO_INPUT_GET( B_LED ) );
        GPIO_OUTPUT_SET(R_LED, GPIO_INPUT_GET( B_LED )^1 );
        vTaskDelay( 500 / portTICK_RATE_MS );
    }
}

void yellowLedControllerTask(void *pvParameters) {
    while(1) {
        gpio16_output_set( GPIO_INPUT_GET( Y_LED ));
        GPIO_OUTPUT_SET(R_LED, GPIO_INPUT_GET( Y_LED )^1 );
        vTaskDelay( 500 / portTICK_RATE_MS );
    }
}

void init() {
    gpio16_output_conf();

    gpio16_output_set(1);
    GPIO_OUTPUT_SET(R_LED,D_LOW);
    GPIO_OUTPUT_SET(B_LED,D_LOW);
    GPIO_OUTPUT_SET(Y_LED,D_LOW);
}

void setup() {
    init();

    xTaskCreate(redLedControllerTask,"RED LED Task",100,NULL,3,NULL);
    xTaskCreate(blueLedControllerTask,"BLUE LED Task", 100, NULL,3,NULL);
    xTaskCreate(yellowLedControllerTask,"YELLOW LED Task", 100, NULL,2,NULL);  
}

void user_init(void) {
    printf("\nUdemy FreeRTOS Course - 04 Task Priorities");
    setup();
}
