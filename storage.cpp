
#include "storage.h"
#include "logger.h"

#include <Arduino.h>
#include <Arduino_due_FreeRTOS.h>

Storage::Storage(void) :
  m_State(Storage::State_eT::Startup),
  m_LoggerID(LOGGER.Register("Storage"))
{
  LOG_LEVEL_DEBUG;
}

Storage::~Storage()
{
}

void Storage::Update(void)
{
  switch (m_State)
  {
    case Storage::State_eT::Startup:

      break;

    case Storage::State_eT::Normal:

      break;

    case Storage::State_eT::Full:

      break;

  }
}

int Storage::Read(int Address, void *Buffer, size_t ReadSizeBytes)
{
  (void) Address;
  (void) Buffer;
  (void) ReadSizeBytes;
  return -1;
}

int Storage::Write(int Address, void *Buffer, size_t BufferSizeBytes)
{
  (void) Address;
  (void) Buffer;
  (void) BufferSizeBytes;
  return -1;
}

void Storage::EventHandler(int SenderID, int MessageID, void *Context)
{
  (void) SenderID;
  (void) MessageID;
  (void) Context;
}