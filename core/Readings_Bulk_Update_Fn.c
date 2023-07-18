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
 * --------------------------------------------------------------------------------------------------
 *  FName: Readings_Bulk_Update_Fn
 *  Desc: Call readingsBulkUpdate to update the reading.
 *        Example: readingsUpdate($hash,"temperature",$value);
 *  Para: entry_common_definition_t * ->  ptr to the common definition of the reading
 *       reading_t * -> ptr to the reading that was updated
 *       const bool ->  
 *       time_t -> optional, timestamp overrides
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
int
Readings_Bulk_Update_Fn(entry_common_definition_t *p_entry_common_definition,
    reading2_t *p_reading,
    const bool changed,
    time_t timestamp)
{
  // check if readings begin update was called
  if (!p_entry_common_definition->p_changed) {

      #if CORE_SCDE_DBG >= 1
      Log("Readings_Bulk_Update_Fn", 1, // <- Fn, log-level
          "Error! Readings_Bulk_Update_Fn called without calling Readings_Begin_Update_Fn "
          "first in Def-Name: '%.*s'."
		  ,p_entry_common_definition->name
		  ,p_entry_common_definition->nameLen);
      #endif

	  return 0;
  }

// -------------------------------------------------------------------------------------------------
//debug info...
  string_t value_as_text = 
      p_reading->p_reading_type->p_get_raw_reading_as_text_fn(p_reading);				

  printf("readingsBulkUpdate called for reading:'%.*s' value:'%.*s'\n"
  	,p_reading->name.len
  	,p_reading->name.p_char
  	,value_as_text.len
    ,value_as_text.p_char);
  	
  free(value_as_text.p_char);
 // -------------------------------------------------------------------------------------------------


  // default is use bulk update timestamp
  time_t readings_update_timestamp = p_entry_common_definition->p_changed->update_timestamp;
  
  // override timestamp ?
  if (timestamp) readings_update_timestamp = timestamp;
  
  // set the readings
//  Set_Readings_Value_Fn(p_entry_common_definition, reading_name, reading_value, readings_update_timestamp);
  p_reading->timestamp = readings_update_timestamp;

// ????

  // create notify for changes?
  if (changed) {
  
      Add_Event_Fn(p_entry_common_definition, p_reading);//, timestamp);
  }


/*
4968	  setReadingsVal($hash, $reading, $value, 
4969	                 $timestamp ? $timestamp : $hash->{".updateTimestamp"})
4970	        if($update_timestamp); 
4971	 
4972	  my $rv = "$reading: $value";
4973	  if($changed) {
4974	    if($reading eq "state") {
4975	      $rv = $value;
4976	      $hash->{CHANGEDWITHSTATE} = [];
4977	    }
4978	    addEvent($hash, $rv, $timestamp);
4979	  }
4980	  return $rv;
4981	}
4982	
*/
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
 	
  	
/*
  // alloc mem for new reading entry
  Entry_Reading_t *p_new_entry_reading
	  = malloc(sizeof(Entry_Reading_t));

  // zero the struct
  memset(p_new_entry_reading, 0, sizeof(Entry_Reading_t));

  // copy reading data
  p_new_entry_reading->reading.name.len = readingNameTextLen;
  p_new_entry_reading->reading.name.p_char = readingNameText;
  p_new_entry_reading->reading.value.len = readingValueTextLen;
  p_new_entry_reading->reading.value.p_char = readingValueText;

  // store the new reading in the definitions changed readings list
  STAILQ_INSERT_HEAD(&p_entry_common_definition->p_changed_readings->head_readings,
      p_new_entry_reading, entries);
      
// -------------------------------------------------------------------------------------------------

  // list currently added readings stored for processing
  Entry_Reading_t *p_current_entry_reading;
  STAILQ_FOREACH(p_current_entry_reading, &p_entry_common_definition->p_changed_readings->head_readings, entries) {

	printf("L readingName:%.*s, readingValue:%.*s\n"
		,p_current_entry_reading->reading.name.len
		,p_current_entry_reading->reading.name.p_char
		,p_current_entry_reading->reading.value.len
		,p_current_entry_reading->reading.value.p_char);
  }

// -------------------------------------------------------------------------------------------------
*/
  return 0;
}















/*Die Funktion readingsBulkUpdate() führt ein Update eines einzelnen Readings für die Definition $hash durch. Dabei wird das Readings $reading auf den Wert $value gesetzt. Bevor diese Funktion benutzt werden kann, muss readingsBeginUpdate() zuvor aufgerufen werden, ansonsten werden keine Updates durchgeführt.

changed optional
Flag, ob ein Event für dieses Update erzeugt werden soll (Wert: 1). Oder ob definitiv kein Event erzeugt werden soll (Wert: 0). Wenn nicht gesetzt, wird aufgrund entsprechender Attribute in der Definition von $hash entschieden, ob ein Event zu erzeugen ist, oder nicht (Attribute: event-on-change-reading, event-on-update-reading, event-min-interval, ...)



Rückgabewert $rv
Zeichenkette bestehend aus Reading und Wert getrennt durch einen Doppelpunkt, welcher anzeigt, auf welchen Wert das Reading tatsächlich gesetzt wurde.*/











