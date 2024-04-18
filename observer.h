/**
 * @file observer.h
 * @brief Observer API
 */

#if !defined(INC_OBSERVER_H__)
#define INC_OBSERVER_H__

#include "messages.h"

#define PUBLISHER   Publisher::Instance()

/**
 * @class Observer
 * @brief Abstract class for observers
 */
class Observer
{
public:

  /**
   * @brief Observer event handler
   * @param SenderID  Sender ID number
   * @param MessageID Message ID number
   * @param Context   Message payload
   */
  virtual void EventHandler(int SenderID, int MessageID, void *Context) = 0;

protected:

private:

};

/**
 * @class Publisher
 * @brief Handles publisher
 *        Periodically kicks off publish task to update observers
 * @note  Singleton class
 */
class Publisher
{
public:

  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static Publisher& Instance(void)
  {
    static Publisher PublisherInstance;
    return PublisherInstance;
  }

  /**
   * @brief   Updates all observers
   */
  void Update(void);

  /**
   * @brief   Subscribes to a message
   * @param   MessageID   Message ID number
   * @param   Subscriber  Subscriber pointer
   */
  int Subscribe(int MessageID, Observer *Subscriber);

  /**
   * @brief   Queues publisher message
   * @param   SenderID  Sender ID number
   * @param   MessageID Message ID number
   * @param   Context   Message payload
   * @return  0 for successful send
   */
  int Publish(int SenderID, int MessageID, void *Context);

private:

  /*! Max number of observers/message */
  static constexpr int m_MaxObservers = 8;

  struct
  {
    Observer *Subscribers[m_MaxObservers];
    int NumSubscribers;

    int Sender;
    void *Context;
  } 
  m_ObserverList[m_MaxObservers];

  /*! Logger key */
  const int m_LoggerID;

  /**
   * @brief Constructor
   */
  Publisher(void);

  /**
   * @brief Destructor
   */
  ~Publisher();
  
  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  Publisher(Publisher &Other) = delete;
  void operator=(const Publisher &) = delete;

};

#endif /* INC_OBSERVER_H__ */