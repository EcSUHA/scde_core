/* #################################################################################################
 *
 *  Function: Delete Command - for SCDE (Smart Connected Device Engine)
 *            Deletes an Definition (which may be called Device)
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

#include "include/Delete_Command.h"


// -------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef Delete_Command_DBG  
#define Delete_Command_DBG  5	// 5 is default
#endif

// -------------------------------------------------------------------------------------------------

// make data root locally available
static SCDERoot_t* p_SCDERoot;

// make locally available from data-root: the SCDEFn (Functions / callbacks) for faster operation
static SCDEFn_t* p_SCDEFn;

// -------------------------------------------------------------------------------------------------



/* -------------------------------------------------------------------------------------------------
 *  DName: Delete_ProvidedByCommand
 *  Desc: Data 'Provided By Command' for this Command (functions + infos this command provides
 *        to SCDE)
 *  Data: 
 * -------------------------------------------------------------------------------------------------
 */
// Command Help
const uint8_t Delete_helpText[] = 
  "Usage: Delete <definition-name> [<type dependent arguments>], to delete a device";
// CommandHelp (detailed)
const uint8_t Delete_helpDetailText[] = 
  "Usagebwrebwerb: define <name> <type> <options>, to define a device";

ProvidedByCommand_t Delete_ProvidedByCommand = {
  "Delete",					        // Command-Name of command -> libfilename.so !
  6,						        // length of cmd
  Delete_InitializeCommand_Fn,		// Initialize Fn
  Delete_Command_Fn,				// the Fn code
  { &Delete_helpText, sizeof(Delete_helpText) },
  { &Delete_helpDetailText, sizeof(Delete_helpDetailText) }
};



/* -------------------------------------------------------------------------------------------------
 *  FName: Delete_InitializeCommand_Fn
 *  Desc: Initializion of an (new loaded) SCDE-Command. Init p_SCDERoot and p_SCDE Function Callbacks.
 *  Info: Called only once befor use!
 *  Para: SCDERoot_t* p_SCDERoot_from_core -> ptr to SCDE Data Root from SCDE-Core
 *  Rets: ? unused
 *--------------------------------------------------------------------------------------------------
 */
int
Delete_InitializeCommand_Fn(SCDERoot_t* p_SCDERoot_from_core)
{
  // make data root locally available
  p_SCDERoot = p_SCDERoot_from_core;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  p_SCDEFn = p_SCDERoot->SCDEFn;

// -------------------------------------------------------------------------------------------------

  #if Delete_Command_DBG >= 3
  p_SCDEFn->Log3Fn(Delete_ProvidedByCommand.commandNameText,
	  Delete_ProvidedByCommand.commandNameTextLen,
	  3,
	  "InitializeFn called. Now useable.");
  #endif

// -------------------------------------------------------------------------------------------------

  return 0;
}



/* -------------------------------------------------------------------------------------------------
 *  FName: Delete_Command_Fn
 *  Desc: Deletes (undefines) a Definition by calling Modules UndefineFn to do further module-specific
 *        deinitialization. Finally cleans up common values, including the definition.
 *  Info: 'Definition-Name' is custom Definition name. Allowed: [azAZ09._], uint8_t[32]
 *        'Definition-Args' is custom, stored in Definition->Definition, and passed to modules DefineFn
 *        ret Msg from UndefineFn results in Veto (keep Definition)
 *  Para: const String_t args -> space seperated command args text string,
 *                               normally "definition_name" // module_name definition_args"
 *  Rets: struct Head_String_s -> STAILQ head of queue filled with multiple ret_msg, 
 *                                if NULL -> no ret_msg
 * -------------------------------------------------------------------------------------------------
 */
struct Head_String_s
Delete_Command_Fn (const String_t args)
{
  #if Delete_Command_DBG >= 7
  p_SCDEFn->Log3Fn(Delete_ProvidedByCommand.commandNameText,
	  Delete_ProvidedByCommand.commandNameTextLen,
	  7,
	  "CommandFn called with args '%.*s'",
	  args.len,
	  args.p_char);
  #endif

// -------------------------------------------------------------------------------------------------

  // prepare STAILQ head to store multiple 'ret_msg' elements
  struct Head_String_s head_ret_msg;

  // Initialize the queue
  STAILQ_INIT(&head_ret_msg);

// -------------------------------------------------------------------------------------------------

  // the total seek-counter
  int i = 0;

  // expected argument #1
  String_t definition_name;

  // set * to start of possible 'definition_name' text (seek-start-pos)
  definition_name.p_char = args.p_char;
  
  // an element seek-counter
  int j = 0;
  
  // seek * to start of 'definition_name' text (skip '\32' -> space)
  while( ( i < args.len ) && ( *definition_name.p_char == ' ' ) ) { i++ ; definition_name.p_char++ ; }

  // expected argument #2
  String_t opt_args;

  // take * to start searching 'opt_args' text (seek-start-pos)
  opt_args.p_char = definition_name.p_char;

  // clear element seek-counter
  j = 0;

  // seek * to end of previous args text (to next '\32' -> space)
  while( ( i < args.len ) && ( *opt_args.p_char != ' ' ) ) { i++ , j++ ; opt_args.p_char++ ; }

  // length of 'module_name' text determined
  definition_name.len = j;

  // seek * to start of 'opt_args' (skip '\32' -> space)
  while( ( i < args.len ) && ( *opt_args.p_char == ' ' ) ) { i++ ; opt_args.p_char++ ; }
  
  // length of 'opt_args' text determined (its the rest)
  opt_args.len = args.len - i;

// -------------------------------------------------------------------------------------------------

  // verify lengths > 0, ! opt_args.len = 0 IS ALLOWED !
  if ( definition_name.len == 0 ) {

	  // alloc mem for ret_msg entry
	  Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		  "Error! Could not interpret '%.*s'! Usage: Define <definition-name> [<type dependent arguments>]",
		  args.len,
		  (char *)args.p_char);

	  // insert ret_msg entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	  // return head of singly linked tail queue, which holds 'ret_msg' elements
      return head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

  // get the Entry Common Definition by Name
  Entry_Common_Definition_t* p_entry_common_definition =
	STAILQ_FIRST(&p_SCDERoot->HeadCommon_Definitions);

  while (1) {

	// end of stored definition entries? Response with error -> name not found
	if (p_entry_common_definition == NULL) {

	  // alloc mem for ret_msg entry
	  Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		  "Error! Could not find definition '%.*s'.",
		  args.len,
		  (char *)args.p_char);

	  // insert ret_msg entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	  // return head of singly linked tail queue, which holds 'ret_msg' elements
      return head_ret_msg;
  }

	// entry with matching Definition Name ? -> break loop
	if ( (p_entry_common_definition->nname.len == definition_name.len)
		&& (!strncasecmp((const char *)p_entry_common_definition->nname.p_char,
		     (const char *)definition_name.p_char, definition_name.len)) ) {

		// found, break and keep prt
		break;
	}

	// get next Common_Definition for processing
	p_entry_common_definition = STAILQ_NEXT(p_entry_common_definition, entries);
  }

// -------------------------------------------------------------------------------------------------

  // UndefineFn NOT assigned by module ? 
  if (!p_entry_common_definition->module->provided->Undefine_Fn) {

	  // it is allowed to have no Undefine Fn !?
  }
  
  // Call UndefineFn assigned by module -> ret_msg forces VETO! (keep) 
  else	{
 
 	  #if Delete_Command_DBG >= 7
	  p_SCDEFn->Log3Fn(Delete_ProvidedByCommand.commandNameText,
		  Delete_ProvidedByCommand.commandNameTextLen,
		  7,
		  "Calling DeleteFn of Module '%.*s' to delete definition '%.*s' "
		  "with arguments '%.*s'.",
		  p_entry_common_definition->module->provided->typeNameLen,
		  p_entry_common_definition->module->provided->typeName,
		  p_entry_common_definition->nameLen,
		  p_entry_common_definition->name,
		  opt_args.len,
      	  opt_args.p_char);
	  #endif

	  // call Modules Undefine Fn, and get ret msg. Interpret NULL as veto !
	  Entry_String_t* p_entry_ret_msg =
		  p_entry_common_definition->module->provided->Undefine_Fn(p_entry_common_definition); //, opt_args);

	  // got an error msg? -> VETO, cancel delete, return with error msg!
	  if (p_entry_ret_msg) {

	      // insert ret_msg entrys in stail-queue
	      STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);
		  
         goto veto;
	  }
  }

// -------------------------------------------------------------------------------------------------

  // remove the definition from SCDE-Root
  STAILQ_REMOVE(&p_SCDERoot->HeadCommon_Definitions,
	  p_entry_common_definition, Common_Definition_s, entries);

  // now do the common cleanup

  // free the initial state '???' - may be NULL now!
  if (p_entry_common_definition->state) free(p_entry_common_definition->state);

  // check for Readings here, and free if any
  // !!! to do

  // check for Attributes here, and free if any
  // !!! to do

  // free the 'definition' string - may be NULL!
  if (p_entry_common_definition->def.p_char) free(p_entry_common_definition->def.p_char);

  // free the custom 'name' - is NOT NULL!
  free(p_entry_common_definition->nname.p_char);

  // delete the semaphore for definition access - is ALWAYS there !
  // vSemaphoreDelete(p_entry_common_definition->def_mux);

  // finally free the Definition
  free(p_entry_common_definition);

// -------------------------------------------------------------------------------------------------

  // return STAILQ head, stores multiple ret_msg, if NULL -> no ret_msg-entries
  return head_ret_msg;
  
// -------------------------------------------------------------------------------------------------

veto:

  // An veto occured! Cancel the Definition-Deletion! We have a ret_msg entry in the STAILQ

  // return head of singly linked tail queue, which holds 'ret_msg' elements
  return head_ret_msg;
}


