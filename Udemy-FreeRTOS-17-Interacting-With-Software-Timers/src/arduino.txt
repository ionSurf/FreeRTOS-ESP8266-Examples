#include <Arduino_FreeRTOS.h>
#include "timers.h"

#define RED       6
#define YELLOW    7
#define BLUE      8

#define  BLUE_TIMER_PERIOD   (pdMS_TO_TICKS(120))
#define  RED_TIMER_PERIOD    (pdMS_TO_TICKS(300))

TimerHandle_t   xBlueTimer,xRedTimer; 

void setup()
{
  Serial.begin(9600);
  pinMode(RED,OUTPUT);
  pinMode(BLUE,OUTPUT);
  
  BaseType_t  xBlueTimerStarted, xRedTimerStarted;

  xBlueTimer  = xTimerCreate("Blue Timer",BLUE_TIMER_PERIOD,pdTRUE,0,prvTimerCallback);
  xRedTimer   = xTimerCreate("Red Timer", RED_TIMER_PERIOD, pdTRUE,0,prvTimerCallback);

 if((xBlueTimer != NULL) && (xRedTimer !=NULL))
  {
     xBlueTimerStarted  = xTimerStart(xBlueTimer,0);
     xRedTimerStarted   = xTimerStart(xRedTimer,0);

     while((xBlueTimerStarted != pdPASS) && (xRedTimerStarted != pdPASS)) {}
  }
  
}
  uint32_t blueExecutionCount;

 void prvTimerCallback(TimerHandle_t xTimer)
 {
   TickType_t xTimeNow;

    xTimeNow =  xTaskGetTickCount();

   if(xTimer == xBlueTimer)
   {
    blueExecutionCount++;
    Serial.print("Blue Timer execution  : ");
    Serial.println(xTimeNow);
    digitalWrite(BLUE,digitalRead(BLUE)^1);
     if(blueExecutionCount ==  7)
     {
      xTimerStop(xBlueTimer,0); 
     }
   }
   else
   {
   Serial.print("Red Timer execution : ");
   Serial.println(xTimeNow);
   digitalWrite(RED,digitalRead(RED)^1);
   }
 }

void loop(){}
