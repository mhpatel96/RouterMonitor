/**
 * @file rtc.h
 * @brief Chip real time clock API
 */

#if !defined(INC_RTC_H__)
#define INC_RTC_H__

#include "Observer.h"

#define REALTIMECLOCK            RealTimeClock::Instance()

/*! Maps day texts to enum values */
#define DAY_MAP \
  X( 0x00, "Mon", Mon) \
  X( 0x01, "Tue", Tue) \
  X( 0x02, "Wed", Wed) \
  X( 0x03, "Thu", Thu) \
  X( 0x04, "Fri", Fri) \
  X( 0x05, "Sat", Sat) \
  X( 0x06, "Sun", Sun)

/*! Maps month texts to enum values */
#define MONTH_MAP \
  X(0x00 , "Jan", Jan, 31) \
  X(0x01 , "Feb", Feb, 29) \
  X(0x02 , "Mar", Mar, 31) \
  X(0x03 , "Apr", Apr, 30) \
  X(0x04 , "May", May, 31) \
  X(0x05 , "Jun", Jun, 30) \
  X(0x06 , "Jul", Jul, 31) \
  X(0x07 , "Aug", Aug, 31) \
  X(0x08 , "Sep", Sep, 30) \
  X(0x09 , "Oct", Oct, 31) \
  X(0x0A , "Nov", Nov, 30) \
  X(0x0B , "Dec", Dec, 31)

/**
 * @class RealTimeClock
 * @brief Provides application level to real time clock peripheral.
 * @note  Singleton class
 *        There is only a single RTC peripheral available on this chip
 */
class RealTimeClock : private Observer
{
public:

  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static RealTimeClock& Instance(void)
  {
    static RealTimeClock RealTimeClockInstance;
    return RealTimeClockInstance;
  }

  /**
   * @enum   Day references
   */
  typedef enum class
  {
#define X(Value, Text, Name) Name = Value,
    DAY_MAP
#undef X
  } Day_eT;

  /**
   * @enum   Month references
   */
  typedef enum class
  {
#define X(Value, Text, Name, NumDays) Name = Value,
    MONTH_MAP
#undef X
  } Month_eT;

  /**
   * @struct RTC data structure
   */
  typedef struct
  {
    int Second;
    int Minute;
    int Hour;
    int Century;
    int Year;
    Month_eT Month;
    int Date;
    Day_eT Day;
  } Time_sT;

  /**
   * @brief Sets start time
   * @param StartTime Initial time data structure
   */
  void Set(const Time_sT &StartTime);

  /**
   * @brief   Gets current time
   * @return  Current time value (duh)
   */
  Time_sT Get(void);

  /**
   * @brief   Gets formatted time string
   * @param   Time Time data structure to print
   * @return  Pointer to formatted text string
   */
  char* GetFormatted(const Time_sT &Time);

private:

  /*! Day text strings, indexed to corresponding enum */
  static const char *s_DayText[7];
  /*! Month text strings, indexed to corresponding enum */
  static const char *s_MonthText[12];
  /*! Month num days, indexed to corresponding enum */
  static const int s_MonthNumDays[12];

  /*! Formatted time string */
  char m_FormattedTime[64];

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
  RealTimeClock(void);

  /**
   * @brief Destructor
   */
  ~RealTimeClock();
  
  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  RealTimeClock(RealTimeClock &Other) = delete;
  void operator=(const RealTimeClock &) = delete;
};

#endif /* INC_RTC_H__ */
