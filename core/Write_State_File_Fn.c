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
#include <stdio.h>

#include "SCDE_s.h"

#include "SCDE.h"


//setstate myBH1750 2021-1-6 15:17:3 BH1750:STATE state luminosity: 36.7 lx


/* --------------------------------------------------------------------------------------------------
 *  FName: WriteStatefile
 *  Desc: Writes the statefile to filesystem.
 *  Info: 
 *  Para: -/-
 *  Rets: struct headRetMsgMultiple_s -> head from STAILQ, stores multiple (all) readings
 *        from requested Definition, NULL=NONE
 * --------------------------------------------------------------------------------------------------
 */
struct headRetMsgMultiple_s
WriteStatefile()
{
  // prepare multiple RetMsg storage
  struct headRetMsgMultiple_s headRetMsgMultiple;

  // Initialize the queue (init STAILQ head)
  STAILQ_INIT(&headRetMsgMultiple);

  // get attribute "global->statefile" value
  String_t state_filename_attr_definition = {(uint8_t*) "global", 6};
  char state_filename_attr_name[] = "statefile";
 
  char *p_state_filename_attr_value =
		Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn(&state_filename_attr_definition,
			 &state_filename_attr_name);

	// attribute not found
	if (!p_state_filename_attr_value) {

		// alloc mem for retMsg
		strTextMultiple_t *retMsg =
			malloc(sizeof(strTextMultiple_t));

		// response with error text
		retMsg->strTextLen = asprintf(&retMsg->strText
			,"Error, Arribute %s not found in Definition %.*s !\r\n"
			,(char*) &state_filename_attr_name
			,state_filename_attr_definition.len
			,state_filename_attr_definition.p_char);

		// insert retMsg in stail-queue
		STAILQ_INSERT_TAIL(&headRetMsgMultiple, retMsg, entries);

		// return STAILQ head, stores multiple retMsg, if NULL -> no retMsg-entries
		return headRetMsgMultiple;
	}
/*
	// attribute found, but value not assigned
	if (!p_state_filename_attr_value->p_char) {
		
//		// dealloc from FN GetAttrValTextByDefTextAttrText
//		free(attrStateFNValueName);

		// alloc mem for retMsg
		strTextMultiple_t *retMsg =
			malloc(sizeof(strTextMultiple_t));

		// response with error text
		retMsg->strTextLen = asprintf(&retMsg->strText
			,"Error, arribute %.*s found in Definition %.*s, but no value is assigned !\r\n"
			,state_filename_attr_name.len
			,state_filename_attr_name.p_char
			,state_filename_attr_definition.len
			,state_filename_attr_definition.p_char);

		// insert retMsg in stail-queue
		STAILQ_INSERT_TAIL(&headRetMsgMultiple, retMsg, entries);

		// return STAILQ head, stores multiple retMsg, if NULL -> no retMsg-entries
		return headRetMsgMultiple;
	}
*/	
	// my $now = gettimeofday();
	time_t now = TimeNow();
			
//todo: my @t = localtime($now);

//todo: $stateFile = ResolveDateWildcards($stateFile, @t);

	// create statefilename string
	char *stateFile;
	asprintf(&stateFile
			,"/spiffs/%s.cfg"
			,p_state_filename_attr_value);
	 
//	// free attribute statefile value
//	free (attrStateFNValueName->strText);	 
//	free (attrStateFNValueName);
		
	// open statefile
	FILE* sFH = fopen(stateFile, "w");
	if (sFH == NULL) {

		// alloc mem for retMsg
		strTextMultiple_t *retMsg =
			malloc(sizeof(strTextMultiple_t));

		// response with error text
		retMsg->strTextLen = asprintf(&retMsg->strText
			,"Error, could not open $stateFile: %s!\r\n"
			,stateFile);

//   #Log 1, $errormsg; ???

		free(stateFile);

		// insert retMsg in stail-queue
		STAILQ_INSERT_TAIL(&headRetMsgMultiple, retMsg, entries);

		// return STAILQ head, stores multiple retMsg, if NULL -> no retMsg-entries
		return headRetMsgMultiple;
	}

  // free our prepared filename
  free(stateFile);	//Noch benötigt ? Vorher freigeben?, dann nicht doppelt


  // stores the time-text		
  struct tm timeinfo;

  // to fill with: "Sat Aug 19 14:16:59 2017"
  char strftime_buf[64];

/*
  // PREPARATIONS OF INTERNAL CLOCK
	localtime_r(&now, &timeinfo);

	// Set timezone to Eastern Standard Time and print local time
	setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
	tzset();
	// END OF PREPARATION
*/

  // get time to struct timeinfo
  localtime_r(&now, &timeinfo);

  // get strftime-text into strftime_buf 
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

  // start statefile with date:-> #Sat Aug 19 14:16:59 2017
  fprintf(sFH,"#%s\r\n", strftime_buf);


			





 







/*
	                                               ## $d ist der Name!!!
 # foreach my $d (sort keys %defs) {
 #   next if($defs{$d}{TEMPORARY});		//temporäre nicht!!
 #   if($defs{$d}{VOLATILE}) {
 #     my $def = $defs{$d}{DEF};
 #     $def =~ s/;/;;/g; # follow-on-for-timer at
 #     print SFH "define $d $defs{$d}{TYPE} $def\n";
 #   }
 #   my @arr = GetAllReadings($d);
 #   print SFH join("\n", @arr)."\n" if(@arr);
 # }
 # return "$attr{global}{statefile}: $!" if(!close(SFH));
 # return "";
		*/	


  // loop the definitions to process its readings
  Entry_Common_Definition_t *p_entry_common_definition;
  STAILQ_FOREACH(p_entry_common_definition, &SCDERoot.head_definition, entries) {
		
      // temporary definitions should not be processed
      if (p_entry_common_definition->defCtrlRegA & F_TEMPORARY) continue;

      // get STAILQ head with multiple readings_as_text
      struct head_string_s head_readings_as_text =
          Get_All_Readings_Fn(p_entry_common_definition);

	  // if queue not empty -> got readings for definition
	  if (!STAILQ_EMPTY(&head_readings_as_text)) {

		  // get further queue entries, till empty
		  while (!STAILQ_EMPTY(&head_readings_as_text)) {

			  // get a retMsg element from queue
			  Entry_String_t *p_reading_as_text =
			      STAILQ_FIRST(&head_readings_as_text);

			  // store setstate line
			  fprintf(sFH,"%.*s"
			      ,p_reading_as_text->string.len
				  ,p_reading_as_text->string.p_char);

			  // done, remove this entry
			  STAILQ_REMOVE_HEAD(&head_readings_as_text, entries);

              // cleanup text + entry
			  free(p_reading_as_text->string.p_char);
		      free(p_reading_as_text);
          }
      }
  }

  // close statefile
  fclose(sFH);


  // debug-> print file content
  printf("\ndbg content/spiffs/state.cfg:\n");
  int c;
  FILE *file;
  file = fopen("/spiffs/state.cfg", "r");
  if (file) {
    while ((c = getc(file)) != EOF)
      putchar(c);
    fclose(file);
  }
  printf("dbg end\n");
  // debug end


  // return STAILQ head, stores multiple retMsg with readings, if NULL -> none
  return headRetMsgMultiple;
}



