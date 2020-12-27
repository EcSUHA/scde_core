#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

#include "SCDE_s.h"

#include "SCDE.h"



/* --------------------------------------------------------------------------------------------------
 *  FName: Get_Ptr_To_Definition_By_Name_Fn
 *  Desc: Returns the ptr to the definition entry with the given definition name
 *  Para: const String_t definition_name -> the definition name 
 *  Rets: Entry_Common_Definition_t * -> Pointer to definition entry / NULL if not found
 * --------------------------------------------------------------------------------------------------
 */
Entry_Common_Definition_t *
Get_Ptr_To_Definition_By_Name_Fn(const String_t definition_name)
{
  Entry_Common_Definition_t *p_entry_common_definition;

  STAILQ_FOREACH(p_entry_common_definition, &SCDERoot.HeadCommon_Definitions, entries) {

	  if ( (p_entry_common_definition->nname.len == definition_name.len) &&
          (!strncasecmp((const char*) p_entry_common_definition->nname.p_char,
		  (const char*) definition_name.p_char, definition_name.len)) ) {

		  return p_entry_common_definition;
      }
  }

  return NULL;
}



