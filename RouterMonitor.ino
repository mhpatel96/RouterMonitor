
#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

#include "rtc.h"
#include "logger.h"

#define THREAD_SLEEP_MS(ms)   vTaskDelay(pdMS_TO_TICKS(ms))

static int m_LoggerID;

static void LED_Task(void *args)
{
  (void) args;

  static bool flip = false;
  pinMode(LED_BUILTIN, OUTPUT);

  while (1)
  {
    digitalWrite(LED_BUILTIN, flip);
    flip = !flip;
    THREAD_SLEEP_MS(500);
  }
}

static void Test_Task(void *args)
{
  (void) args;

  REALTIMECLOCK;
  RealTimeClock::Time_sT TimeCurrent;

  while (1)
  {
    THREAD_SLEEP_MS(2000);
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
