
#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

#include "rtc.h"
#include "logger.h"
#include "storage.h"
#include "internet.h"

static int m_LoggerID;

static void Test_Task(void *args)
{
  (void) args;

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

  PUBLISHER;
  STORAGE;
  REALTIMECLOCK;
  INTERNET;

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
