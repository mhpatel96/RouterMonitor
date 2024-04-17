
#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

#include "rtc.h"
#include "logger.h"

static int m_LoggerID;

static void LED_Task(void *args)
{
  (void) args;

  static bool Flip = false;
  pinMode(LED_BUILTIN, OUTPUT);

  while (1)
  {
    digitalWrite(LED_BUILTIN, Flip);
    Flip = !Flip;
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

static void Test_Task(void *args)
{
  (void) args;

  REALTIMECLOCK;
  RealTimeClock::Time_sT TimeCurrent;

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(2000));
    TimeCurrent = REALTIMECLOCK.Get();
  }
}

void setup(void)
{
  m_LoggerID = LOGGER.Register("Main");
  LOG_LEVEL_DEBUG;

  (void) xTaskCreate(LED_Task, "LED_Task", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY + 1, nullptr);
  (void) xTaskCreate(Test_Task, "Test_Task", 512, nullptr, tskIDLE_PRIORITY + 1, nullptr);

  vTaskStartScheduler();
  Serial.println("Error starting scheduler!");
}

void loop(void)
{
  static volatile uint32_t i = 0;
  i++;
  __asm volatile ("NOP");
}
