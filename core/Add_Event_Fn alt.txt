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
		,const string_t notify_name
		,const string_t notify_value
		,const time_t timestamp)
{
  // assign update_timestamp, only if not already done  *** SHOULD BE ALWAYS THERE ???***
  if ( (!p_entry_common_definition->p_changed->update_timestamp) && (timestamp) ) 
      p_entry_common_definition->p_changed->update_timestamp = timestamp;

  // entry_notify_t to process existing readings, start with first
  entry_notify_t *p_existing_entry_notify = 
      STAILQ_FIRST(&p_entry_common_definition->p_changed->head_notifies);

  // loop through the existing readings. May be it is existing already
  while (true) {

      // looped through the existing readings, but no existing + matching reading found?
	  // -> add new reading at tail of definitions readings SLTQ
      if (!p_existing_entry_notify) {

          // alloc mem for new reading entry
          entry_notify_t *p_new_entry_notify =
	          malloc(sizeof(entry_notify_t));

          // zero the struct
//        memset(p_new_entry_notify, 0, sizeof(entry_notify_t));

          // fill reading with name
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

          // add new reading to definitions readings SLTQ
	      STAILQ_INSERT_TAIL(&p_entry_common_definition->p_changed->head_notifies,
              p_new_entry_notify, entries);
				
	      // added new, done
	      break;
      }

      // is there an existing reading with matching name? Then replace old value. 
	  if ( (p_existing_entry_notify->notify.name.len == 
		    notify_name.len)
			&& (!strncmp((const char*) p_existing_entry_notify->notify.name.p_char,
				(const char*) notify_name.p_char,
				notify_name.len)) ) {

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
  	
      string_t td_string = Get_Formated_Date_Time_Fn(p_current_entry_notify->notify.timestamp);
	  printf("L  %.*s | %.*s = %.*s\n"
	      ,td_string.len
		  ,td_string.p_char
		  ,p_current_entry_notify->notify.name.len
		  ,p_current_entry_notify->notify.name.p_char
		  ,p_current_entry_notify->notify.value.len
		  ,p_current_entry_notify->notify.value.p_char);	
	  free(td_string.p_char);
  }

// -------------------------------------------------------------------------------------------------

  return;
}



