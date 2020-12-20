/* #################################################################################################
 *
 *  Function: Define Command - for SCDE (Smart Connected Device Engine)
 *            Creates an Definition (which may be called Device)
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

#include "Define_Command.h"


// --------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef Define_Command_DBG  
#define Define_Command_DBG  5	// 5 is default
#endif

// -------------------------------------------------------------------------------------------------

// make data root locally available
static SCDERoot_t* p_SCDERoot;

// make locally available from data-root: the SCDEFn (Functions / callbacks) for operation
static SCDEFn_t* p_SCDEFn;

// -------------------------------------------------------------------------------------------------



/* -------------------------------------------------------------------------------------------------
 *  DName: Define_ProvidedByCommand
 *  Desc: Data 'Provided By Command' for the Define Command (functions + infos this command provides
 *        to SCDE)
 *  Data: 
 * -------------------------------------------------------------------------------------------------
 */
// Command Help
const uint8_t Define_helpText[] = 
  "Usage: Define <definition-name> <type-name> <type dependent arguments>, to create an definition of that type.";
// CommandHelp (detailed)
const uint8_t Define_helpDetailText[] = 
  "Usage: Define <definition-name> <type-name> <type dependent arguments>, to define a device";


ProvidedByCommand_t Define_ProvidedByCommand = {
  "Define",					        // Command-Name of command -> libfilename.so !
  6,						        // length of cmd
  Define_InitializeCommand_Fn,	    // Initialize Fn
  Define_Command_Fn,				// the Fn code
  { &Define_helpText, sizeof(Define_helpText) },
  { &Define_helpDetailText, sizeof(Define_helpDetailText) }
};



/* -------------------------------------------------------------------------------------------------
 *  FName: Define_InitializeCommand_Fn
 *  Desc: Initializion of an (new loaded) SCDE-Command. Init p_SCDERoot and p_SCDE Function Callbacks.
 *  Info: Called only once befor use!
 *  Para: SCDERoot_t* p_SCDERoot_from_core -> ptr to SCDE Data Root from SCDE-Core
 *  Rets: ? unused
 *--------------------------------------------------------------------------------------------------
 */
int
Define_InitializeCommand_Fn(SCDERoot_t* p_SCDERoot_from_core)
{
  // make data root locally available
  p_SCDERoot = p_SCDERoot_from_core;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  p_SCDEFn = p_SCDERoot->SCDEFn;

// -------------------------------------------------------------------------------------------------

  #if Define_Command_DBG >= 3
  p_SCDEFn->Log3Fn(Define_ProvidedByCommand.commandNameText,
	  Define_ProvidedByCommand.commandNameTextLen,
	  3,
	  "InitializeFn called. Now useable.");
  #endif

// -------------------------------------------------------------------------------------------------

  return 0;
}



/* -------------------------------------------------------------------------------------------------
 *  FName: Define_Command_Fn
 *  Desc: Creates a new Definiton of Module "Type-Name" with custom Name "Name", prevills some common
 *        values and calls Modules DefineFn, to continue further module-specific initialization.
 *  Info: 'Definition-Name' is custom Definition name. Allowed: [azAZ09._], uint8_t[32]
 *        'Module-Name' is Module name
 *        'Definition-Args' is custom, stored in Definition->Definition, and passed to modules DefineFn
 *         an ret msg from DefineFn results in Veto (stop creation of definition)
 *  Para: const String_t args -> space seperated command args text string,
 *                               normally "definition_name module_name definition_args"
 *  Rets: struct Head_String_s -> STAILQ head of queue filled with multiple ret_msg,
 *                                if NULL -> no ret_msg
 * -------------------------------------------------------------------------------------------------
 */
struct Head_String_s
Define_Command_Fn(const String_t args)
{
  #if Define_Command_DBG >= 7
  p_SCDEFn->Log3Fn(Define_ProvidedByCommand.commandNameText,
	  Define_ProvidedByCommand.commandNameTextLen,
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
  String_t module_name;

  // take * to start searching 'module_name' text (seek-start-pos)
  module_name.p_char = definition_name.p_char;

  // seek * to end of previous args text (to next '\32' -> space)
  while( ( i < args.len ) && ( *module_name.p_char != ' ' ) ) { i++, j++ ; module_name.p_char++ ; }

  // length of 'definition_name' text determined
  definition_name.len = j;

  // seek * to start of 'module_name' text (skip '\32' -> space)
  while( ( i < args.len ) && ( *module_name.p_char == ' ' ) ) { i++ ; module_name.p_char++ ; }

  // expected argument #3
  String_t opt_args;

  // take * to start searching 'opt_args' text (seek-start-pos)
  opt_args.p_char = module_name.p_char;

  // clear element seek-counter
  j = 0;

  // seek * to end of previous args text (to next '\32' -> space)
  while( ( i < args.len ) && ( *opt_args.p_char != ' ' ) ) { i++ , j++ ; opt_args.p_char++ ; }

  // length of 'module_name' text determined
  module_name.len = j;

  // seek * to start of 'opt_args' (skip '\32' -> space)
  while( ( i < args.len ) && ( *opt_args.p_char == ' ' ) ) { i++ ; opt_args.p_char++ ; }
  
  // length of 'opt_args' text determined (its the rest)
  opt_args.len = args.len - i;

// -------------------------------------------------------------------------------------------------

  // verify lengths > 0, ! opt_args.len = 0 IS ALLOWED !
  if ( ( definition_name.len == 0 ) || ( module_name.len == 0 ) ) {

	  // alloc mem for ret_msg entry
	  Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		  "Error! Could not interpret '%.*s'! Usage: Define <definition-name> <type-name> [<type dependent arguments>]",
		  args.len,
		  (char *)args.p_char);

	  // insert ret_msg entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	  // return head of singly linked tail queue, which holds 'ret_msg' elements
      return head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------
  // dupe check
 /*
   if(Defined($defs{$name}));
   asprintf(&RetMsg, "%s already Defined, Define it first\n", name);
   return RetMsg;
*/

// -------------------------------------------------------------------------------------------------
  // name conventions check
/*
   if($name !~ m/^[a-z0-9.:_]*$/i);
   asprintf(&RetMsg, "Invalid characters in name (not A-Za-z0-9._): %s\n", name);
   return RetMsg;
  */
  
// -------------------------------------------------------------------------------------------------

   // get the module ptr by name
   Module_t* p_module = p_SCDEFn->GetLoadedModulePtrByNameFn(module_name.p_char,
 	   module_name.len);

  // do we got the module? Else try to load it
  if (!p_module) {

      // for reloadmodule command
	  String_t reloadmodule_cmd;

      // build reloadmodule command
	  reloadmodule_cmd.len = asprintf(&reloadmodule_cmd.p_char,
	      "reloadmodule %.*s",
		  module_name.len,
		  (char *)module_name.p_char);
		
	  // now execute the reloadmodule command
	  struct headRetMsgMultiple_s head_ret_msg_from_fn =
	    p_SCDEFn->AnalyzeCommandFn(reloadmodule_cmd.p_char, reloadmodule_cmd.len);

	  // free build
	  free(reloadmodule_cmd.p_char);

	  // ret-msg ? = error !
      if (!STAILQ_EMPTY(&head_ret_msg_from_fn)) {

 	      // add the ret_msg entrys from Fn return
	      STAILQ_CONCAT(&head_ret_msg, &head_ret_msg_from_fn);

	      // return head of singly linked tail queue, which holds 'ret_msg' elements
	      return head_ret_msg;
      }
  }

  // retry - get the module ptr by name
  p_module = p_SCDEFn->GetLoadedModulePtrByNameFn(module_name.p_char,
 	   module_name.len);

  // still no Module -> we don't have it, error!
  if (!p_module) {					// normally can not reach this point !!!!!!! exept when module n.o.k.

	  // alloc mem for ret_msg entry
	  Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		  "Error! Could not load Module-Type '%.*s'.",
		  module_name.len,
		  (char *)module_name.p_char);

	  // insert ret_msg entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	  // return head of singly linked tail queue, which holds 'ret_msg' elements
      return head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

  // can we get ptr to an Definition with requested Name? -> same Name is not allowed, error!
  if (p_SCDEFn->GetDefinitionPtrByNameFn(definition_name.len, definition_name.p_char)) {

	  // alloc mem for ret_msg entry
	  Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
	      "Error! An Definition called '%.*s' is already in use.",
		  definition_name.len,
		  (char *)definition_name.p_char);

	  // insert ret_msg entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	  // return head of singly linked tail queue, which holds 'ret_msg' elements
      return 	head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

  // alloc mem for modul specific definition structure (Entry_Common_Definition_t + X)
  Entry_Common_Definition_t* p_new_entry_common_definition
	  = malloc(p_module->provided->sizeof_custom_definition);

  // zero the struct
  memset(p_new_entry_common_definition, 0, p_module->provided->sizeof_custom_definition);

// -------------------------------------------------------------------------------------------------

  // prepare default fields in definition

  // create semaphore for definition access
 // p_new_entry_common_definition->def_mux = xSemaphoreCreateMutex();

  // copy ptr to associated module
  p_new_entry_common_definition->module = p_module;

  // init the list for 'attr_value's
  LIST_INIT(&p_new_entry_common_definition->head_attr_value);

  // store custom 'name' (NAME LENGTH IS > 0 !)
  p_new_entry_common_definition->nname.p_char = malloc(definition_name.len);
  memcpy(p_new_entry_common_definition->nname.p_char, definition_name.p_char, definition_name.len);
  p_new_entry_common_definition->nname.len = definition_name.len;

  // store 'definition' !!ITS ALLOWED TO HAVE NO DEFINITION!!
  if (opt_args.len) {
	p_new_entry_common_definition->def.p_char = malloc(opt_args.len);
	memcpy(p_new_entry_common_definition->def.p_char, opt_args.p_char, opt_args.len);
	p_new_entry_common_definition->def.len = opt_args.len;
  }

  // assign an Unique Number
  p_new_entry_common_definition->nr = p_SCDERoot->device_count++;

  // init readings (stailq), now empty
  STAILQ_INIT(&p_new_entry_common_definition->headReadings);

  // set initial state '???'
  p_new_entry_common_definition->stateLen =
	  asprintf((char**)&p_new_entry_common_definition->state, "???");

// -------------------------------------------------------------------------------------------------

  // xx mark definition as under construction?

  // store new definition to SCDE-Root
  STAILQ_INSERT_HEAD(&p_SCDERoot->HeadCommon_Definitions,
	  p_new_entry_common_definition, entries);

// -------------------------------------------------------------------------------------------------

  // DefineFn NOT assigned by module ? -> retMsg + VETO!
  if (!p_module->provided->define_fn) {

      // alloc mem for ret_msg entry
	  Entry_String_t* p_entry_ret_msg =
		  malloc(sizeof(Entry_String_t));

	  // response with error text
	  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		  "Error! DefineFn not implemented in Module of Type '%.*s'. Veto.",
		  module_name.len,
		  (char *)module_name.p_char);

	  // insert ret_msg entry in stail-queue
	  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);
  }

  // Call DefineFn assigned by module -> retMsg forces VETO!
  else {

	  #if Define_Command_DBG >= 7
	  p_SCDEFn->Log3Fn(Define_ProvidedByCommand.commandNameText,
		  Define_ProvidedByCommand.commandNameTextLen,
		  7,
		  "Calling DefineFn of Module '%.*s' for construction of new Definition '%.*s' "
		  "using arguments '%.*s'. (Nr.:%d, Initial State:%.*s).",
		  p_new_entry_common_definition->module->provided->typeNameLen,
		  p_new_entry_common_definition->module->provided->typeName,
		  p_new_entry_common_definition->nameLen,
		  p_new_entry_common_definition->name,
		  p_new_entry_common_definition->definitionLen,
      	  p_new_entry_common_definition->definition,
    	  p_new_entry_common_definition->nr,
		  p_new_entry_common_definition->stateLen,
     	  p_new_entry_common_definition->state);
	  #endif

	  // call Modules Define Fn, and get ret msg. Interpret NULL as veto !
	  Entry_String_t* p_entry_ret_msg = 
		  p_module->provided->define_fn(p_new_entry_common_definition);

	  // got an error msg? -> VETO, cancel define, clean up in next steps!
	  if (p_entry_ret_msg) {

		  // insert ret_msg entry in stail-queue
		  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);
	  }
  }

// -------------------------------------------------------------------------------------------------

  // ret_msg elements in the stailq? veto!
  if (!STAILQ_EMPTY(&head_ret_msg)) goto veto;

  // return head of singly linked tail queue, which holds 'ret_msg' elements
  return head_ret_msg;
  
// -------------------------------------------------------------------------------------------------

veto:

  // An veto occured! Cancel the Definition-Creation! We have a ret_msg entry in the STAILQ
  
  #if Define_Command_DBG >= 7
  p_SCDEFn->Log3Fn(Define_ProvidedByCommand.commandNameText,
	  Define_ProvidedByCommand.commandNameTextLen,
	  7,
	  "An veto occured while creating an Definition with args '%.*s'. Check logged msg.",
	  args.len,
	  args.p_char);
  #endif

  // remove new definition from SCDE-Root
  STAILQ_REMOVE(&p_SCDERoot->HeadCommon_Definitions,
	  p_new_entry_common_definition, Entry_Common_Definition_s, entries);

  // free the initial state '???' - may be NULL now!
  if (p_new_entry_common_definition->state) free(p_new_entry_common_definition->state);

  // check for Readings here, and free if any
  // !!! to do

  // check for Attributes here, and free if any
  // !!! to do
  
  // free the 'definition' string - may be NULL!
  if (p_new_entry_common_definition->def.p_char) free(p_new_entry_common_definition->def.p_char);

  // free the custom 'name' - is NOT NULL!
  free(p_new_entry_common_definition->nname.p_char);

  // delete the semaphore for definition access - is ALWAYS there !
  // vSemaphoreDelete(p_new_entry_common_definition->def_mux);

  // finally free the Definition
  free(p_new_entry_common_definition);

// -------------------------------------------------------------------------------------------------

  // return STAILQ head, stores multiple ret_msg, if NULL -> no ret_msg-entries
  return head_ret_msg;
}


