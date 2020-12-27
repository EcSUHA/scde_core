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
#include <strings.h>

#include "SCDE_s.h"

#include "SCDE.h"



/*
Die Funktion readingsBeginUpdate() bereitet die Definition mit dem Hash $hash auf ein Update von Readings vor.
 Dies betrifft insbesondere das Setzen von Umgebungsvariablen sowie dem aktuellen Zeitstempel als Änderungszeitpunkt.
  Der Aufruf dieser Funktion ist notwendig um eigentliche Updates mit der Funktion readingsBulkUpdate() auf der gewünschten 
  Definition durchführen zu können.*/

	
	
	
/*
 * --------------------------------------------------------------------------------------------------
 *  FName: Readings_Begin_Update_Fn
 *  Desc: Call Readings_Begin_Update_Fn before you start updating Readings. The updated Readings
 *        will all get the same timestamp, which is the time when you called this subroutine.
 *  Para: Entry_Common_Definition_t *p_entry_common_definition -> the definition which wants to update readings
 *  Rets: time_t update_time -> the assigned update time, 0 = error
 * --------------------------------------------------------------------------------------------------
 */
time_t
Readings_Begin_Update_Fn(Entry_Common_Definition_t *p_entry_common_definition)
{
  // check if bulk update is already called / started
  if (p_entry_common_definition->p_changed) {

      #if CORE_SCDE_DBG >= 1
      Log("Readings_Begin_Update_Fn", 1, // <- Fn, log-level
          "Error! Update for definition '%.*s' is already started. "
	      "Can not begin new update."
		  ,p_entry_common_definition->name
		  ,p_entry_common_definition->nameLen);
      #endif

	  return 0;
  }

  // alloc mem for changed structure (changed_t)
  changed_t * p_changed =
	  (changed_t *) malloc(sizeof(changed_t));

  // zero the changed_readings structure (changed_t)
  memset(p_changed, 0, sizeof(changed_t));

  // init the STAILQ that stores the notifies (init head)
  STAILQ_INIT(&p_changed->head_notifies);

  // get begin update timestamp
  time_t update_timestamp = GetUniqueTiSt(); // die quelle kann nicht bleiben!!!

  // store begin update timestamp
  p_changed->update_timestamp = update_timestamp;

  #if CORE_SCDE_DBG >= 7
  // prepare time for debug
  struct tm timeinfo;
  localtime_r(&p_changed->update_timestamp, &timeinfo);

  Log("Readings_Begin_Update_Fn", 7, // <- Fn, log-level
      "Readings_Begin_Update_Fn called for definition '%.*s'. "
	  "Readings will get TiSt '%d.%d.%d, %d:%d:%d'."
	  ,p_entry_common_definition->nameLen
	  ,p_entry_common_definition->name
	  ,timeinfo.tm_mday
	  ,timeinfo.tm_mon+1
	  ,timeinfo.tm_year+1900
	  ,timeinfo.tm_hour
	  ,timeinfo.tm_min
	  ,timeinfo.tm_sec);
  #endif

  // store the Changed_Readings_t
  p_entry_common_definition->p_changed = p_changed;

  return update_timestamp;
}



