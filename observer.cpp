
#include "observer.h"
#include "logger.h"

#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

/* Prevent C++ name mangling */
#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */
void Publisher_Task(void *args)
{
  (void) args;

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(10));
    PUBLISHER.Update();
  }
}
#if defined(__cplusplus)
}
#endif  /* __cplusplus */

Publisher::Publisher(void) : 
  m_LoggerID(LOGGER.Register("Publisher"))
{
  LOG_LEVEL_DEBUG;
  
  memset(m_ObserverList, 0, sizeof(m_ObserverList));
  for (int i = 0; i < m_MaxObservers; i++)
  {
    m_ObserverList[i].Sender = SENDER_ID_NONE;
  }

  (void) xTaskCreate(Publisher_Task, "Publisher_Task", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY + 1, nullptr);
}

Publisher::~Publisher()
{
}

void Publisher::Update(void)
{
  for (int i = 0; i < m_MaxObservers; i++)
  {
    if (m_ObserverList[i].Sender != SENDER_ID_NONE)
    {
      for (int j = 0; j < m_ObserverList[i].NumSubscribers; j++)
      {
        m_ObserverList[i].Subscribers[j]->EventHandler(m_ObserverList[i].Sender, i, m_ObserverList[i].Context);
      }
      m_ObserverList[i].Sender = SENDER_ID_NONE;
    }
  }
}

int Publisher::Subscribe(int MessageID, Observer *Subscriber)
{
  int ret = -1;

  if (m_ObserverList[MessageID].NumSubscribers < m_MaxObservers)
  {
    m_ObserverList[MessageID].Subscribers[m_ObserverList[MessageID].NumSubscribers++] = Subscriber;
    ret = 0;
  }

  return ret;
}

int Publisher::Publish(int SenderID, int MessageID, void *Context)
{
  int ret = -1;

  if (m_ObserverList[MessageID].Sender != SENDER_ID_NONE)
  {
    m_ObserverList[MessageID].Sender = SenderID;
    m_ObserverList[MessageID].Context = Context;
    ret = 0;
  }

  return ret;
}