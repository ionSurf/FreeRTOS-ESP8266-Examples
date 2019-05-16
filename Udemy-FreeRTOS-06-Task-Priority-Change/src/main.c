#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_common.h"
#include "gpio.h"
#include <stdio.h>

#define   R_LED 15
#define   B_LED 14
#define   Y_LED 12

typedef int TaskProfiler;

TaskProfiler RLEDProfiler;
TaskProfiler BLEDProfiler;
TaskProfiler YLEDProfiler;

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
     RLEDProfiler++;
     printf("RedTask Run Time : %d",RLEDProfiler);
     
  //  digitalWrite(RED, digitalRead(RED)^1);
    vTaskDelay( 300 / portTICK_RATE_MS );
  }
}

void blueLedControllerTask(void *pvParameters) {
  
  while(1) {
    BLEDProfiler++;
    printf("BlueTask Run Time : %d",BLEDProfiler);
    
    //digitalWrite(BLUE,digitalRead(BLUE)^1);
    vTaskDelay( 300 / portTICK_RATE_MS );
  }
}

void yellowControllerTask(void *pvParameters) {
  
  while(1) {
    YLEDProfiler++;
    printf("YellowTask Run Time : %d", YLEDProfiler);
    //digitalWrite(YELLOW,digitalRead(YELLOW)^1);
    
    vTaskPrioritySet(NULL,2);
    vTaskDelay( 300 / portTICK_RATE_MS );
  }
}

xTaskHandle blue_Handle,red_Handle, yellow_Handle;

int gpio_init() {
    gpio16_output_conf();
    
    gpio16_output_set(0);
    GPIO_OUTPUT_SET(R_LED, 0 );
    GPIO_OUTPUT_SET(B_LED, 0 );
    GPIO_OUTPUT_SET(Y_LED, 0 );
    return 1;
}

void setup() {
    gpio_init();

    xTaskCreate(redLedControllerTask, "RED LED Task",100,NULL,1,&red_Handle);
    xTaskCreate(blueLedControllerTask, "BLUE LED Task", 100, NULL, 1,&blue_Handle);
    xTaskCreate(yellowControllerTask, "YELLOW LED Task", 100,NULL,1,&yellow_Handle);  
}

void user_init(void) {
    printf("\nUdemy FreeRTOS Course - 06 Task Priority Change\n");
    setup();
}

