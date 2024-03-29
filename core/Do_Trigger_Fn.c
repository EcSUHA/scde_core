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
#include <string.h>

#include "SCDE_s.h"

#include "SCDE.h"


/* -------------------------------------------------------------------------------------------------
 *  FName: Do_Trigger_Fn
 *  Desc:  
 *  Info: 
 *  Para: Entry_Definition_t *p_entry_common_definition -> definition for which notifies should be 
 *                                                         triggered
 *        char *p_changed_state_value_as_text -> updated state value, as text, or NULL if none, will
 *                                               be updated and triggert,too
 *  Rets: Entry_String_t * -> SLTQ head, stores multiple strings, 
 *                                       the definition CMD line and attribute CMD lines, NULL=NONE
 * -------------------------------------------------------------------------------------------------
 */
Entry_String_t *
Do_Trigger_Fn(Entry_Common_Definition_t *p_entry_common_definition,
    char *p_changed_state_value_as_text)
{
  // one return message ?
  Entry_String_t *p_entry_ret_msg = NULL;
  
// -------------------------------------------------------------------------------------------------

  #if CORE_SCDE_DBG >= 7
  if (p_changed_state_value_as_text) {
   
      Log("Do_Trigger_Fn",7,
          "Got definition '%.*s' to trigger, including changed 'state' value '%s'."
          ,p_entry_common_definition->nameLen
	      ,p_entry_common_definition->name
          ,p_changed_state_value_as_text);
  }
  
  else {
      Log("Do_Trigger_Fn",7,
          "Got definition '%.*s' to trigger."
         ,p_entry_common_definition->nameLen
	     ,p_entry_common_definition->name);
  }
  #endif

// -------------------------------------------------------------------------------------------------

//??  $hash->{".triggerUsed"} = 1 if(defined($hash->{".triggerUsed"}));

// -------------------------------------------------------------------------------------------------

  // got an changed state value ? -> take value amd push it to the definitions changed_readings queue
  if (p_changed_state_value_as_text) {

//    Get_Reading_From_Definition_by_Text() //call!

      // identify state reading, if any
 	  entry_reading2_t *p_entry_state_reading;
      STAILQ_FOREACH(p_entry_state_reading, &p_entry_common_definition->head_readings2, entries) {
      
      	  if ( (p_entry_state_reading->reading.name.len == 5) &&
          (!strncasecmp((const char*) p_entry_state_reading->reading.name.p_char,
		  "state", 5)) ) {
		  
		      break;
		  }
      }
      
      // update the state-reading value
      p_entry_state_reading->reading.p_reading_type->p_Store_Raw_Reading_From_Text(&p_entry_state_reading->reading,
          p_changed_state_value_as_text);

      // found the state reading ?
      if (p_entry_state_reading) {

          // NO changed readings? -> initialize changed_readings first
          if (!p_entry_common_definition->p_changed) {
      
              // alloc mem for changed structure (changed_t)
              changed_t *p_changed =
	              (changed_t *) malloc(sizeof(changed_t));

              // zero the changed_readings structure (Changed_Readings_t)
//            memset(p_changed, 0, sizeof(Changed_Readings_t));

              // changed_readings - single-linked-tail-queue that stores the readings - init head
              STAILQ_INIT(&p_changed->head_notifies);
          
              // mark timestamp as not initialized
              p_changed->update_timestamp = 0;
          
              // store the changed_readings structure
              p_entry_common_definition->p_changed =
                  p_changed;     
          }
          
          // add the state changed event
          Add_Event_Fn(p_entry_common_definition ,&p_entry_state_reading->reading);
      }        
  }
  
  // no changed state or readings? -> return
  if (!p_entry_common_definition->p_changed) {
  
      return p_entry_ret_msg;
  }

// -------------------------------------------------------------------------------------------------
 
  // get current time
//  time_t now = GetUniqueTiSt(); // die quelle kann nicht bleiben!!!

  // have max? + not in inner trigger loop ?
  if ( (1) && (!(p_entry_common_definition->Common_CtrlRegA & (F_IN_TRIGGER))) ) {

      // now we are entering inner loop -> set Flag F_IN_TRIGGER
      p_entry_common_definition->Common_CtrlRegA |= F_IN_TRIGGER;

      // create notify list, if not already maintained
      if (LIST_EMPTY(&SCDERoot.head_definitions_to_be_notified)) Create_Notify_List_Fn();

 	  // call the Notify_Fn for all definitions in the to-notify list  
 	  entry_definition_to_be_notified_t *p_current_entry_definition_to_be_notified;
      LIST_FOREACH(p_current_entry_definition_to_be_notified, &SCDERoot.head_definitions_to_be_notified, entries) {
 	  
 	      // get from the current list entry: the definition entry to process
          Entry_Common_Definition_t *p_notified_entry_common_definition =
              p_current_entry_definition_to_be_notified->p_entry_common_definition;

          // check that the definition entry to process is still in definitions SLTQ, else break!
          Entry_Common_Definition_t *p_current_entry_common_definition; 
          STAILQ_FOREACH(p_current_entry_common_definition, &SCDERoot.head_definition, entries) {

              if (p_current_entry_common_definition == p_notified_entry_common_definition) break;
          }

          // an entry = still existing? / NULL = was deleted in a previous notify
          if (!p_current_entry_common_definition) break;

          // to store the ret_msg from Notify_Fn
	      Entry_String_t* p_entry_ret_msg = NULL;

	      // call 'module's 'Notify_Fn' for this 'definition' to notify - if provided by Module
	      if (p_notified_entry_common_definition->module->provided->notify_fn) {

		      // call 'module's 'Notify_Fn', collect ret_msg
		      p_entry_ret_msg =
			      p_notified_entry_common_definition->module->provided->notify_fn(p_entry_common_definition,
			          p_notified_entry_common_definition);
	      }
      }
  }

//delete($hash->{NTFY_TRIGGERTIME});

  #if CORE_SCDE_DBG >= 5
  Log("Do_Trigger_Fn",5,
      "End notify loop for '%.*s'."
      ,p_entry_common_definition->nameLen
	  ,p_entry_common_definition->name);
  #endif






//debug
 /*
  // list currently added readings stored for processing
  entry_notify_t *p_current_entry_notify;
  STAILQ_FOREACH(p_current_entry_notify, &p_entry_common_definition->p_changed->head_readings, entries) {

	printf("L readingName:%.*s, readingValue:%.*s\n"
		,p_current_entry_notify->reading.name.len
		,p_current_entry_notify->reading.name.p_char
		,p_current_entry_notify->reading.value.len
		,p_current_entry_notify->reading.value.p_char);

	if (p_current_entry_notify->reading.name.p_char) 
	    free(p_current_entry_notify->reading.name.p_char);
    if (p_current_entry_notify->reading.value.p_char) 
	    free(p_current_entry_notify->reading.value.p_char);
	    
  }
*/
 
 
 
 
// -------------------------------------------------------------------------------------------------

  string_t td_string = 
      Get_Formated_Date_Time_Fn(p_entry_common_definition->p_changed->update_timestamp); 
  printf("Do_Trigger_Fn done, common timestamp '%.*s', notifies in definition will be deleted:\n",
      td_string.len, td_string.p_char);
  free(td_string.p_char);
   
  // list currently added readings stored for processing
  entry_notify_t *p_listed_entry_notify;
  STAILQ_FOREACH(p_listed_entry_notify, &p_entry_common_definition->p_changed->head_notifies, entries) {
  	
      string_t td_string = Get_Formated_Date_Time_Fn(p_listed_entry_notify->notify.reading->timestamp);	
      string_t value_as_text = 
          p_listed_entry_notify->notify.reading->p_reading_type->p_get_raw_reading_as_text_fn(p_listed_entry_notify->notify.reading);				

	  printf("L  %.*s | %.*s = %.*s\n"
	      ,td_string.len
		  ,td_string.p_char
		  ,p_listed_entry_notify->notify.reading->name.len
		  ,p_listed_entry_notify->notify.reading->name.p_char
		  ,value_as_text.len
          ,value_as_text.p_char);
  
	  free(value_as_text.p_char);
	  free(td_string.p_char);
  }

// -------------------------------------------------------------------------------------------------

  // delete currently added notifies from notify list
  entry_notify_t *p_current_entry_notify = 
      STAILQ_FIRST(&p_entry_common_definition->p_changed->head_notifies);
  while (p_current_entry_notify != NULL) {

      entry_notify_t *p_last_entry_notify =
          p_current_entry_notify;

      // get next Common_Definition for processing
	  p_current_entry_notify = STAILQ_NEXT(p_current_entry_notify, entries);
		
	  free(p_last_entry_notify);
  }

  // finally the 'changed' information stuct, including marking it as not initialized = NULL
  free(p_entry_common_definition->p_changed);
  p_entry_common_definition->p_changed = NULL;
 
// -------------------------------------------------------------------------------------------------

 
 
 
 
 
 

















/*
3723	  if($max && !defined($hash->{INTRIGGER})) {
3724	    $hash->{INTRIGGER}=1;
3725	    if($attr{global}{verbose} >= 5) {
3726	      Log 5, "Starting notify loop for $dev, " . scalar(@{$hash->{CHANGED}}) . 
3727	        " event(s), first is " . escapeLogLine($hash->{CHANGED}->[0]);
3728	    }
3729	    createNtfyHash() if(!%ntfyHash);
3730	    $hash->{NTFY_TRIGGERTIME} = $now; # Optimize FileLog
3731	    my $ntfyLst = (defined($ntfyHash{$dev}) ? $ntfyHash{$dev} : $ntfyHash{"*"});
3732	    foreach my $n (@{$ntfyLst}) {
3733	      next if(!defined($defs{$n}));     # Was deleted in a previous notify
3734	      my $r = CallFn($n, "NotifyFn", $defs{$n}, $hash);
3735	      $ret .= " $n:$r" if($r);
3736	    }
3737	    delete($hash->{NTFY_TRIGGERTIME});
*/














  // now we are leaving inner loop -> clear Flag F_IN_TRIGGER
  p_entry_common_definition->Common_CtrlRegA &= ~(F_IN_TRIGGER);




 
  // return ptr to ret_msg as SLTQ entry, if STAILQ_EMPTY -> none
  return p_entry_ret_msg;
}



