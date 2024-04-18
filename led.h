/**
 * @file led.h
 * @brief LED manager
 */

#if !defined(INC_LED_H__)
#define INC_LED_H__

#define LED   Led::Instance()

#include "Observer.h"

/*! Maps day texts to enum values */
#define PATTERN_MAP \
  X( 0x00, BlinkFast, false, true, false, true, false, true, false, true, false, true) \
  X( 0x01, BlinkSlow, false, false, false, false, false, true, true, true, true, true) \
  X( 0x02, Morse,     false, false, true, true, false, false, false, true, true, true)

class Led : private Observer
{
public:

  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static Led& Instance(void)
  {
    static Led LedInstance;
    return LedInstance;
  }

  /**
   * @brief   Updates LED
   */
  void Update(void);

private:

  enum class State_eT
  {
    Off, On
  } 
  m_State;

  enum class Pattern_eT
  {
#define X(Value, Name, ...) Name = Value,
    PATTERN_MAP
#undef X
  } 
  m_Pattern;

  static constexpr int s_PatternRes = 10;
  static const bool s_PatternMap[3][s_PatternRes];

  unsigned int m_PatternStep;

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
   * @brief No args constructor
   */
  Led(void);

  /**
   * @brief Destructor
   */
  ~Led();

  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  Led(Led &Other) = delete;
  void operator=(const Led &) = delete;

};

#endif /* INC_LED_H__ */