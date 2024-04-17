
#include "rtc.h"
#include "logger.h"

#include <string.h>
#include <chip.h>
#include <Arduino.h>

const char *RealTimeClock::s_DayText[7] = {
#define X(Value, Text, Name) Text,
  DAY_MAP
#undef X
};

const char *RealTimeClock::s_MonthText[12] = {
#define X(Value, Text, Name, NumDays) Text,
  MONTH_MAP
#undef X
};

const int RealTimeClock::s_MonthNumDays[12] = {
#define X(Value, Text, Name, NumDays) NumDays,
  MONTH_MAP
#undef X
};

RealTimeClock::RealTimeClock(void) : m_LoggerID(LOGGER.Register("RTC"))
{
  LOG_LEVEL_INFO;

  /* Set 24hr mode */
  RTC->RTC_MR = ~RTC_MR_HRMOD;

  /* Stop RTC and wait until it can be written to */
  RTC->RTC_CR |= RTC_CR_UPDTIM | RTC_CR_UPDCAL;
  while (!(RTC->RTC_SR & RTC_SR_ACKUPD));
  RTC->RTC_SCCR = RTC_SCCR_ACKCLR;

  char CompilationDate[] = __DATE__;
  char CompilationTime[] = __TIME__;

  LOG_DEBUG("Initialising %s %s", CompilationDate, CompilationTime);

  /* Parse start from compilation time */
  RealTimeClock::Time_sT StartTime;

  int i;
  char TempBuff[8];

  /* Month */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationDate[0], 3);

  i = 0;
  while (i < 12)
  {
    if (!strcmp(s_MonthText[i], TempBuff))
    {
      StartTime.Month = (RealTimeClock::Month_eT)(i + 1);
      i = 0xFF;
    }
    else
    {
      i++;
    }
  }

  /* Date */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationDate[4], 2);

  StartTime.Date = atoi(TempBuff);

  /* Century */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationDate[7], 2);

  StartTime.Century = atoi(TempBuff);

  /* Year */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationDate[9], 2);

  StartTime.Year = atoi(TempBuff);

  /* Hour */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationTime[0], 2);

  StartTime.Hour = atoi(TempBuff);

  /* Minute */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationTime[3], 2);

  StartTime.Minute = atoi(TempBuff);

  /* Second */
  memset(&TempBuff[0], 0, sizeof(TempBuff));
  memcpy(&TempBuff[0], &CompilationTime[6], 2);

  StartTime.Second = atoi(TempBuff);

  /* Day */
  i = 0;
  for (int temp = 0; temp < (int)StartTime.Month - 1; temp++)
  {
    i += s_MonthNumDays[temp];
  }
  i += StartTime.Date;

  StartTime.Day = (RealTimeClock::Day_eT)((i % 7) - 1);

  Set(StartTime);
}

RealTimeClock::~RealTimeClock()
{
}

void RealTimeClock::Set(const RealTimeClock::Time_sT &StartTime)
{
  uint32_t InitialRegisterValue;
  
  /* Update Time fields (Hour, Minute, Second) */
  InitialRegisterValue = 0;
  InitialRegisterValue |= ((StartTime.Second % 10) | (StartTime.Second / 10 << 4)) << RTC_TIMR_SEC_Pos;
  InitialRegisterValue |= ((StartTime.Minute % 10) | (StartTime.Minute / 10 << 4)) << RTC_TIMR_MIN_Pos;
  InitialRegisterValue |= ((StartTime.Hour % 10) | (StartTime.Hour / 10 << 4)) << RTC_TIMR_HOUR_Pos;
  RTC->RTC_TIMR = InitialRegisterValue;
  LOG_DEBUG("Set date: %#08X", InitialRegisterValue);

  /* Update Calendar fields(century, Year, Month, date, day) */
  InitialRegisterValue = 0;
  InitialRegisterValue |= ((StartTime.Century % 10) | (StartTime.Century / 10 << 4)) << RTC_CALR_CENT_Pos;
  InitialRegisterValue |= ((StartTime.Year % 10) | (StartTime.Year / 10 << 4)) << RTC_CALR_YEAR_Pos;
  InitialRegisterValue |= ((((int)StartTime.Month + 1) % 10) | (((int)StartTime.Month + 1) / 10 << 4)) << RTC_CALR_MONTH_Pos;
  InitialRegisterValue |= ((((int)StartTime.Day + 1) % 10) | (((int)StartTime.Day + 1) / 10 << 4)) << RTC_CALR_DAY_Pos;
  InitialRegisterValue |= ((StartTime.Date % 10) | (StartTime.Date / 10 << 4)) << RTC_CALR_DATE_Pos;
  RTC->RTC_CALR = InitialRegisterValue;
  LOG_DEBUG("Set calender: %#08X", InitialRegisterValue);

  /* Restart RTC */
  RTC->RTC_CR &= ~(RTC_CR_UPDTIM | RTC_CR_UPDCAL);

  /* Check for RTC error */
  int Error = RTC->RTC_VER & 0xFF;
  if (Error)
  {
    LOG_ERROR("Invalid setup! %#02X", Error);
  }
  
  LOG_INFO("Set to %s", GetFormatted(StartTime));
}

char* RealTimeClock::GetFormatted(const RealTimeClock::Time_sT &Time)
{
  sprintf(m_FormattedTime,
          "%s %02d%02d/%02d/%02d %02d:%02d:%02d",
          s_DayText[(int)Time.Day],
          Time.Century, Time.Year, Time.Month, Time.Date,
          Time.Hour, Time.Minute, Time.Second
        );
  return m_FormattedTime;
}

RealTimeClock::Time_sT RealTimeClock::Get(void)
{
  Time_sT Time;

  int CalenderRegValue = RTC->RTC_CALR;
  int TimerRegValue = RTC->RTC_TIMR;

  int val;

  /* Century */
  val = (CalenderRegValue & RTC_CALR_CENT_Msk) >> RTC_CALR_CENT_Pos;
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Century = val;

  /* Year */
  val = (CalenderRegValue & RTC_CALR_YEAR_Msk) >> RTC_CALR_YEAR_Pos;
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Year = val;

  /* Month */
  val = (CalenderRegValue & RTC_CALR_MONTH_Msk) >> RTC_CALR_MONTH_Pos;
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Month = (RealTimeClock::Month_eT)(val - 1);

  /* Date */
  val = (CalenderRegValue & RTC_CALR_DATE_Msk) >> RTC_CALR_DATE_Pos;
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Date = val;

  /* Day */
  val = (CalenderRegValue & RTC_CALR_DAY_Msk) >> RTC_CALR_DAY_Pos;
  Time.Day = (RealTimeClock::Day_eT)(val - 1);

  /* Hour */
  val = (TimerRegValue & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos;
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Hour = val;

  /* Minute */
  val = (TimerRegValue & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos;
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Minute = val;

  /* Second */
  val = (TimerRegValue & RTC_TIMR_SEC_Msk);
  val = (val & 0x0F) + ((val >> 4) * 10);
  Time.Second = val;

  LOG_DEBUG(GetFormatted(Time));

  return Time;
}
