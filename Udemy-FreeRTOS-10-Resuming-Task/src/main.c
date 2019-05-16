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

// Task Handles
xTaskHandle RTask_H, BTask_H, YTask_H;

// Task Priorities
uint32_t    suspend_monitor;
uint32_t    resume_monitor;
bool        _suspended = false;

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
      vTaskDelay( 10 / portTICK_RATE_MS );
  }
}

void B_Controller_Task(void *pvParameters) {
    while(1) {
        BLEDProfiler++;
        printf("BLUE Run Time : %d", BLEDProfiler);
        
        //  Suspend logic
        suspend_monitor++;
        if(suspend_monitor >= 30) {
            _suspended = true;
            suspend_monitor =0;
            vTaskSuspend(NULL);
      }
      
      vTaskDelay( 10 / portTICK_RATE_MS );
  }
}
void Y_Controller_Task(void *pvParameters) {
    while(1) {
        YLEDProfiler++;
        printf("YELLOW Run Time : %d", YLEDProfiler);

        if ( _suspended ) {
            resume_monitor++;
            if ( resume_monitor >= 10 ) {       // resume_monitor % 10 == 0 :: No. resumes 
                vTaskResume( BTask_H );
                suspend_monitor = 0;
                _suspended      = false;
                resume_monitor  = 0;
            }
        }

        vTaskDelay( 10 / portTICK_RATE_MS );
    }
}

void setup() {
    init_gpio();
    xTaskCreate(&R_Controller_Task, "RED LED Task", 100, NULL, 1, &RTask_H);
    xTaskCreate(&B_Controller_Task, "BLUE LED Task", 100, NULL, 1, &BTask_H);
    xTaskCreate(&Y_Controller_Task, "YELLOW LED Task", 100, NULL, 1, &YTask_H); 
}

void user_init(void) {
    printf("\nUdemy FreeRTOS Course - 10 Resuming a Task\n");

    setup();
}