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
#include <stdarg.h>

#include "SCDE_s.h"

#include "SCDE.h"



/* --------------------------------------------------------------------------------------------------
 *  FName: Log
 *  Desc: This is the logging function provided for the core functions with 4 infos:
 *        time-stamp, loglevel, core Fn, and the log-text elements
 *  Info: Level 0=System; 16=debug
 *  Para: const char *internal_fn -> the creator Fn of this log entry
 *        const uint8_t log_level -> the log-level of this log entry
 *        const char *format -> ptr to further elements used by the printf Fn
 *        ... -> further arguments
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
void
Log(const char *internal_fn
		,const uint8_t log_level
		,const char *format
		,...)
{
  // for current time
  time_t nowTist;

  // get current time
  time(&nowTist);

  // create and fill timeinfo struct
  struct tm timeinfo;
  localtime_r(&nowTist, &timeinfo);

  // time,loglevel,name
  printf("%d.%d.%d %d:%d:%d (%d) Core,Fn(%s): "	//Log|
		,timeinfo.tm_year+1900
		,timeinfo.tm_mon+1
		,timeinfo.tm_mday
		,timeinfo.tm_hour
		,timeinfo.tm_min
		,timeinfo.tm_sec
		,log_level
		,internal_fn);

  // the variable arguments
  va_list list;
  va_start(list, format);
  vprintf(format, list);
  va_end(list);

  // finalize line
  printf("\n");
}



