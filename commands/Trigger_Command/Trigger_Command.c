/* #################################################################################################
 *
 *  Function: Trigger Command - for SCDE (Smart Connected Device Engine)
 *            Triggers ????? to Definitions
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

#include "Trigger_Command.h"


// -------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef Trigger_Command_DBG  
#define Trigger_Command_DBG  5	// 5 is default
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
 *  Desc: Data 'Provided By Command' for the Trigger Command (functions + infos this command provides
 *        to SCDE)
 *  Data: 
 * -------------------------------------------------------------------------------------------------
 */

// Command Help
const uint8_t Trigger_helpText[] = 
  "Usage: Trigger <def-spec> [<State-val>], to add Triggeributes to definitions";
// CommandHelp (detailed)
const uint8_t Trigger_helpDetailText[] = 
  "Usagebwrebwerb: Trigger <name> <type> <options>, to Define a device";


ProvidedByCommand_t Trigger_ProvidedByCommand = {
  "Trigger",					    // Command-Name of command -> libfilename.so !
  7,						        // length of cmd
  Trigger_Initialize_Command_Fn,	// Initialize Fn
  Trigger_Command_Fn,				// the Fn code
  { &Trigger_helpText, sizeof(Trigger_helpText) },
  { &Trigger_helpDetailText, sizeof(Trigger_helpDetailText) }
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
Trigger_Initialize_Command_Fn(SCDERoot_t* p_SCDERoot_from_core)
{
  // make data root locally available
  p_SCDERoot = p_SCDERoot_from_core;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  p_SCDEFn = p_SCDERoot->SCDEFn;

// --------------------------------------------------------------------------------------------------

  #if Trigger_Command_DBG >= 3
  p_SCDEFn->Log3Fn(Trigger_ProvidedByCommand.commandNameText
	,Trigger_ProvidedByCommand.commandNameTextLen
	,3
	,"InitializeFn called. Now useable.");
  #endif

// --------------------------------------------------------------------------------------------------

  return 0;
}


/*
 * Note: 'definition_name' is fixed, 'definition_spec' is an definition specification search argument !
 */


// Trigger kann nur definitionen triggern, keine Events!?!
// trigger <Device> <Reading>: <Value> geht auch
// trigger Stromzaehler power: 1000

/* -------------------------------------------------------------------------------------------------
 *  FName: Command_Fn
 *  Desc: Tries to add Attributes to definition-specification matching Definitions, with optional Value.
 *        Then calls modules TriggeributeFn with cmd=add, if retMsg.strText != NULL -> module sends veto.
 *  Info: 'definition_spec' is the definition specification, the input for the definition names
 *                   matching query. All matching definitions will get this Triggeribute assigned.
 *        'new_state_name' is the attribute name
 *        'new_state_value' is the OPTIONAL attribute value (NULL IF NO VALUE ASSIGNED!)
 *  Para: const String_t args -> space seperated command args string "definition_spec new_state_name attr_val"
 *  Rets: struct Head_String_s -> STAILQ head of multiple retMsg, if NULL -> no retMsg
 * -------------------------------------------------------------------------------------------------
 */
struct Head_String_s
Trigger_Command_Fn(const String_t args)
{
  #if Trigger_Command_DBG >= 7
  p_SCDEFn->Log3Fn(Trigger_ProvidedByCommand.commandNameText
	  ,Trigger_ProvidedByCommand.commandNameTextLen
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
  String_t new_state_name;

  // take *, it is at 'definition_spec' text start (seek-start-pos)
  new_state_name.p_char = definition_spec.p_char;

  // seek * to end of previous args text ('definition_spec') (to next '\32' -> space)
  while( ( i < args.len ) && ( *new_state_name.p_char != ' ' ) ) { i++, j++ ; new_state_name.p_char++ ; }

  // length of previous args text ('definition_spec') determined
  definition_spec.len = j;

  // seek * to start of 'new_state_name' text ('\32' after space)
  while( ( i < args.len ) && ( *new_state_name.p_char == ' ' ) ) { i++ ; new_state_name.p_char++ ; }

  // expected argument #3
  String_t new_state_value;
  
  // take *, it is at 'new_state_name' text start (seek-start-pos)
  new_state_value.p_char = new_state_name.p_char;

  // clear element seek-counter
  j = 0;

  // seek * to end of previous args text ('new_state_name') (to next '\32' -> space)
  while( ( i < args.len ) && ( *new_state_value.p_char != ' ' ) ) { i++ , j++ ; new_state_value.p_char++ ; }

  // length of previous args text ('new_state_name') determined
  new_state_name.len = j;

  // seek * to start of 'new_state_value' ('\32' after space)
  while( ( i < args.len ) && ( *new_state_value.p_char == ' ' ) ) { i++ ; new_state_value.p_char++ ; }

  // no further arguments expected - search for more args to generate error msg
  String_t more_args;
	
  // take *, it is at 'new_state_value' text start (seek-start-pos)
  more_args.p_char = new_state_value.p_char;
	
  // clear element seek-counter
  j = 0;

  // seek * to end of previous args text ('new_state_value') (to next '\32' -> space)
  while( ( i < args.len ) && ( *more_args.p_char != ' ' ) ) { i++ , j++ ; more_args.p_char++ ; }

  // length of previous args text ('new_state_value') determined
  new_state_value.len = j;
  
  // seek * to start of 'more_args' ('\32' after space)
  while( ( i < args.len ) && ( *more_args.p_char == ' ' ) ) { i++ ; more_args.p_char++ ; }

  // length of 'more_args' text determined (IN THIS CASE IT IS THE REST)
  more_args.len = args.len - i;
  
  // 'more_args' may end with spaces ...

// -------------------------------------------------------------------------------------------------

  // verify lengths > 0, ATTR_VAL_LEN = 0 ALLOWED! NO MORE ARGS!
  if ( ( definition_spec.len == 0 ) || ( more_args.len) ) {

	// alloc mem for retMsg
	Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	// response with error text
	p_entry_ret_msg->string.len = asprintf((char **)&p_entry_ret_msg->string.p_char,
		"Error! Could not interpret '%.*s'! Usage: Trigger <def-spec> [<new-state> [<new-value>]]",
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

	// to store the ret_msg from this loops Notify_Fn call
	Entry_String_t* p_entry_ret_msg = NULL;

    // start empty
    Entry_Reading_t *p_changed_state = NULL;
    
    if (new_state_name.len) {
   
        // alloc mem for new reading entry
        p_changed_state =
	        malloc(sizeof(Entry_Reading_t));

        // zero the struct
        memset(p_changed_state, 0, sizeof(Entry_Reading_t));
    
        // fill reading with name
        p_changed_state->reading.name.len =
	        asprintf((char **) &p_changed_state->reading.name.p_char
	        ,"%.*s"
	        ,new_state_name.len
	        ,new_state_name.p_char);
	        
        if (new_state_value.len) {
        
            // fill reading with value
            p_changed_state->reading.value.len =
	            asprintf((char **) &p_changed_state->reading.value.p_char
	            ,"%.*s"
	            ,new_state_value.len
	            ,new_state_value.p_char);
	    }   
    }

    // call 'module' 'Notify_Fn', if ret_msg != NULL -> an entry, interpret as veto
    p_entry_ret_msg =
        p_SCDEFn->Do_Trigger_Fn(p_entry_common_definition, p_changed_state);





 // alle entry müssen dealloct werden!



















/*
	// call 'module' 'Notify_Fn' for this 'definition' to notify - if provided
	if (p_entry_common_definition->module->provided->notify_fn) {

		// build 'attr_command' -> add
		String_t attr_command;
		attr_command.len = asprintf((char **) &attr_command.p_char
			,"add");

		#if Trigger_Command_DBG >= 7
		p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
			Attr_ProvidedByCommand.commandNameTextLen,
			7,
			"Calling NotifyFN for definition '%.*s' (modul '%.*s') with new-state '%.*s'.",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
			new_state_name.len,
			new_state_name.p_char);
		#endif

		// call 'module' 'Notify_Fn', if ret_msg != NULL -> an entry, interpret as veto
		p_entry_ret_msg =
			p_entry_common_definition->module->provided->notify_fn(p_entry_common_definition,
			attr_command,
			new_state_name,
			new_state_value);

		// free 'attr_command'
		free(attr_command.p_char);
	}

	else {
		#if Attr_Command_DBG >= 7
		p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
			Attr_ProvidedByCommand.commandNameTextLen,
			7,
			"No AttributeFN available for definition '%.*s' (modul '%.*s') "
			" new_state_name '%.*s' with new_state_value '%.*s' will be assigned without check.",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
  			new_state_name.len,
			new_state_name.p_char,
			new_state_value.len,
			new_state_value.p_char);
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
			"Attribute new_state_name '%.*s' with new_state_value '%.*s' NOT created!",
			p_entry_common_definition->nameLen,
			p_entry_common_definition->name,
			p_entry_common_definition->module->provided->typeNameLen,
			p_entry_common_definition->module->provided->typeName,
  			new_state_name.len,
			new_state_name.p_char,
			new_state_value.len,
			new_state_value.p_char);
		#endif

		// load next entry of 'definition_spec' matching definitions
		p_entry_def_spec_matching_definitions = 
			STAILQ_NEXT(p_entry_def_spec_matching_definitions, entries);

		// to continue loop without adding attributes
		continue;
	}

// -------------------------------------------------------------------------------------------------

	// start with 'no entry' for 'new_state_value' -> may reach search loop - with matching entry
	Entry_new_state_value_t* p_entry_new_state_value = NULL;

	// get first entry from our 'new_state_name' list
  	Entry_new_state_name_t* p_entry_new_state_name = 
		LIST_FIRST(&p_SCDERoot->head_new_state_name);

  	// search the lists 'new_state_name' entries for the requested 'new_state_name'
 	while ( p_entry_new_state_name != NULL ) {

		// compare no + length of characters. Matching list entry with requested 'new_state_name' ?
		if ( ( p_entry_new_state_name->new_state_name.len == new_state_name.len )
			&& ( !strncasecmp((const char*) p_entry_new_state_name->new_state_name.p_char,
				(const char*) new_state_name.p_char,
				new_state_name.len) ) ) {

			// found, keep 'new_state_name' entry to process it
			break;
		}

		// load next 'new_state_name' entry to process it
		p_entry_new_state_name = LIST_NEXT(p_entry_new_state_name, entries);
 	 }

// -------------------------------------------------------------------------------------------------

	// no entry for requested 'new_state_name' found ?
  	if ( p_entry_new_state_name == NULL ) {

		// alloc mem for new 'new_state_name' entry (Entry_new_state_name_s)
		p_entry_new_state_name
			= malloc(sizeof(Entry_new_state_name_t));

		// store 'new_state_name'.len to entry
		p_entry_new_state_name->new_state_name.len = new_state_name.len;

		// store 'new_state_name' to entry
		p_entry_new_state_name->new_state_name.p_char = malloc(new_state_name.len);
		memcpy(p_entry_new_state_name->new_state_name.p_char, new_state_name.p_char, new_state_name.len);
	
		// insert new 'new_state_name' entry to the new_state_name list
		LIST_INSERT_HEAD(&p_SCDERoot->head_new_state_name, p_entry_new_state_name, entries);

	}

// -------------------------------------------------------------------------------------------------

	else {
		// get first entry from our 'new_state_value' list
  		p_entry_new_state_value = LIST_FIRST(&p_entry_common_definition->head_new_state_value);

  		// search this 'definition' list of assigned 'new_state_value' entries
 		while ( p_entry_new_state_value != NULL ) {

			// check, if this entry 'new_state_value' is for requested entry 'new_state_name'
			if ( p_entry_new_state_name == p_entry_new_state_value->p_entry_new_state_name ) {

				// found assigned 'new_state_value', keep, break loop!
				break;
			}
		}

		// load next 'new_state_value' entry to process it
		p_entry_new_state_value = LIST_NEXT(p_entry_new_state_value, entries);
 	 }

// -------------------------------------------------------------------------------------------------

	// no entry for requested 'new_state_value' ?
  	if ( p_entry_new_state_value == NULL ) {

		// alloc mem for new 'new_state_value' entry (Entry_new_state_value_s)
		p_entry_new_state_value
			= malloc(sizeof(Entry_new_state_value_t));

	}

// -------------------------------------------------------------------------------------------------

	else {
		// temporary remove 'new_state_value' entry from the new_state_value list
		LIST_REMOVE(p_entry_new_state_value, entries);

		// free old value from entry
		if (p_entry_new_state_value->new_state_value.p_char) 
			free(p_entry_new_state_value->new_state_value.p_char);
	}

// -------------------------------------------------------------------------------------------------

	// now p_entry_new_state_value is ready for write / update

	// store the assignment of the 'new_state_name' to this  entry 'new_state_value'
	p_entry_new_state_value->p_entry_new_state_name = p_entry_new_state_name;

	// store the 'new_state_value'.len
	p_entry_new_state_value->new_state_value.len = new_state_value.len;

	// may be without value ! Length 0!
	if (new_state_value.len) {

		p_entry_new_state_value->new_state_value.p_char = malloc(new_state_value.len);
		memcpy(p_entry_new_state_value->new_state_value.p_char, new_state_value.p_char, new_state_value.len);
	}

	// add the 'new_state_value' entry to the new_state_value list
	LIST_INSERT_HEAD(&p_entry_common_definition->head_new_state_value, p_entry_new_state_value, entries);

	#if Attr_Command_DBG >= 7
	p_SCDEFn->Log3Fn(Attr_ProvidedByCommand.commandNameText,
		Attr_ProvidedByCommand.commandNameTextLen,
		7,
		"new_state_name '%.*s' with new_state_value '%.*s' assigned to definition '%.*s' "
		"(modul '%.*s').",
		p_entry_new_state_name->new_state_name.len,
		p_entry_new_state_name->new_state_name.p_char,
		p_entry_new_state_value->new_state_value.len,
		p_entry_new_state_value->new_state_value.p_char,
		p_entry_common_definition->nameLen,
		p_entry_common_definition->name,
		p_entry_common_definition->module->provided->typeNameLen,
		p_entry_common_definition->module->provided->typeName);
	#endif

// -------------------------------------------------------------------------------------------------
*/
	// get next entry of definition_spec matching definitions
	p_entry_def_spec_matching_definitions = 
		STAILQ_NEXT(p_entry_def_spec_matching_definitions, entries);
  }

  // return STAILQ head, stores multiple ret_msg, if NULL -> no ret_msg-entries
  return head_ret_msg;
}



