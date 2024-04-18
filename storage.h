/**
 * @file Storage.h
 * @brief Chip Storage API
 */

#if !defined(INC_STORAGE_H__)
#define INC_STORAGE_H__

#include <stddef.h>

#include "Observer.h"

#define STORAGE   Storage::Instance()

class Storage : private Observer
{
public:

  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static Storage& Instance(void)
  {
    static Storage StorageInstance;
    return StorageInstance;
  }

  /**
   * @brief   Updates storage manager
   */
  void Update(void);

private:

  enum class State_eT
  {
    Startup,
    Normal,
    Full
  } 
  m_State;

  /*! Logger key */
  const int m_LoggerID;

  /**
   * @brief   Writes to Storage
   * @param   Address         Address to write to
   * @param   Buffer          Pointer to write buffer
   * @param   BufferSizeBytes Size of buffer in bytes
   * @return  0 on successful read
   */
  int Write(int Address, void *Buffer, size_t BufferSizeBytes);

  /**
   * @brief   Reads from Storage
   * @param   Address       Address to read from
   * @param   Buffer        Pointer to read buffer
   * @param   ReadSizeBytes Size of buffer in bytes
   * @return  0 on successful read
   */
  int Read(int Address, void *Buffer, size_t ReadSizeBytes);
  
  /**
   * @brief Observer event handler
   * @param SenderID  Sender ID number
   * @param MessageID Message ID number
   * @param Context   Message payload
   */
  void EventHandler(int SenderID, int MessageID, void *Context) override;

  /**
   * @brief No args constructor, uses compile time/date for initial value
   */
  Storage(void);

  /**
   * @brief Destructor
   */
  ~Storage();
  
  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  Storage(Storage &Other) = delete;
  void operator=(const Storage &) = delete;
};

#endif /* INC_STORAGE_H__ */