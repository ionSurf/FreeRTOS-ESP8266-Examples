#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_common.h"
#include "gpio.h"
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

xQueueHandle yearQueue;

void vSenderTask(void *pvParameters) {
  //BaseType_t qStatus;
  portBASE_TYPE qStatus;

  int32_t valueToSend = 1999;
  while(1) {
    //qStatus = xQueueSend(yearQueue,&valueToSend,0);
    if ( xQueueSend(yearQueue,&valueToSend,0) != pdPASS ) {
      printf("Could not send message to queue.");
      exit(0);
    }
  }
}

void vReceiverTask(void *pvParameters) {
  int32_t valueReceived;
  //BaseType_t qStatus;
  portBASE_TYPE qStatus;
  //const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  //const portTickType xTicksToWait = pdMS_TO_TICKS(100);
  
  const portTickType xTicksToWait = 100;

  qStatus = xQueueReceive( yearQueue, &valueReceived, xTicksToWait );

  while(1) {
    //if ( xQueueReceive(yearQueue,&valueReceived,xTicksToWait) != pdPASS ) {
    //if ( xQueueReceive( yearQueue, &valueReceived, xTicksToWait ) != pdPASS ) {
    if ( qStatus != pdPASS ) {
      printf("Could not recieve message from queue.");
      exit(0);
    }
    
    printf("Received value  : %d", valueReceived);
    vTaskDelay(1);
  }
}

void setup() {
 
 yearQueue =  xQueueCreate(5,sizeof(int32_t));

 xTaskCreate(vSenderTask,"Sender Task",100,NULL,1,NULL);
 xTaskCreate(vReceiverTask,"Receiver Task", 100,NULL, 1, NULL);
  
}

void user_init(void){
  printf("\nUdemy FreeRTOS Course - 12 Simple Queue\n");

  vTaskDelay( 3000 / portTICK_RATE_MS );
  setup();
}