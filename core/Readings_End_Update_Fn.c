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





/*http://sancho.ccd.uniroma2.it/cgi-bin/man/man2html?STAILQ_INIT+3
 * --------------------------------------------------------------------------------------------------
 *  FName: readingsEndUpdate
 *  Desc: Call readingsEndUpdate when you are done updating readings. This optionally calls DoTrigger
 *        to propagate the changes.
 *  Para: Entry_Common_Definition_t *p_entry_common_definition -> the Definition that requests updated readings
 *        bool doTrigger -> triggering readings ? false / true
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
/*Die Funktion readingsEndUpdate() beendet den Bulk-Update Prozess durch die Funktionen readingsBeginUpdate() & readingsBulkUpdate() und triggert optional die entsprechenden Events sämtlicher erzeugter Readings für die Definition $hash. Desweiteren werden nachgelagerte Tasks wie bspw. die Erzeugung von User-Readings (Attribut: userReadings), sowie die Erzeugung des STATE aufgrund des Attributs stateFormat durchgeführt. Sofern $do_trigger gesetzt ist, werden alle anstehenden Events nach Abschluss getriggert.*/
int
Readings_End_Update_Fn(Entry_Common_Definition_t *p_entry_common_definition,
	bool do_trigger)
{
  // check if bulk update begin was called
  if (!p_entry_common_definition->p_changed) {

      #if CORE_SCDE_DBG >= 1
      Log("Readings_End_Update_Fn", 1, // <- Fn, log-level
          "Error! Readings_End_Update_Fn called without calling Readings_Begin_Update_Fn "
          "first in Def-Name: '%.*s'."
		  ,p_entry_common_definition->nname.p_char
		  ,p_entry_common_definition->nname.len);
      #endif
      
	  return 0;
  }
/*
  // loop through the bulk-update-readings - start with first entry
  Entry_Reading_t *p_current_updated_entry_reading = 
	  STAILQ_FIRST(&p_entry_common_definition->p_changed_readings->head_readings);

  while (p_current_updated_entry_reading != NULL) {

	  // set common tist to BU reading element
	  p_current_updated_entry_reading->reading.time =
	      p_entry_common_definition->p_changed_readings->update_time;

/	LOGD("Now proc. readingName:%.*s, readingValue:%.*s\n"
		,p_current_updated_entry_reading->nameString.length
		,p_current_updated_entry_reading->nameString.characters
		,p_current_updated_entry_reading->valueString.length
		,p_current_updated_entry_reading->valueString.characters);/

	  // Entry_Reading_t to loop trough the existing readings
	  Entry_Reading_t *p_existing_entry_reading = 
		  STAILQ_FIRST(&p_entry_common_definition->head_readings);

	  // loop through the existing readings
	  while (true) {

	      // looped through the existing readings, but no existing,matching reading found?
	      // Add updated reading at tail of definitions readings SLTQ
		  if (!p_existing_entry_reading) {

			// remove "current_updated_entry_reading" from changed_readings SLTQ
			STAILQ_REMOVE(&p_entry_common_definition->p_changed_readings->head_readings,
				p_current_updated_entry_reading, Entry_Reading_s, entries);

			// add "current_updated_entry_reading" to definitions readings SLTQ
			STAILQ_INSERT_TAIL(&p_entry_common_definition->head_readings,
				p_current_updated_entry_reading, entries);

/			LOGD("Added new reading - readingName:%.*s, readingValue:%.*s\n"
				,p_current_updated_entry_reading->nameString.length
				,p_current_updated_entry_reading->nameString.characters
				,p_current_updated_entry_reading->valueString.length
				,p_current_updated_entry_reading->valueString.characters);/

			// added new, break
			break;
		}

		// is there an existing reading name that matches? Then replace old value. 
		if ( (p_existing_entry_reading->reading.name.len == 
		    p_current_updated_entry_reading->reading.name.len)
			&& (!strncmp((const char*) p_existing_entry_reading->reading.name.p_char,
				(const char*) p_current_updated_entry_reading->reading.name.p_char,
				p_current_updated_entry_reading->reading.name.len)) ) {

	        // upd. existing reading, 1. free existing value
			if (p_existing_entry_reading->reading.value.p_char) 
				free(p_existing_entry_reading->reading.value.p_char);

			// upd. existing reading, 2. take updated value
			p_existing_entry_reading->reading.value = 
				p_current_updated_entry_reading->reading.value;

			// upd. existing reading, 3. free current name (we do not need it)
			if (p_current_updated_entry_reading->reading.name.p_char) 
				free(p_current_updated_entry_reading->reading.name.p_char);

			// upd. existing reading, 4. take the time (update_time)
			p_existing_entry_reading->reading.time =
				p_entry_common_definition->p_changed_readings->update_time;

			// remove current_updated_entry_reading from changed readings SLTQ
			STAILQ_REMOVE(&p_entry_common_definition->p_changed_readings->head_readings,
				p_current_updated_entry_reading, Entry_Reading_s, entries);

			// we have removed the entry - free current current_updated_reading SLTQE
			free(p_current_updated_entry_reading);

/			LOGD("Updated old reading - readingName:%.*s, readingValue:%.*s\n"
				,p_existing_entry_reading->nameString.length
				,p_existing_entry_reading->nameString.characters
				,p_existing_entry_reading->valueString.length
				,p_existing_entry_reading->valueString.characters);/

			// found, break
			break;
		}

		// get next existing reading for processing
		p_existing_entry_reading = STAILQ_NEXT(p_existing_entry_reading, entries);

	}

	  // continue loop - get next changed reading
	  p_current_updated_entry_reading = 
		  STAILQ_FIRST(&p_entry_common_definition->p_changed_readings->head_readings);
  }

  // finnished, dealloc changed readings, and mark as not started
  free(p_entry_common_definition->p_changed_readings);
  p_entry_common_definition->p_changed_readings = NULL;

*/










/*printf("readingsEndUpdate finnished. Current readings for this definiton:\n");

  // list readings stored for definition after processing
//  xReadingSLTQE_t *p_current_updated_entry_reading;
  STAILQ_FOREACH(p_current_updated_entry_reading, &p_entry_common_definition->headReadings, entries) {

	LOGD("L readingName:%.*s, readingValue:%.*s\n"
		,p_current_updated_entry_reading->nameString.length
		,p_current_updated_entry_reading->nameString.characters
		,p_current_updated_entry_reading->valueString.length
		,p_current_updated_entry_reading->valueString.characters);
  }*/


  //sucht das state reading in den definitions-readings und packt es formatiert in den/das STATE!
  // Evaluate the state-reading embedded in this definition
  Eval_State_Format_Fn(p_entry_common_definition);

  // propagate changes (changed_readings)
  if ( (do_trigger) && (SCDERoot.global_control_register_a & F_INIT_DONE)) {

      if (!(SCDERoot.global_control_register_a & F_READINGS_UPDATE_DELAY_TRIGGER)) 
          Do_Trigger_Fn(p_entry_common_definition, NULL);
  } 
  
  // do NOT propagate changes, clean up - but only if not executing in trigger
  else if (!(p_entry_common_definition->Common_CtrlRegA & (F_IN_TRIGGER)) ) {

//--- debug txt
      printf("do NOT propagate the following notifies: (clean up!)\n");
      entry_notify_t *p_displayed_entry_notify = 
          STAILQ_FIRST(&p_entry_common_definition->p_changed->head_notifies);
      while (p_displayed_entry_notify != NULL) {

	      string_t td_string = Get_Formated_Date_Time_Fn(p_displayed_entry_notify->notify.reading->timestamp);	
          string_t value_as_text = 
              p_displayed_entry_notify->notify.reading->p_reading_type->p_get_raw_reading_as_text_fn(p_displayed_entry_notify->notify.reading);				

	      printf("L  %.*s | %.*s = %.*s\n"
	         ,td_string.len
		      ,td_string.p_char
		      ,p_displayed_entry_notify->notify.reading->name.len
		      ,p_displayed_entry_notify->notify.reading->name.p_char
		      ,value_as_text.len
              ,value_as_text.p_char);
  
	      free(value_as_text.p_char);
	      free(td_string.p_char);

		  p_displayed_entry_notify = STAILQ_NEXT(p_displayed_entry_notify, entries);
       }
//--- 
 
      // delete all currently added notifies - no processing! 
      entry_notify_t *p_current_entry_notify = 
          STAILQ_FIRST(&p_entry_common_definition->p_changed->head_notifies);
      while (p_current_entry_notify != NULL) {

/*	      if (p_current_entry_notify->notify.name.p_char) 
	          free(p_current_entry_notify->notify.name.p_char);
          if (p_current_entry_notify->notify.value.p_char) 
	          free(p_current_entry_notify->notify.value.p_char);
*/
          entry_notify_t *p_last_entry_notify =
              p_current_entry_notify;

		  // get next Common_Definition for processing
		  p_current_entry_notify = STAILQ_NEXT(p_current_entry_notify, entries);
		
		  free(p_last_entry_notify);
      }

      // delete the changed_readings struct, too + mark as not in use = NULL
      free(p_entry_common_definition->p_changed);
      p_entry_common_definition->p_changed = NULL;
  }

  return 0;
}




/*
bool dotrigger
flag init done

  // propagate changes (changed_readings)
4809	  if($dotrigger && $init_done) {
4810	    DoTrigger($name, undef, 0) if(!$readingsUpdateDelayTrigger);
4811	  } else {
4812	    if(!defined($hash->{INTRIGGER})) {
4813	      delete($hash->{CHANGED});
4814	      delete($hash->{CHANGEDWITHSTATE})
4815	    }
4816	  }

*/



















