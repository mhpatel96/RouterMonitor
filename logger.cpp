
#include "logger.h"

#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

/* Prevent C++ name mangling */
#if defined(__cplusplus)
extern "C" {
#endif  /* __cplusplus */
void Logger_Task(void *args)
{
  (void) args;

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(10));
    LOGGER.Flush();
  }
}
#if defined(__cplusplus)
}
#endif  /* __cplusplus */
  
const char *Logger::s_LogLevelText[4] = {
#define X(Value, Text, Name) Text,
  LOG_LEVEL_MAP
#undef X
};

Logger::Logger(void) : 
  m_LogModuleCount(0), 
  m_LogQueuePos(0)
{
  Serial.begin(115200);

  for (int i = 0; i < s_LogQueueSize; i++)
  {
    m_LogModules[i].Level = Logger::LogLevel_eT::LogLevel_None;
  }

  strncpy(m_LogModules[s_LogQueueSize - 1].Name, "All", s_LogQueueSize);

  (void) xTaskCreate(Logger_Task, "Logger_Task", configMINIMAL_STACK_SIZE, nullptr, tskIDLE_PRIORITY + 1, nullptr);

  Serial.println("Logger: Initialised");
}

Logger::~Logger()
{
}

int Logger::Register(const char *Name)
{
  int ModuleID = m_LogModuleCount++;

  if (m_LogModuleCount < (s_LogQueueSize - 1))
  {
    strncpy(m_LogModules[ModuleID].Name, Name, s_LogQueueSize);
  }

  Serial.print("Logger: Registered module: ");
  Serial.print(ModuleID);
  Serial.print("|");
  Serial.println(Name);

  return ModuleID;
}

void Logger::Level(int ID, LogLevel_eT NewLogLevel)
{
  m_LogModules[ID].Level = NewLogLevel;

  Serial.print("Logger: Set module level - ");
  Serial.print(ID);
  Serial.print("|");
  Serial.print(m_LogModules[ID].Name);
  Serial.print("|");
  Serial.println(s_LogLevelText[(int)NewLogLevel]);
}

int Logger::LogF(int ID, LogLevel_eT Level, const char *Format, ...)
{
  int ret = 0;

  /* Check verbosity level and queue size */
  if ((Level > m_LogModules[ID].Level) || (ID >= m_LogModuleCount))
  {
    ret = -2;
  }
  else if (m_LogQueuePos >= s_LogQueueSize)
  {
    if (m_LogQueuePos < 0)
    {
      m_LogQueuePos = 0;
    }
    else
    {
      ret = -1;
    }
  }

  if (ret == 0)
  {
    /* Format string and add to queue */
    va_list Args;
    va_start(Args, Format);
    vsnprintf(m_LogQueue[m_LogQueuePos].Message, s_LogQueueMaxMessageLength, Format, Args);
	  va_end(Args);

    /* Update queue */
    m_LogQueue[m_LogQueuePos].ID = ID;
    m_LogQueue[m_LogQueuePos].Level = Level;
    m_LogQueuePos++;
  }

  return ret;
}

void Logger::Flush(void)
{
  int OutputBufferPos = 0;
  char OutputBuffer[80] = {0};
  const char *TargetString = nullptr;
  int TargetStringLength = 0;

  int TempQueuePos = 0;
  while (TempQueuePos < m_LogQueuePos)
  {
    /* Module name */
    TargetString = m_LogModules[m_LogQueue[TempQueuePos].ID].Name;
    TargetStringLength = strlen(TargetString);
    memcpy(&OutputBuffer[0], TargetString, TargetStringLength);
    OutputBufferPos += TargetStringLength;
    OutputBuffer[OutputBufferPos++] = ' ';
    OutputBuffer[OutputBufferPos++] = '-';
    OutputBuffer[OutputBufferPos++] = ' ';

    /* Log level */
    TargetString = s_LogLevelText[(int)m_LogQueue[TempQueuePos].Level];
    TargetStringLength = strlen(TargetString);
    memcpy(&OutputBuffer[OutputBufferPos], TargetString, TargetStringLength);
    OutputBufferPos += TargetStringLength;
    OutputBuffer[OutputBufferPos++] = ':';
    OutputBuffer[OutputBufferPos++] = ' ';

    /* Message */
    TargetString = m_LogQueue[TempQueuePos].Message;
    TargetStringLength = strlen(TargetString);
    memcpy(&OutputBuffer[OutputBufferPos], TargetString, TargetStringLength);

    Serial.println(OutputBuffer);

    TempQueuePos++;
  }

  m_LogQueuePos = 0;
}
