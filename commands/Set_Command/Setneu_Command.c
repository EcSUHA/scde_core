/* #################################################################################################
 *
 *  Function: Set Command - for SCDE (Smart Connected Device Engine)
 *            Sets the Definitions
 *
 *  ESP 8266EX & ESP32 SOC Activities ...
 *  Copyright by EcSUHA
 *
 *  Created by Maik Schulze, Sandfuhren 4, 38448 Wolfsburg, Germany for EcSUHA.de 
 *
 *  MSchulze780@GMAIL.COM
 *  EcSUHA - ECONOMIC SURVEILLANCE AND HOME AUTOMATION - WWW.EcSUHA.DE
 * #################################################################################################
 */



#include <ProjectConfig.h>

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#include <Platform.h>
#endif // ESP_PLATFORM

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SCDE_s.h>

#include "Set_Command.h"


// -------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef Set_Command_DBG  
#define Set_Command_DBG  5	// 5 is default
#endif

// -------------------------------------------------------------------------------------------------

// make data root locally available
static SCDERoot_t* p_SCDERoot;

// make locally available from data-root: the SCDEFn (Functions / callbacks) for operation
static SCDEFn_t* p_SCDEFn;

// -------------------------------------------------------------------------------------------------



/*
 * -------------------------------------------------------------------------------------------------
 *  DName: ProvidedByCommand
 *  Desc: Data 'Provided By Command' for the Set Command (functions + infos this command provides
 *        to SCDE)
 *  Data: 
 * -------------------------------------------------------------------------------------------------
 */

// Command Help
const uint8_t Set_helpText[] = 
  "Usage: Set <def-spec> <Type dependent arguments or ?>, to set the definitions";
// CommandHelp (detailed)
const uint8_t Set_helpDetailText[] = 
  "Usagebwrebwerb: Set <def-spec> <Type dependent arguments or ?>, to set the definitions";


ProvidedByCommand_t Set_ProvidedByCommand = {
  "Set",					    // Command-Name of command -> libfilename.so !
  3,						    // length of cmd
  Set_Initialize_Command,	    // Initialize Fn
  Set_Command,				    // the Fn code
  { &Set_helpText, sizeof(Set_helpText) },
  { &Set_helpDetailText, sizeof(Set_helpDetailText) }
};



/* --------------------------------------------------------------------------------------------------
 *  FName: Initialize_Command_Fn
 *  Desc: Initializion of an (new loaded) SCDE-Command. Init p_SCDERoot and p_SCDE Function Callbacks.
 *  Info: Called only once befor use!
 *  Para: SCDERoot_t* p_SCDERoot_from_core -> ptr to SCDE Data Root from SCDE-Core
 *  Rets: ? unused
 *--------------------------------------------------------------------------------------------------
 */
int
Set_Initialize_Command(SCDERoot_t* p_SCDERoot_from_core)
{
  // make data root locally available
  p_SCDERoot = p_SCDERoot_from_core;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  p_SCDEFn = p_SCDERoot->SCDEFn;

// --------------------------------------------------------------------------------------------------

  #if Set_Command_DBG >= 3
  p_SCDEFn->Log3Fn(Set_ProvidedByCommand.commandNameText
	,Set_ProvidedByCommand.commandNameTextLen
	,3
	,"InitializeFn called. Now useable.");
  #endif

// --------------------------------------------------------------------------------------------------

  return 0;
}


/*
 * Note: 'definition_name' is fixed, 'definition_spec' is an definition specification search argument !
 */

/*
Device specification (devspec)

The commands attr, deleteattr, displayattr, delete, get, list, set, setreading, setstate, trigger can take a more complex device specification as argument, which will be expanded to a list of devices. A device specification (short devspec) can be:
a single device name. This is the most common case.
a list of devices, separated by comma (,)
a regular expression
a NAME=VALUE pair, where NAME can be an internal value like TYPE, a Reading-Name or an attribute. VALUE is a regexp. To negate the comparison, use NAME!=VALUE. To restrict the search, use i: as prefix for internal values, r: for readings and a: for attributes. See the example below.
if the spec is followed by the expression :FILTER=NAME=VALUE, then the values found in the first round are filtered by the second expression.
Examples:
set lamp1 on
set lamp1,lamp2,lamp3 on
set lamp.* on
set room=kitchen off
set room=kitchen:FILTER=STATE=on off
set room=kitchen:FILTER=STATE!=off off
*/



/* -------------------------------------------------------------------------------------------------
 *  FName: Command_Fn
 *  Desc: Tries to add Attributes to definition-specification matching Definitions, with optional Value.
 *        Then calls modules AttributeFn with cmd=add, if retMsg.strText != NULL -> module sends veto.
 *  Info: 'definition_spec' is the definition specification, the input for the definition names
 *                   matching query. All matching definitions will get this attribute assigned.
 *        'attr_name' is the attribute name
 *        'attr_value' is the OPTIONAL attribute value (NULL IF NO VALUE ASSIGNED!)
 *  Para: const String_t args -> space seperated command args string "definition_spec attr_name attr_val"
 *  Rets: struct Head_String_s -> STAILQ head of multiple retMsg, if NULL -> no retMsg
 * -------------------------------------------------------------------------------------------------
 */
struct Head_String_s
Set_Command_Fn(const String_t args)
{
  #if Set_Command_DBG >= 7
  p_SCDEFn->Log3Fn(Set_ProvidedByCommand.commandNameText
	  ,Set_ProvidedByCommand.commandNameTextLen
	  ,7
	  ,"CommandFn called with args '%.*s'"
	  ,args.len
	  ,args.p_char);
  #endif

// --------------------------------------------------------------------------------------------------

  // prepare STAILQ head to store multiple 'ret_msg' elements
  struct Head_String_s head_ret_msg;

  // Initialize the queue
  STAILQ_INIT(&head_ret_msg);

// --------------------------------------------------------------------------------------------------

  // the total seek-counter
  int i = 0;

  // expected argument #1
  String_t definition_spec;

  // set * to start of possible 'definition_spec' text (seek-start-pos)
  definition_spec.p_char = args.p_char;
  
  // an element seek-counter
  int j = 0;
  
  // seek * to start of 'definition_spec' text ('\32' after space)
  while( ( i < args.len ) && ( *definition_spec.p_char == ' ' ) ) { i++ ; definition_spec.p_char++ ; }

  // expected argument #2
  String_t args;

  // take *, it is at 'definition_spec' text start (seek-start-pos)
  args.p_char = definition_spec.p_char;

  // seek * to end of previous args text ('definition_spec') (to next '\32' -> space)
  while( ( i < args.len ) && ( *args.p_char != ' ' ) ) { i++, j++ ; args.p_char++ ; }

  // length of previous args text ('definition_spec') determined
  definition_spec.len = j;

  // seek * to start of 'args' text ('\32' after space)
  while( ( i < args.len ) && ( *args.p_char == ' ' ) ) { i++ ; args.p_char++ ; }

  // expected argument #3
  String_t attr_value;
  
  // take *, it is at 'args' text start (seek-start-pos)
  attr_value.p_char = args.p_char;

  // clear element seek-counter
  j = 0;

  // seek * to end of previous args text ('args') (to next '\32' -> space)
  while( ( i < args.len ) && ( *attr_value.p_char != ' ' ) ) { i++ , j++ ; attr_value.p_char++ ; }

  // length of previous args text ('args') determined
  args.len = j;

  // seek * to start of 'attr_value' ('\32' after space)
  while( ( i < args.len ) && ( *attr_value.p_char == ' ' ) ) { i++ ; attr_value.p_char++ ; }

  // no further arguments expected - search for more args to generate error msg
  String_t more_args;
	
  // take *, it is at 'attr_value' text start (seek-start-pos)
  more_args.p_char = attr_value.p_char;
	
  // clear element seek-counter
  j = 0;

  // seek * to end of previous args text ('attr_value') (to next '\32' -> space)
  while( ( i < args.len ) && ( *more_args.p_char != ' ' ) ) { i++ , j++ ; more_args.p_char++ ; }

  // length of previous args text ('attr_value') determined
  attr_value.len = j;
  
  // seek * to start of 'more_args' ('\32' after space)
  while( ( i < args.len ) && ( *more_args.p_char == ' ' ) ) { i++ ; more_args.p_char++ ; }

  // length of 'more_args' text determined (IN THIS CASE IT IS THE REST)
  more_args.len = args.len - i;
  
  // 'more_args' may end with spaces ...

// -------------------------------------------------------------------------------------------------

  // verify lengths > 0, ATTR_VAL_LEN = 0 ALLOWED! NO MORE ARGS!
  if ( ( definition_spec.len == 0 ) || ( args.len == 0) || ( more_args.len) ) {

	// alloc mem for retMsg
	Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	// response with error text
	p_entry_ret_msg->string.len = asprintf((char **)&p_entry_ret_msg->string.p_char,
		"Error! Could not interpret '%.*s'! Usage: Attr <def-spec> <attr-name> [<attr-val>]",
		args.len,
		(char *)args.p_char);

	// insert ret_msg as entry in stail-queue
	STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	// return head of singly linked tail queue, which holds 'ret_msg' elements
    return 	head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

  // get all 'definition' that match 'definition_spec'. Result returned as SLTQ head.
  struct Head_Definition_Ptr_s head_def_spec_matching_definitions =
      p_SCDEFn->Get_Definitions_That_Match_DefSpec_String_Fn(definition_spec);

// -------------------------------------------------------------------------------------------------

  // start processing, load first returned entry 'definition'
  Entry_Definition_Ptr_t* p_entry_def_spec_matching_definitions = 
	  STAILQ_FIRST(&head_def_spec_matching_definitions);

  // no entry for definitions that match requested 'definition_spec' ? We should throw out an error!
  if ( p_entry_def_spec_matching_definitions == NULL ) {

	  // alloc mem for retMsg
	  Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf((char **)&p_entry_ret_msg->string.p_char,
		  "Could not find an <def-spec> matching definition! Check '%.*s'!",
		  definition_spec.len,
		  (char *)definition_spec.p_char);

	  // insert ret_msg as entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	  // return head of singly linked tail queue, which holds 'ret_msg' elements
  	  return head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

  // loop through the found 'definition' entrys - till the queue has no more entrys
  while ( p_entry_def_spec_matching_definitions != NULL ) {

// -------------------------------------------------------------------------------------------------

	// get the ptr to the current 'definition'
	Entry_Common_Definition_t* p_entry_common_definition = 
		p_entry_def_spec_matching_definitions->p_entry_common_definition;

	// to store the ret_msg from AttributeFn
	Entry_String_t* p_entry_ret_msg = NULL;

	// call 'module' 'Attribute_Fn' for this 'definition' to notify - if provided by Type
	if (p_entry_common_definition->module->provided->attribute_fn) {

		// build 'attr_command' -> add
		String_t attr_command;
		attr_command.len = asprintf((char **) &attr_command.p_char
			,"add");

		#if Attr_Command_DBG >= 7
		p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
			Attr_ProvidedByCommand.commandNameTextLen,
			7,
			"Calling AttributeFN for definition '%.*s' (modul '%.*s') with args "
			"attr_command '%.*s' attr_name '%.*s' attr_value '%.*s'",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
			attr_command.len,
			attr_command.p_char,
  			attr_name.len,
			attr_name.p_char,
			attr_value.len,
			attr_value.p_char);
		#endif

		// call 'module' 'Attribute_Fn', if ret_msg != NULL -> an entry, interpret as veto
		p_entry_ret_msg =
			p_entry_common_definition->module->provided->attribute_fn(p_entry_common_definition,
			attr_command,
			attr_name,
			attr_value);

		// free 'attr_command'
		free(attr_command.p_char);
	}

	else {
		#if Attr_Command_DBG >= 7
		p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
			Attr_ProvidedByCommand.commandNameTextLen,
			7,
			"No AttributeFN available for definition '%.*s' (modul '%.*s') "
			" attr_name '%.*s' with attr_value '%.*s' will be assigned without check.",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
  			attr_name.len,
			attr_name.p_char,
			attr_value.len,
			attr_value.p_char);
		#endif
	}

// -------------------------------------------------------------------------------------------------

	// 'ret_msg' as entry ? => veto from attributeFn! Add 'ret_msg' entry to queue, do NOT delete attribute !
	if (p_entry_ret_msg) {

		// insert ret_msg as entry in stail-queue
		STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

 		#if Attr_Command_DBG >= 7
		p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
			Attr_ProvidedByCommand.commandNameTextLen,
			7,
			"Got an veto msg from AttributeFN for definition '%.*s' (modul '%.*s'). "
			"Attribute attr_name '%.*s' with attr_value '%.*s' NOT created!",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
  			attr_name.len,
			attr_name.p_char,
			attr_value.len,
			attr_value.p_char);
		#endif

		// load next entry of 'definition_spec' matching definitions
		p_entry_def_spec_matching_definitions = 
			STAILQ_NEXT(p_entry_def_spec_matching_definitions, entries);

		// to continue loop without adding attributes
		continue;
	}

// -------------------------------------------------------------------------------------------------

	// start with 'no entry' for 'attr_value' -> may reach search loop - with matching entry
	entry_attr_value_t *p_entry_attr_value = NULL;

	// get first entry from our 'attr_name' list
  	entry_attr_name_t *p_entry_attr_name = 
		LIST_FIRST(&p_SCDERoot->head_attr_names);

  	// search the lists 'attr_name' entries for the requested 'attr_name'
 	while ( p_entry_attr_name != NULL ) {

		// compare no + length of characters. Matching list entry with requested 'attr_name' ?
		if ( ( strlen(p_entry_attr_name->p_attr_name) == attr_name.len )
			&& ( !strncasecmp((const char*) p_entry_attr_name->p_attr_name,
				(const char*) attr_name.p_char,
				attr_name.len) ) ) {

			// found, keep 'attr_name' entry to process it
			break;
		}

		// load next 'attr_name' entry to process it
		p_entry_attr_name = LIST_NEXT(p_entry_attr_name, entries);
 	 }

// -------------------------------------------------------------------------------------------------

	// no entry for requested 'attr_name' found ?
  	if ( p_entry_attr_name == NULL ) {

		// alloc mem for new 'attr_name' entry (entry_attr_name_s)
		p_entry_attr_name
			= malloc(sizeof(entry_attr_name_t));

		// store 'attr_name' to entry
		asprintf(&p_entry_attr_name->p_attr_name,
			"%.*s",
			attr_name.len,
			attr_name.p_char);

		// insert new 'attr_name' entry to the attr_name list
		LIST_INSERT_HEAD(&p_SCDERoot->head_attr_names, p_entry_attr_name, entries);
	}

// -------------------------------------------------------------------------------------------------

	else {
		// get first entry from our 'attr_value' list
  		p_entry_attr_value = LIST_FIRST(&p_entry_common_definition->head_attr_values);

  		// search this 'definition' list of assigned 'attr_value' entries
 		while ( p_entry_attr_value != NULL ) {

			// check, if this entry 'attr_value' is for requested entry 'attr_name'
			if ( p_entry_attr_name == p_entry_attr_value->p_entry_attr_name ) {

				// found assigned 'attr_value', keep, break loop!
				break;
			}
			
		    // load next 'attr_value' entry to process it
		    p_entry_attr_value = LIST_NEXT(p_entry_attr_value, entries);
		}
 	 }

// -------------------------------------------------------------------------------------------------

	// no entry for requested 'attr_value' ?
  	if ( p_entry_attr_value == NULL ) {

		// alloc mem for new 'attr_value' entry (entry_attr_value_s)
		p_entry_attr_value
			= malloc(sizeof(entry_attr_value_t));
	}

// -------------------------------------------------------------------------------------------------

	else {
		// temporary remove 'attr_value' entry from the attr_value list
		LIST_REMOVE(p_entry_attr_value, entries);

		// free old value from entry
		if (p_entry_attr_value->p_attr_value) 
			free(p_entry_attr_value->p_attr_value);
	}

// -------------------------------------------------------------------------------------------------

	// now p_entry_attr_value is ready for write / update

	// store the assignment of the 'attr_name' to this  entry 'attr_value'
	p_entry_attr_value->p_entry_attr_name = p_entry_attr_name;

    // store 'attr_value' to entry
	asprintf(&p_entry_attr_value->p_attr_value,
	    "%.*s",
		attr_value.len,
		attr_value.p_char);

	// add the 'attr_value' entry to the attr_value list
	LIST_INSERT_HEAD(&p_entry_common_definition->head_attr_values, p_entry_attr_value, entries);

	#if Attr_Command_DBG >= 7
	p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
		Attr_ProvidedByCommand.commandNameTextLen,
		7,
		"attr_name '%s' with attr_value '%s' assigned to definition '%.*s' "
		"(modul '%.*s').",
		p_entry_attr_name->p_attr_name,
		p_entry_attr_value->p_attr_value,
		p_entry_common_definition->nname.len,
		p_entry_common_definition->nname.p_char,
		p_entry_common_definition->module->provided->typeNameLen,
		p_entry_common_definition->module->provided->typeName);
	#endif

// -------------------------------------------------------------------------------------------------

	// get next entry of definition_spec matching definitions
	p_entry_def_spec_matching_definitions = 
		STAILQ_NEXT(p_entry_def_spec_matching_definitions, entries);
  }

  // return STAILQ head, stores multiple ret_msg, if NULL -> no ret_msg-entries
  return head_ret_msg;
}



