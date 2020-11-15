#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SCDE_s.h"

#include "SCDE.h"



/* helper
 * --------------------------------------------------------------------------------------------------
 *  FName: Format Time
 *  Desc: Creates formated time-text (uint8_t style - in this case zero terminated) from given
 *        time-stamp. Returned in msgText_t (text in allocated memory + length information)
 *  Note: DO NOT FORGET TO FREE MEMORY !! 
 *  Para: time_t tiSt -> the time-stamp that should be used
 *  Rets: strText_t -> formated time-text data
 * --------------------------------------------------------------------------------------------------
 */
strText_t
FmtTime(time_t tiSt)
{
  // our msg-text data packet
  strText_t strText;

  // get timeinfo for time-stamp
  struct tm timeinfo;
  localtime_r(&tiSt, &timeinfo);

  // prepare formated-time-string in allocated memory
  strText.strTextLen = asprintf((char**) &strText.strText
	,"%02d:%02d:%02d"
	,timeinfo.tm_hour
	,timeinfo.tm_min
	,timeinfo.tm_sec);

  return strText;
}
