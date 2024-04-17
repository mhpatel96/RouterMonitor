/**
 * @file eeprom.h
 * @brief Chip EEPROM API
 */

#if !defined(INC_EEPROM_H__)
#define INC_EEPROM_H__

#include <stddef.h>

class EEPROM
{
public:

  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static EEPROM& Instance(void)
  {
    static EEPROM EEPROMInstance;
    return EEPROMInstance;
  }

  /**
   * @brief   Writes to EEPROM
   * @param   Address         Address to write to
   * @param   Buffer          Pointer to write buffer
   * @param   BufferSizeBytes Size of buffer in bytes
   * @return  0 on successful read
   */
  int Write(int Address, void *Buffer, size_t BufferSizeBytes);

  /**
   * @brief   Reads from EEPROM
   * @param   Address       Address to read from
   * @param   Buffer        Pointer to read buffer
   * @param   ReadSizeBytes Size of buffer in bytes
   * @return  0 on successful read
   */
  int Read(int Address, void *Buffer, size_t ReadSizeBytes);

private:

  /*! Logger key */
  const int m_LoggerID;

  /**
   * @brief No args constructor, uses compile time/date for initial value
   */
  EEPROM(void);

  /**
   * @brief Destructor
   */
  ~EEPROM();
  
  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  EEPROM(EEPROM &Other) = delete;
  void operator=(const EEPROM &) = delete;
};

#endif /* INC_EEPROM_H__ */