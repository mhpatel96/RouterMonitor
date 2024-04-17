
#include "eeprom.h"
#include "logger.h"

EEPROM::EEPROM(void) : m_LoggerID(LOGGER.Register("EEPROM"))
{
}

EEPROM::~EEPROM()
{
}

int EEPROM::Read(int Address, void *Buffer, size_t ReadSizeBytes)
{
  (void) Address;
  (void) Buffer;
  (void) ReadSizeBytes;
  return -1;
}

int EEPROM::Write(int Address, void *Buffer, size_t BufferSizeBytes)
{
  (void) Address;
  (void) Buffer;
  (void) BufferSizeBytes;
  return -1;
}