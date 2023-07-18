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


/* --------------------------------------------------------------------------------------------------
 *  FName: Create_Reading
 *  Desc: Creates an new reading in calling definition
 *  Info: 
 *  Para: Entry_Common_Definition_t* p_entry_common_definition -> definition that maintains the new reading
 *        char *p_name -> name of the reading
 *        char *p_initial_value_as_text -> ptr to initial reading value as text (NULL = none/clr)
 *        char *p_initial_custom_unit_text -> ptr to custom unit text, overrides ..
 *        char *p_r_provided_by_module -> ptr to the name of the module that provides this reading type
 *        char *template_name -> reading template name in the specified module that provides the reading
 *        (DO WE NEED INITIAL TIMESTAMP? OR ALWAYS ZERO? int timestamp -> initial timestamp
 *  Rets: struct Head_String_s -> SLTQ head, stores multiple strings, 
 *                                       the definition CMD line and attribute CMD lines, NULL=NONE
 * --------------------------------------------------------------------------------------------------
 */
reading2_t *
Create_Reading_Fn(entry_common_definition_t *p_entry_common_definition, char *p_name,
    char *p_initial_value_as_text, char *p_initial_custom_unit_text, char *p_r_provided_by_module, char *template_name)
{
  // find the module that provides the reading
  Entry_Module_t *p_entry_module =
      GetLoadedModulePtrByName((uint8_t *)p_r_provided_by_module,
          strlen(p_r_provided_by_module));

  // module not found? -> can not create
  if (!p_entry_module) return NULL;

  // no reading-types provided? -> can not create
  if (!p_entry_module->provided->p_reading_types) return NULL;
  
  // make accessable ..
  reading_type_t (*p_array_reading_types) [1] =
      (reading_type_t (*) [1]) p_entry_module->provided->p_reading_types;
  
  int i; 
  // loop through list to find reading-type by template-name
  for (i = 0 ; p_array_reading_types[i]->template_name ; i++ ) {

      if (!strcmp(p_array_reading_types[i]->template_name, template_name)) break;
  }
  
  // NULL = EOL ? reading template not found -> can not create
  if (!p_array_reading_types[i]->template_name) return NULL;

  // store reading-type of matching template
  reading_type_t *p_reading_type =
      &p_array_reading_types[i];
      
  // prepare reading name, given or template name ?
  char *p_reading_name;
  if (p_name) p_reading_name = p_name;
  else p_reading_name = p_reading_type->template_name;
 
 /* 
  // is reading is already assigned -> can not create
  if (!Get_Reading_Ptr_By_Definition_And_Name(p_entry_common_definition,
      p_reading_name) return NULL;
 */
  
  // create the reading
  entry_reading2_t *p_entry_reading =
      malloc(sizeof(entry_reading2_t));

  // assign the reading type
  p_entry_reading->reading.p_reading_type = 
      p_reading_type;

  // an the reading name
  p_entry_reading->reading.name.len = 
      asprintf((char **) &p_entry_reading->reading.name.p_char, "%s",
          p_reading_name);
          
  // mark as empty reading, nothing to free     
  p_entry_reading->reading.raw_data.p_char = NULL;

  // do we have an initial value as text? take it!
  if (p_initial_value_as_text) {
  
      p_entry_reading->reading.p_reading_type->p_Store_Raw_Reading_From_Text(&p_entry_reading->reading,
          p_initial_value_as_text);
  }
 
  // do we have an initial custom unit text? take it!
  if (p_initial_custom_unit_text) {
  
      asprintf((char **) &p_entry_reading->reading.unit, "%s",
          p_initial_custom_unit_text);
       
  // else use unit from template
  } else {
  
      asprintf((char **) &p_entry_reading->reading.unit, "%s",
          p_reading_type->unit);
  }
   
  // assign timestamp 0
  p_entry_reading->reading.timestamp = 0;

  // and add reading to definitions readings queue
  STAILQ_INSERT_TAIL(&p_entry_common_definition->head_readings2,
      p_entry_reading, entries);

  // return for processing or link storage
  return &p_entry_reading->reading;
}
 


