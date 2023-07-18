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
 *  FName: Get_Def_And_Attr
 *  Desc: Returns an SLTQ-head, links to the definition cmd-line and the attribute cmd-lines as
 *        entrys from the given 'entry_definition'
 *  Info: 
 *  Para: Entry_Common_Definition_t* p_entry_common_definition -> definition for which Def and Attr cmds are requested
 *  Rets: struct Head_String_s -> SLTQ head, stores multiple strings, 
 *                                       the definition CMD line and attribute CMD lines, NULL=NONE
 * --------------------------------------------------------------------------------------------------
 */
struct head_string_s
Get_Def_And_Attr_Fn(Entry_Common_Definition_t *p_entry_common_definition)
{
  // prepare STAILQ head that stores def_and_attr_text elements
  struct head_string_s head_def_and_attr_text;

  // initialize the queue head
  STAILQ_INIT(&head_def_and_attr_text);

//---------------------------------------------------------------------------------------------------

  // first get the define cmd-line, but skip global definition - its defined by SCDE on startup
	
  if (! //NOT! skip "global" definition!
	( (p_entry_common_definition->nameLen == 6) &&
	     ( strncasecmp((char *)p_entry_common_definition->name, "Global", 6) == 0 ) ) ) {
		
	// alloc new entry string, the retMsg
	entry_string_t* p_def_text =
		malloc(sizeof(entry_string_t));
		
	// check: are definition args stored ?
	if (p_entry_common_definition->definition.p_char) {
		
		// write define line with definition args and store it into entry
		p_def_text->string.len = asprintf(&p_def_text->string.p_char,
			"define %.*s %.*s %.*s\r\n",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
			p_entry_common_definition->definition.len,
			p_entry_common_definition->definition.p_char);
	}
		
	// no definition args stored ...
	else {

		// write define line without definition args and store it into entry
		p_def_text->string.len = asprintf(&p_def_text->string.p_char,
			"define %.*s %.*s\r\n",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName);
	}
		
	// insert entry string (the retMsg) into stail-queue
	STAILQ_INSERT_TAIL(&head_def_and_attr_text, p_def_text, entries);		
  }

//---------------------------------------------------------------------------------------------------

  // second the attribute(s)

  // loop the attributes stored for this definition for processing
  entry_attr_value_t *p_entry_attr_value;

  LIST_FOREACH(p_entry_attr_value, &p_entry_common_definition->head_attr_values, entries) {

	if (
		// is current entry the "global" Definition ? and
		( ( p_entry_common_definition->nameLen == 6 ) &&
		( strncasecmp((char *)p_entry_common_definition->name, "Global", 6) == 0 ) ) &&

		(

		// is the 'attr-name' "configfile"? or
		( strcasecmp(p_entry_attr_value->p_entry_attr_name->p_attr_name,
			 "configfile") == 0 ) ||

		// is the 'attr-name' "version"?
		( strcasecmp(p_entry_attr_value->p_entry_attr_name->p_attr_name,
			"version") == 0 )  
						)
							) {

		// when exec till here: current Attribute is "configfile" or "version"
		// skip, because its defined by SCDE on startup
	}

	// standard attribute, that should be stored ..
	else {
	
		// alloc new entry string, the retMsg
		entry_string_t *p_attr_text =
			malloc (sizeof(entry_string_t));

		// write attr-line to allocated memory and store to into entry
		p_attr_text->string.len = asprintf(&p_attr_text->string.p_char,
			"attr %.*s %s %s\r\n",
			p_entry_common_definition->nname.len,
			p_entry_common_definition->nname.p_char,
			p_entry_attr_value->p_entry_attr_name->p_attr_name,
			p_entry_attr_value->p_attr_value);

		// insert entry string (the retMsg) into stail-queue
		STAILQ_INSERT_TAIL(&head_def_and_attr_text, p_attr_text, entries);
	}
  }

  // return STAILQ head, stores multiple entries string (the retMsg), if STAILQ_EMPTY -> none
  return head_def_and_attr_text;
}



