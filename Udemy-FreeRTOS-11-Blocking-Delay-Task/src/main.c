#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_common.h"
#include "gpio.h"
#include <stdio.h>

#define   R_GPIO 15
#define   B_GPIO 14
#define   Y_GPIO 12

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


//const portTickType _500ms = pdMS_TO_TICKS(500); 
//pdMSTOTICKS( 500 );
const portTickType _500ms = 500 / portTICK_RATE_MS;


int init_gpio() {
    gpio16_output_conf();

    gpio16_output_set(1);
    GPIO_OUTPUT_SET(R_GPIO, 0);
    GPIO_OUTPUT_SET(B_GPIO, 0);
    GPIO_OUTPUT_SET(Y_GPIO, 0);

    return 1;
}

void R_Controller_Task(void *pvParameters) {
  while(1) {
      RLEDProfiler++;
      printf("RED Run Time : %d",RLEDProfiler);

      gpio16_output_set(0);
      GPIO_OUTPUT_SET( R_GPIO, 1 );
      vTaskDelay( _500ms );
      gpio16_output_set(1);
      GPIO_OUTPUT_SET( R_GPIO, 0 );
      vTaskDelay( _500ms );
  }
}
void B_Controller_Task(void *pvParameters) {
    while(1) {
        BLEDProfiler++;
        printf("BLUE Run Time : %d", BLEDProfiler);
      vTaskDelay( 1 );
  }
}
void Y_Controller_Task(void *pvParameters) {
    while(1) {
        YLEDProfiler++;
        printf("YELLOW Run Time : %d", YLEDProfiler);
        vTaskDelay( 1 );
    }
}

void setup() {
    init_gpio();
    xTaskCreate(&R_Controller_Task, "RED LED Task", 100, NULL, 1, NULL);
    xTaskCreate(&B_Controller_Task, "BLUE LED Task", 100, NULL, 1, NULL);
    xTaskCreate(&Y_Controller_Task, "YELLOW LED Task", 100, NULL, 1, NULL); 
}

void user_init(void) {
    printf("\nUdemy FreeRTOS Course - 11 Blocking a Task with vDelay\n");

    setup();
}