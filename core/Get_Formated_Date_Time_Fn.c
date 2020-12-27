#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SCDE_s.h"

#include "SCDE.h"



/* helper
 * --------------------------------------------------------------------------------------------------
 *  FName: Get_Formated_Date_Time
 *  Desc: Creates formated date-time-text (uint8_t style - in this case zero terminated) from given
 *        time-stamp. Returned in msgText_t (text in allocated memory + length information)
 *  Note: DO NOT FORGET TO FREE MEMORY !! 
 *  Para: time_t tiSt -> the time-stamp that should be used
 *  Rets: strText_t -> formated date-time-text data
 * --------------------------------------------------------------------------------------------------
 */
string_t
Get_Formated_Date_Time_Fn(time_t timestamp)
{
  // our text as result - don't forget to free!
  string_t text;

  // get timeinfo for timestamp
  struct tm timeinfo;
  localtime_r(&timestamp, &timeinfo);

  // prepare formated-time-string in allocated memory
  text.len = asprintf((char**) &text.p_char
	,"%04d-%02d-%02d %02d:%02d:%02d"
	,timeinfo.tm_year+1900
	,timeinfo.tm_mon+1
	,timeinfo.tm_mday
	,timeinfo.tm_hour
	,timeinfo.tm_min
	,timeinfo.tm_sec);

  return text;
}



