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
4625	sub
4626	addEvent($$;$)
4627	{
4628	  my ($hash,$event,$timestamp) = @_;
4629	  push(@{$hash->{CHANGED}}, $event);
4630	  if($timestamp) {
4631	    $hash->{CHANGETIME} = [] if(!defined($hash->{CHANGETIME}));
4632	    $hash->{CHANGETIME}->[@{$hash->{CHANGED}}-1] = $timestamp;
4633	  }
4634	}
4635	
*/



/*
 * --------------------------------------------------------------------------------------------------
 *  FName: Add_Event_Fn
 *  Desc: Adds an event as entry_event_t to the changed-readings List
 *  Para: Entry_Common_Definition_t *p_entry_common_definition
 *		  const string_t notify_name ->
 *		  const string_t notify_value ->
 *		  const time_t update_time) ->
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
void
Add_Event_Fn(Entry_Common_Definition_t *p_entry_common_definition
		,reading2_t *reading)
//		,const time_t timestamp)
{
  // assign update_timestamp, only if not already done  *** SHOULD BE ALWAYS THERE ???***
//  if ( (!p_entry_common_definition->p_changed->update_timestamp) && (timestamp) ) 
//      p_entry_common_definition->p_changed->update_timestamp = timestamp;

  // entry_notify_t to process existing readings, start with first
  entry_notify_t *p_existing_entry_notify = 
      STAILQ_FIRST(&p_entry_common_definition->p_changed->head_notifies);

  // loop through the existing notifies. May be it exists already
  while (true) {

      // looped through the existing notifies, but no existing + matching notify found?
	  // -> add new notify at tail of definitions notifies SLTQ
      if (!p_existing_entry_notify) {

          // alloc mem for new notify entry
          entry_notify_t *p_new_entry_notify =
	          malloc(sizeof(entry_notify_t));

          // zero the struct
//        memset(p_new_entry_notify, 0, sizeof(entry_notify_t));

/*          // fill reading with name
          p_new_entry_notify->notify.name.len =
	          asprintf((char **) &p_new_entry_notify->notify.name.p_char
	          ,"%.*s"
	          ,notify_name.len
	          ,notify_name.p_char);

          // fill reading with value
          p_new_entry_notify->notify.value.len =
	          asprintf((char **) &p_new_entry_notify->notify.value.p_char
	          ,"%.*s"
	          ,notify_value.len
	          ,notify_value.p_char);

		  // set time (timestamp), if any
		  if (timestamp) p_new_entry_notify->notify.timestamp = timestamp;
		      else p_new_entry_notify->notify.timestamp = 0;
*/

          // insert the reading that notifies into the notify
          p_new_entry_notify->notify.reading = reading;

          // add new reading to definitions readings SLTQ
	      STAILQ_INSERT_TAIL(&p_entry_common_definition->p_changed->head_notifies,
              p_new_entry_notify, entries);
				
	      // added new, done
	      break;
      }

      // is there an existing reading with matching name? Then ?. 
	  if ( p_existing_entry_notify->notify.reading == reading ) {
/*
          // upd. existing reading, 1. free existing value
	      if (p_existing_entry_notify->notify.value.p_char) 
				free(p_existing_entry_notify->notify.value.p_char);

          // upd. existing reading, 2. fill reading with new value
          p_existing_entry_notify->notify.value.len =
          
	          asprintf((char **) &p_existing_entry_notify->notify.value.p_char
	          ,"%.*s"
	          ,notify_value.len
	          ,notify_value.p_char);

		  // set time (timestamp), if any
		  if (timestamp) p_existing_entry_notify->notify.timestamp = timestamp;
		      else p_existing_entry_notify->notify.timestamp = 0;
*/
		  // updated old, done
		  break;
      }

    // get next existing reading for processing
    p_existing_entry_notify = STAILQ_NEXT(p_existing_entry_notify, entries);
  }

// -------------------------------------------------------------------------------------------------

  string_t td_string = 
      Get_Formated_Date_Time_Fn(p_entry_common_definition->p_changed->update_timestamp);
  printf("Add_Event_Fn done, common timestamp '%.*s', notifys stored in definition:\n",
      td_string.len, td_string.p_char);
  free(td_string.p_char);
   
  // list currently added notifies stored for processing
  entry_notify_t *p_current_entry_notify;
  STAILQ_FOREACH(p_current_entry_notify, &p_entry_common_definition->p_changed->head_notifies, entries) {
	  
	  string_t td_string = Get_Formated_Date_Time_Fn(p_current_entry_notify->notify.reading->timestamp);	
      string_t value_as_text = 
          p_current_entry_notify->notify.reading->p_reading_type->p_get_raw_reading_as_text_fn(p_current_entry_notify->notify.reading);				

	  printf("L  %.*s | %.*s = %.*s\n"
	      ,td_string.len
		  ,td_string.p_char
		  ,p_current_entry_notify->notify.reading->name.len
		  ,p_current_entry_notify->notify.reading->name.p_char
		  ,value_as_text.len
          ,value_as_text.p_char);
  
	  free(value_as_text.p_char);
	  free(td_string.p_char);
  }

// -------------------------------------------------------------------------------------------------

  return;
}



