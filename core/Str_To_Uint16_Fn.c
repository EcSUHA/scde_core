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
 

bool
Str_To_Uint16(const char *str, uint16_t *res)
{
  char *end;
  errno = 0;
  long val = strtol(str, &end, 10);
  if (errno || end == str || *end != '\0' || val < 0 || val >= 0x10000) {
      return false;
  }
  *res = (uint16_t)val;
  return true;
}

 
 
 
