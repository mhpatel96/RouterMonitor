/**
 * @file logger.h
 * @brief Logger API
 */

#if !defined(INC_LOGGER_H__)
#define INC_LOGGER_H__

#define LOGGER                Logger::Instance()

#define LOG_LEVEL_NONE        LOGGER.Level(m_LoggerID, Logger::LogLevel_eT::LogLevel_None)
#define LOG_LEVEL_ERROR       LOGGER.Level(m_LoggerID, Logger::LogLevel_eT::LogLevel_Error)
#define LOG_LEVEL_INFO        LOGGER.Level(m_LoggerID, Logger::LogLevel_eT::LogLevel_Info)
#define LOG_LEVEL_DEBUG       LOGGER.Level(m_LoggerID, Logger::LogLevel_eT::LogLevel_Debug)

#define LOG_ERROR(...)        LOGGER.LogF(m_LoggerID, Logger::LogLevel_eT::LogLevel_Error, __VA_ARGS__)
#define LOG_INFO(...)         LOGGER.LogF(m_LoggerID, Logger::LogLevel_eT::LogLevel_Info, __VA_ARGS__)
#define LOG_DEBUG(...)        LOGGER.LogF(m_LoggerID, Logger::LogLevel_eT::LogLevel_Debug, __VA_ARGS__)

/*! Maps log level text strings to enum values */
#define LOG_LEVEL_MAP \
  X( 0x00, "None ", LogLevel_None  ) \
  X( 0x01, "Error", LogLevel_Error ) \
  X( 0x02, "Info ", LogLevel_Info  ) \
  X( 0x03, "Debug", LogLevel_Debug )

/**
 * @class Logger
 * @brief Handles logger
 *        Each componenent must register themself with the logger and be assigned an ID
 *        Log priorities are assigned on a per component basis
 * @note  Singleton class
 */
class Logger
{
public:
  /**
   * @brief   Constructs and gets singleton instance reference
   * @return  Singleton reference
   */
  static Logger& Instance(void)
  {
    static Logger LoggerInstance;
    return LoggerInstance;
  }

  /**
   * @enum Logger verbosity level type
   */
  typedef enum class 
  {
#define X(Value, Text, Name) Name = Value,
    LOG_LEVEL_MAP
#undef X
  } LogLevel_eT;

  /*! Logger text strings, indexed to corresponding enum */
  static const char *s_LogLevelText[4];

  /**
   * @enum    Registers module with logger
   * @param   Name  Module name
   * @return  Module ID
   */
  int Register(const char *Name);

  /**
   * @enum  Sets logger verbosity level
   * @param NewLogLevel New logger verbosity level
   */
  void Level(int ID, LogLevel_eT NewLogLevel);

  /**
   * @enum    Logs formatted string
   * @param   ID      Module ID
   * @param   Level   Verbosity level
   * @param   Format  Format string
   * @return  0 for successful log
   */
  int LogF(int ID, LogLevel_eT Level, const char *Format, ...);

  /**
   * @enum  Flushes log buffer
   */
  void Flush(void);

private:

  static constexpr int s_LogQueueSize = 8;
  static constexpr int s_LogQueueMaxMessageLength = 64;

  /*! Logger modules */
  int m_LogModuleCount;

  struct
  {
    char Name[s_LogQueueSize];
    LogLevel_eT Level;
  } 
  m_LogModules[s_LogQueueSize];

  /*! Logger queue */
  int m_LogQueuePos;

  struct
  {
    int ID;
    LogLevel_eT Level;
    char Message[s_LogQueueMaxMessageLength];
  } 
  m_LogQueue[s_LogQueueSize];

  /**
   * @brief Constructor
   */
  Logger(void);

  /**
   * @brief Destructor
   */
  ~Logger();
  
  /*! Delete copy contructors/assignment operators to enforce singleton pattern */
  Logger(Logger &Other) = delete;
  void operator=(const Logger &) = delete;

};

#endif /* INC_LOGGER_H__ */