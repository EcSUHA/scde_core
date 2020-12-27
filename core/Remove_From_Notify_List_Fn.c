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
 *  FName: Remove_From_Notify_List_Fn
 *  Desc: Removes definitions from the notify list. 
 *  Info: Faster then generating new list.
 *  Para: Entry_Definition_t *p_entry_common_definition -> definition that should be removed
 *  Rets: -/-
 * -------------------------------------------------------------------------------------------------
 */
void
Remove_From_Notify_List_Fn(Entry_Definition_t *p_entry_common_definition)
{

  return;
}



