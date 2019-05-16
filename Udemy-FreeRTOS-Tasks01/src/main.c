#include "esp_common.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"

#define   R_LED 15
#define   B_LED 14
#define   Y_LED 12

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

void initPins() {
    gpio16_output_conf();
    
    gpio16_output_set(1);
    GPIO_OUTPUT_SET(2, GPIO_INPUT_GET( R_LED )^1 );
    gpio16_output_set(1);
    GPIO_OUTPUT_SET(2, GPIO_INPUT_GET( B_LED )^1 );
    gpio16_output_set(1);
    GPIO_OUTPUT_SET(2, GPIO_INPUT_GET( Y_LED )^1 );

    vTaskDelay(500 / portTICK_RATE_MS);
}

RLedControllerTask( void* pvParameters ) {
    RLEDProfiler++;
    while(1) {
        if ( gpio16_input_get() > 1 ) {
            gpio16_output_set(0);
        } else {
            gpio16_output_set(1);
        }
        GPIO_OUTPUT_SET(2, GPIO_INPUT_GET( R_LED )^1 );
        printf("This is RED. RLEDProfiler : %d",RLEDProfiler);
    }
}
BLedControllerTask( void* pvParameters ) {
    BLEDProfiler++;
    while(1) {
        if ( gpio16_input_get() > 1 ) {
            gpio16_output_set(0);
        } else {
            gpio16_output_set(1);
        }
        GPIO_OUTPUT_SET(2, GPIO_INPUT_GET( B_LED )^1 );
        printf("This is BLUE. BLEDProfiler : %d",BLEDProfiler);
    }
}
YLedControllerTask( void* pvParameters ) {
    YLEDProfiler++;
    while(1) {
        if ( gpio16_input_get() > 1 ) {
            gpio16_output_set(0);
        } else {
            gpio16_output_set(1);
        }
        GPIO_OUTPUT_SET(2, GPIO_INPUT_GET( Y_LED )^1 );
        printf("This is YELLOW. YLEDProfiler : %d",YLEDProfiler);
    }
}

void setup() {
    initPins();
    
    xTaskCreate( &RLedControllerTask, "RED LED Task", 100, NULL, 1, NULL );
    xTaskCreate( &BLedControllerTask, "BLUE LED Task", 100, NULL, 1, NULL );
    xTaskCreate( &YLedControllerTask, "YELLOW LED Task", 100, NULL, 1, NULL );
}

// Name "user_init" required as so by 8266 RTOS
void user_init(void) {
    printf("\nUdemy FreeRTOS Course - 01 Task Creation\n");
    setup();
}