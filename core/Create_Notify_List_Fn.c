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
 *  FName: Create_Notify_List_Fn
 *  Desc: Creates the notify list (should be empty when calling!)
 *  Info: 
 *  Para: -/-
 *  Rets: -/-
 * -------------------------------------------------------------------------------------------------
 */
void
Create_Notify_List_Fn()
{
  if (!LIST_EMPTY(&SCDERoot.head_definitions_to_be_notified)) {
  
      #if CORE_SCDE_DBG >= 1
      Log("Create_Notify_List_Fn",1,
          "Error! Expected empty notify list.");
      #endif
      
      return;
  }


  // fill the notify list

  // search for all definitions that should be notified
  Entry_Common_Definition_t *p_entry_common_definition = 
      STAILQ_FIRST(&SCDERoot.head_definition);
  
  while (p_entry_common_definition != NULL) {

      // 'module' provides 'Notify_Fn' ?
	  if (p_entry_common_definition->module->provided->notify_fn) {

		  // alloc mem for new 'notify' entry (Entry_Notify_s)
		  entry_definition_to_be_notified_t *p_entry_definition_to_be_notified =
			  malloc(sizeof(entry_definition_to_be_notified_t));

		  // store definition that should be notified to entry
		  p_entry_definition_to_be_notified->p_entry_common_definition = 
		      p_entry_common_definition;
	
		  // insert new 'notify' entry to the notify list
		  LIST_INSERT_HEAD(&SCDERoot.head_definitions_to_be_notified,
		      p_entry_definition_to_be_notified, entries);
	  }

      // get next definition entry for processing
      p_entry_common_definition = STAILQ_NEXT(p_entry_common_definition, entries);
  }

//debug
  printf("\ncurrently added notifies:\n");
  // list currently added notifies
  entry_definition_to_be_notified_t *p_current_entry_notify;
  LIST_FOREACH(p_current_entry_notify, &SCDERoot.head_definitions_to_be_notified, entries) {

	printf("L notify:%.*s\n"
		,p_current_entry_notify->p_entry_common_definition->nname.len
		,p_current_entry_notify->p_entry_common_definition->nname.p_char);
  }



  return;
}



