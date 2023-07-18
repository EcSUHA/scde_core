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





/*
 * Converts an integer value to its hex character (helper)
 */
char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}







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
 

/*
 * adds zero terminated query at given request-head buffer write position,
 * + removes leading '&' + inserts leading '?' !
 * + does uri encoding   + returns NO length, but zero terminates 
 */
void
Add_Query_Encoded(char *p_query_buf, char *p_req_head_buf)
{
  *p_req_head_buf++ = '?'; // lead-in of the query
  while (*p_query_buf) {
  
    if (isalnum(*p_query_buf) || *p_query_buf == '-' || *p_query_buf == '_' || *p_query_buf == '.' || *p_query_buf == '~') 

        *p_req_head_buf++ = *p_query_buf;

    else if (*p_query_buf == ' ') 

        *p_req_head_buf++ = '+';
        
    else *p_req_head_buf++ = '%', *p_req_head_buf++ = to_hex(*p_query_buf >> 4), *p_req_head_buf++ = to_hex(*p_query_buf & 15);

    p_query_buf++;
  }
  
  *p_req_head_buf = '\0';
  
  return;
}
		
 
 
 
