
#include "led.h"
#include "messages.h"
#include "logger.h"

#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

#define LED_WRITE(value)    digitalWrite(LED_BUILTIN, (value))

const bool Led::s_PatternMap[3][10] = 
{
#define X(Value, Name, ...) {__VA_ARGS__},
    PATTERN_MAP
#undef X
};

/* Prevent C++ name mangling */
#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */
void LED_Task(void *args)
{
  (void) args;

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(100));
    LED.Update();
  }
}
#if defined(__cplusplus)
}
#endif  /* __cplusplus */


Led::Led(void) :
  m_Pattern(Led::Pattern_eT::BlinkFast),
  m_PatternStep(0),
  m_LoggerID(LOGGER.Register("LED"))
{
  LOG_LEVEL_DEBUG;

  pinMode(LED_BUILTIN, OUTPUT);

  PUBLISHER.Subscribe(MESSAGE_ID_INTERNET, this);
  PUBLISHER.Subscribe(MESSAGE_ID_STORAGE, this);

  (void) xTaskCreate(LED_Task, "LED_Task", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY + 1, nullptr);
}

Led::~Led()
{
}

void Led::Update(void)
{
  if (m_State == Led::State_eT::On)
  {
    LED_WRITE(s_PatternMap[(int)m_Pattern][m_PatternStep++]);
  }

  if (m_PatternStep >= s_PatternRes)
  {
    m_PatternStep = 0;
  }
}

void Led::EventHandler(int SenderID, int MessageID, void *Context)
{
  (void) SenderID;

  int State = (int) Context;
  switch (MessageID)
  {
   case MESSAGE_ID_INTERNET:
    m_State = Led::State_eT::On;
    if (State)
    {
      m_Pattern = Led::Pattern_eT::BlinkSlow;
    }
    else
    {
      m_Pattern = Led::Pattern_eT::BlinkFast;
    }
    break;

   case MESSAGE_ID_STORAGE:
    if (State)
    {
      m_State = Led::State_eT::On;
      m_Pattern = Led::Pattern_eT::Morse;
    }
    else
    {
      m_State = Led::State_eT::Off;
    }    
    break;
  }
  (void) Context;
}