#include <Arduino.h>
#include "freertos/event_groups.h"

/* define event bits */
#define TASK_1_BIT        ( 1 << 0 ) //1
#define TASK_2_BIT        ( 1 << 1 ) //10
#define TASK_3_BIT        ( 1 << 2 ) //100
#define ALL_SYNC_BITS (TASK_1_BIT | TASK_2_BIT | TASK_3_BIT) //111


/* create a hardware timer */
hw_timer_t * timer = NULL;
/* create event group */
EventGroupHandle_t eg;
int count = 0;

/* timer ISR callback */
void IRAM_ATTR onTimer(){
  BaseType_t xHigherPriorityTaskWoken;
  count++;
  if(count == 2){
    /* if counter is equal 2 then set event bit of task1 */
    xEventGroupSetBitsFromISR(eg,TASK_1_BIT, &xHigherPriorityTaskWoken);
  }else if(count == 3){
    /* if counter is equal 3 then set event bit of task 2 and 3 */
    xEventGroupSetBitsFromISR(eg,TASK_2_BIT | TASK_3_BIT, &xHigherPriorityTaskWoken);
  }else if(count == 4){
    /* reset counter to start again */
    count = 0;
  }
}

void setup() {

  Serial.begin(112500);
  eg = xEventGroupCreate();

  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
  timer = timerBegin(0, 80, true);

  /* Attach onTimer function to our timer */
  timerAttachInterrupt(timer, &onTimer, true);

  /* Set alarm to call onTimer function every second 1 tick is 1us
  => 1 second is 1000000us */
  /* Repeat the alarm (third parameter) */
  timerAlarmWrite(timer, 1000000, true);

  /* Start an alarm */
  timerAlarmEnable(timer);
  Serial.println("start timer");
  
  xTaskCreate(
      task1,           /* Task function. */
      "task1",        /* name of task. */
      10000,                    /* Stack size of task */
      NULL,                     /* parameter of the task */
      1,                        /* priority of the task */
      NULL);                    /* Task handle to keep track of created task */
  xTaskCreate(
      task2,           /* Task function. */
      "task2",        /* name of task. */
      10000,                    /* Stack size of task */
      NULL,                     /* parameter of the task */
      1,                        /* priority of the task */
      NULL);                    /* Task handle to keep track of created task */
  xTaskCreate(
      task3,           /* Task function. */
      "task3",        /* name of task. */
      10000,                    /* Stack size of task */
      NULL,                     /* parameter of the task */
      1,                        /* priority of the task */
      NULL);                    /* Task handle to keep track of created task */
}

void loop() {

}

void task1( void * parameter )
{
  for(;;){
    /* wait forever until event bit of task 1 is set */
    EventBits_t xbit = xEventGroupWaitBits(eg, TASK_1_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.print("task1 has even bit: ");
    Serial.println(xbit);
  }
  vTaskDelete( NULL );
}
/* this task is similar to sendTask1 */
void task2( void * parameter )
{

  for(;;){
    /* wait forever until event bit of task 2 is set */
    EventBits_t xbit = xEventGroupWaitBits(eg, TASK_2_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.print("task2 has even bit: ");
    Serial.println(xbit);
  }
  vTaskDelete( NULL );
}
void task3( void * parameter )
{
  for(;;){
    /* wait forever until event bit of task 3 is set */
    EventBits_t xbit = xEventGroupWaitBits(eg, TASK_3_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.print("task3 has even bit: ");
    Serial.println(xbit);
  }
  vTaskDelete( NULL );
}