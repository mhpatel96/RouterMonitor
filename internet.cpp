
#include "internet.h"
#include "logger.h"

#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

/* Prevent C++ name mangling */
#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */
void Internet_Task(void *args)
{
  (void) args;

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(10));
    INTERNET.Update();
  }
}
#if defined(__cplusplus)
}
#endif  /* __cplusplus */

Internet::Internet(void) : 
  m_LoggerID(LOGGER.Register("Internet"))
{
  LOG_LEVEL_DEBUG;
  (void) xTaskCreate(Internet_Task, "Internet_Task", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY + 1, nullptr);
}

Internet::~Internet()
{
}

void Internet::Update(void)
{

}

void Internet::EventHandler(int SenderID, int MessageID, void *Context)
{
  (void) SenderID;
  (void) MessageID;
  (void) Context;
}