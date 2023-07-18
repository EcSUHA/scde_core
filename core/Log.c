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
  char attrkey_verbose[] = "verbose";
  string_t global_definition = {(uint8_t*) "global", 6};
  		        
  // attribute verbose assigned to 'global', to control global log-level?			        
  char *custom_log_level =
      Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn(&global_definition, &attrkey_verbose);
  
  // else start with default log-level 5
  int adjusted_log_level = 9;

  // analyze custom log-level string (from verbose attribute value)
  if (custom_log_level) adjusted_log_level = atoi(custom_log_level);

  // according to log-level: do we need to log it?
  if ( adjusted_log_level < log_level ) return;

  // ok, create log 

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



