/**
 * @file internet.h
 * @brief Internet connection handler
 */

#include "Observer.h"

#if !defined(INC_INTERNET_H__)
#define INC_INTERNET_H__

#define INTERNET    Internet::Instance()

class Internet : private Observer
{
public: 

  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static Internet& Instance(void)
  {
    static Internet InternetInstance;
    return InternetInstance;
  }

  /**
   * @brief   Updates driver
   */
  void Update(void);
  
private:

  /*! Logger key */
  const int m_LoggerID;
  
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
  Internet(void);

  /**
   * @brief Destructor
   */
  ~Internet();
  
  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  Internet(Internet &Other) = delete;
  void operator=(const Internet &) = delete;

};

#endif /* INC_INTERNET_H__ */