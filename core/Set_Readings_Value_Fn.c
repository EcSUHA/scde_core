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
4608	setReadingsVal($$$$)
4609	{
4610	  my ($hash,$rname,$val,$ts) = @_;
4611	
4612	  if($hash->{".or"} && grep($rname =~ m/^$_$/, @{$hash->{".or"}}) ) {
4613	    if(defined($hash->{READINGS}{$rname}) && 
4614	       defined($hash->{READINGS}{$rname}{VAL}) &&
4615	        $hash->{READINGS}{$rname}{VAL} ne $val ) {
4616	      $hash->{OLDREADINGS}{$rname}{VAL} = $hash->{READINGS}{$rname}{VAL};
4617	      $hash->{OLDREADINGS}{$rname}{TIME} = $hash->{READINGS}{$rname}{TIME};
4618	    }
4619	  }
4620	
4621	  $hash->{READINGS}{$rname}{VAL} = $val;
4622	  $hash->{READINGS}{$rname}{TIME} = $ts;
4623	}
*/



/*
 * --------------------------------------------------------------------------------------------------
 *  FName: Set_Readings_Value_Fn
 *  Desc: Sets an reading with value and update time in given definition
 *  Para: Entry_Common_Definition_t *p_entry_common_definition
 *		  const string_t reading_name ->
 *		  const string_t reading_value ->
 *		  const time_t timestamp) ->
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
void
Set_Readings_Value_Fn(Entry_Common_Definition_t *p_entry_common_definition
		,const string_t reading_name
		,const string_t reading_value
		,const time_t timestamp)
{
  // Entry_Reading_t to process existing readings, start with first
  Entry_Reading_t *p_existing_entry_reading = 
      STAILQ_FIRST(&p_entry_common_definition->head_readings);

  // loop through the existing readings
  while (true) {

      // looped through the existing readings, but no existing + matching reading found?
	  // -> add new reading at tail of definitions readings SLTQ
      if (!p_existing_entry_reading) {

          // alloc mem for new reading entry
          Entry_Reading_t *p_new_entry_reading =
	          malloc(sizeof(Entry_Reading_t));

          // zero the struct
 //       memset(p_new_entry_reading, 0, sizeof(Entry_Reading_t));

          // fill reading with name
          p_new_entry_reading->reading.name.len =
	          asprintf((char **) &p_new_entry_reading->reading.name.p_char
	          ,"%.*s"
	          ,reading_name.len
	          ,reading_name.p_char);

          // fill reading with value
          p_new_entry_reading->reading.value.len =
	          asprintf((char **) &p_new_entry_reading->reading.value.p_char
	          ,"%.*s"
	          ,reading_value.len
	          ,reading_value.p_char);
	          
		  // add time (timestamp)
		  p_new_entry_reading->reading.time = timestamp;

          // add new reading to definitions readings SLTQ
	      STAILQ_INSERT_TAIL(&p_entry_common_definition->head_readings,
              p_new_entry_reading, entries);

				
	      // added new, done
	      break;
      }

      // is there an existing reading with matching name? Then replace old value. 
	  if ( (p_existing_entry_reading->reading.name.len == 
		    reading_name.len)
			&& (!strncmp((const char*) p_existing_entry_reading->reading.name.p_char,
				(const char*) reading_name.p_char,
				reading_name.len)) ) {

          // upd. existing reading, 1. free existing value
	      if (p_existing_entry_reading->reading.value.p_char) 
				free(p_existing_entry_reading->reading.value.p_char);

          // upd. existing reading, 2. fill reading with new value
          p_existing_entry_reading->reading.value.len =
	          asprintf((char **) &p_existing_entry_reading->reading.value.p_char
	          ,"%.*s"
	          ,reading_value.len
	          ,reading_value.p_char);

		  // set time (timestamp)
		  p_existing_entry_reading->reading.time = timestamp;

		  // updated old, done
		  break;
      }

    // get next existing reading for processing
    p_existing_entry_reading = STAILQ_NEXT(p_existing_entry_reading, entries);
  }

// -------------------------------------------------------------------------------------------------

  // list currently added readings stored for processing
  Entry_Reading_t *p_current_entry_reading;
  printf("SetReading_Fn done, summary of readings stored in definition:\n");
  STAILQ_FOREACH(p_current_entry_reading, &p_entry_common_definition->head_readings, entries) {
  
      string_t td_string = Get_Formated_Date_Time_Fn(p_current_entry_reading->reading.time);
	  printf("L  %.*s | %.*s = %.*s\n"
	      ,td_string.len
		  ,td_string.p_char
		  ,p_current_entry_reading->reading.name.len
		  ,p_current_entry_reading->reading.name.p_char
		  ,p_current_entry_reading->reading.value.len
		  ,p_current_entry_reading->reading.value.p_char);	
	  free(td_string.p_char);
  }

// -------------------------------------------------------------------------------------------------

  return;
}





