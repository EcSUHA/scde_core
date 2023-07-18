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



/* --------------------------------------------------------------------------------------------------
 *  FName: Get_All_Readings_Fn
 *  Desc: Loop all readings stored in this definition and converts them to Setreading args-text,
 *       for recreation. The text is stored in a STAILQ. The Head is returned, else NULL.
 *       Includes STATE reading.
 *  Info: Fixed format:
 *        setstate myBH1750 BH1750:STATE 2021-1-6 15:17:3 state luminosity: 36.7 lx
 *        recreation-cmd Def-Name Type:Readingtype Date Clock ? readingname: 'rest is readingvalue'
 *  Para: Entry_Common_Definition_t *p_entry_common_definition -> ptr to definition which readings
 *                                   are requested as text
 *  Rets: struct head_string_s head_readings_as_text -> head from STAILQ, the prepared readings as
 *                                                      text, NULL=NONE
 * --------------------------------------------------------------------------------------------------
 */
struct head_string_s
Get_All_Readings_Fn(Entry_Common_Definition_t *p_entry_common_definition)
{
  // prepare STAILQ head for multiple readings_as_text storage
  struct head_string_s head_readings_as_text;

  // initialize the queue head
  STAILQ_INIT(&head_readings_as_text);

//---------------------------------------------------------------------------------------------------

  // first the STATE reading
 
  if ( (p_entry_common_definition->state_reading_value.p_char) && 
	   (p_entry_common_definition->state_reading_value.len) ) {
//	if(defined($val) &&
//     $val ne "unknown" &&
//     $val ne "Initialized" &&
//     $val ne "" &&
//     $val ne "???") {

		// alloc new p_entry_reading_as_text queue element
		entry_string_t *p_entry_reading_as_text =
			malloc(sizeof(entry_string_t));

		// write line to allocated memory and store to queue
		p_entry_reading_as_text->string.len = asprintf(&p_entry_reading_as_text->string.p_char
			,"setstate %.*s %.*s\r\n"
			,p_entry_common_definition->nname.len
			,p_entry_common_definition->nname.p_char
			,p_entry_common_definition->state_reading_value.len
			,p_entry_common_definition->state_reading_value.p_char);


        // display for debug
		printf(&p_entry_reading_as_text->string.p_char
			,"dbg 1.st row: setstate %.*s %.*s\r\n"
			,p_entry_common_definition->nname.len
			,p_entry_common_definition->nname.p_char
			,p_entry_common_definition->state_reading_value.len
			,p_entry_common_definition->state_reading_value.p_char);
        // dbg end
        

		// insert STATE reading as text in stail-queue
		STAILQ_INSERT_TAIL(&head_readings_as_text, p_entry_reading_as_text, entries);

  }

//---------------------------------------------------------------------------------------------------

  // now all other readings

  // loop the readings stored in this definition, for converting to text
  entry_reading2_t *p_current_entry_reading;
  STAILQ_FOREACH(p_current_entry_reading, &p_entry_common_definition->head_readings2, entries) {

      // set to current timestamp, if missing
      if (!p_current_entry_reading->reading.timestamp) {

	      //Log 4, "WriteStatefile $d $c: Missing TIME, using current time";

		  time(&p_current_entry_reading->reading.timestamp);
      }

/*		// set current value, if missing
		if (!p_current_entry_reading->readingTist) {

			//Log 4, "WriteStatefile $d $c: Missing VAL, setting it to 0";

			p_current_entry_reading->readingTist = ;

		}*/

      // get reading timestamp
	  struct tm timeinfo;
	  localtime_r(&p_current_entry_reading->reading.timestamp, &timeinfo);

      // get reading as text
      string_t value_as_text = 
          p_current_entry_reading->reading.p_reading_type->p_get_raw_reading_as_text_fn(&p_current_entry_reading->reading);				

	  // alloc new entry to store reading_as_text as queue element
	  entry_string_t *p_entry_reading_as_text =
	      malloc(sizeof(entry_string_t));

	  // write reading as text-line to allocated memory and store in queue
	  p_entry_reading_as_text->string.len = asprintf(&p_entry_reading_as_text->string.p_char
	      ,"setstate %.*s %.*s:%s %d-%d-%d %d:%d:%d %.*s %.*s\r\n"
	      ,p_entry_common_definition->nname.len
	      ,p_entry_common_definition->nname.p_char
			
		  ,p_current_entry_reading->reading.p_reading_type->p_provided_by_module->typeNameLen
		  ,(char *)&p_current_entry_reading->reading.p_reading_type->p_provided_by_module->typeName
		  ,p_current_entry_reading->reading.p_reading_type->template_name
		
		  ,timeinfo.tm_year+1900
		  ,timeinfo.tm_mon+1
		  ,timeinfo.tm_mday
		  ,timeinfo.tm_hour
		  ,timeinfo.tm_min
		  ,timeinfo.tm_sec
			
		  ,p_current_entry_reading->reading.name.len
		  ,p_current_entry_reading->reading.name.p_char
			
		   ,value_as_text.len
           ,value_as_text.p_char);

/*
      // display for debug
	  printf("dbg setstate %.*s %.*s:%s %d-%d-%d %d:%d:%d %.*s %.*s\r\n"
	      ,p_entry_common_definition->nname.len
		  ,p_entry_common_definition->nname.p_char
			
		  ,p_current_entry_reading->reading.p_entry_reading_type->p_provided_by_module->typeNameLen
		  ,(char *)&p_current_entry_reading->reading.p_entry_reading_type->p_provided_by_module->typeName
		  ,p_current_entry_reading->reading.p_entry_reading_type->template_name

		  ,timeinfo.tm_year+1900
		  ,timeinfo.tm_mon+1
		  ,timeinfo.tm_mday
		  ,timeinfo.tm_hour
		  ,timeinfo.tm_min
		  ,timeinfo.tm_sec
			
		  ,p_current_entry_reading->reading.name.len
		  ,p_current_entry_reading->reading.name.p_char
			
		  ,value_as_text.len
          ,(char *)value_as_text.p_char);
      // end
*/

      // free our reading as text
	  free(value_as_text.p_char);

	  // insert reading_as_text in stail-queue
	  STAILQ_INSERT_TAIL(&head_readings_as_text, p_entry_reading_as_text, entries);
	}

  // return STAILQ head, stores all readings of this definition as text. if STAILQ_EMPTY -> none
  return head_readings_as_text;
}



