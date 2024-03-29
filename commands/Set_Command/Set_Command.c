﻿/* #################################################################################################
 *
 *  Function: Set Command for SCDE (Smart Connected Device Engine)
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

#include "include/Set_Command.h"



// set default build verbose - if no external override
#ifndef Set_Command_DBG  
#define Set_Command_DBG  5	// 5 is default
#endif



// -------------------------------------------------------------------------------------------------

// make data root locally available
static SCDERoot_t* SCDERoot;

// make locally available from data-root: the SCDEFn (Functions / callbacks) for faster operation
static SCDEFn_t* SCDEFn;

// -------------------------------------------------------------------------------------------------



/* --------------------------------------------------------------------------------------------------
 *  DName: Set_ProvidedByCommand
 *  Desc: Data 'Provided By Command' for this Command (functions + infos this command provides
 *        to SCDE)
 *  Data: 
 * --------------------------------------------------------------------------------------------------
 */

// Command Help
const uint8_t Set_helpText[] = 
  "Usage: Set <Def-Name> <Type dependent arguments or ?>";
// CommandHelp (detailed)
const uint8_t Set_helpDetailText[] = 
  "Usagebwrebwerb: define <name> <type> <options>, to define a device";

ProvidedByCommand_t Set_ProvidedByCommand = {
  "Set",					// Command-Name of command -> libfilename.so !
  3,						// length of cmd
  Set_Initialize_Command,			// Initialize Fn
  Set_Command,				// the Fn code
  { &Set_helpText, sizeof(Set_helpText) },
  { &Set_helpDetailText, sizeof(Set_helpDetailText) }
};



/* --------------------------------------------------------------------------------------------------
 *  FName: Set_InitializeCommandFn
 *  Desc: Initializion of SCDE Function Callback of an new loaded command
 *  Info: Stores Command-Information (Function Callbacks) to SCDE-Root
 *  Para: SCDERoot_t* SCDERootptr -> ptr to SCDE Data Root
 *  Rets: ? unused
 *--------------------------------------------------------------------------------------------------
 */
int 
Set_Initialize_Command(SCDERoot_t* SCDERootptr)
{
  // make data root locally available
  SCDERoot = SCDERootptr;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  SCDEFn = SCDERootptr->SCDEFn;

// --------------------------------------------------------------------------------------------------

  #if Set_Command_DBG >= 3
  SCDEFn->Log3Fn(Set_ProvidedByCommand.commandNameText
	,Set_ProvidedByCommand.commandNameTextLen
	,3
	,"InitializeFn called. Now useable.");
  #endif

// --------------------------------------------------------------------------------------------------

  return 0;
}



/**
  * --------------------------------------------------------------------------------------------------
  * FName: CommandSet -> gets arguments from SET 'Name Args'
  * Desc: Passes args "Args" to the SetFn of the define with name "Name"
  *
  * , and Module "TypeName" and calls Modules SetFn with
  *       args "Args"
  * Info: SET 'Name' 'Args'
  *       'Name' is a valid definition name [azAZ09._] char[31]
  *       'Args' are passed to modules SetFn for processing
  * Para: char* Args -> prt to space seperated txt string "Name TypeName Definition..."
  * Rets: char* -> error-text-string in allocated mem, or NULL=OK
  * --------------------------------------------------------------------------------------------------
  **/

/**
 * -------------------------------------------------------------------------------------------------
 *  FName: Set_Command
 *  Desc: Creates a new common define with name "Name", and Module "Type-Name", prevills some common
 *        values and calls Modules DefFn with for further module-specific initialization.
 *  Info: 'Name' is custom definition name. Allowed: [azAZ09._], uint8_t[32]
 *        'Type-Name' is module name
 *        'Definition ...' is custom and passed to modules DefineFn, stored in Definition->Definition
 *  Para: const uint8_t *args  -> prt to space seperated define text string "Name Type-Name Definition ..."
 *        const size_t argsLen -> length of args
 *  Rets: struct head_string_s -> STAILQ head of multiple retMsg, if NULL -> no retMsg-entry
 * -------------------------------------------------------------------------------------------------
 */
struct head_string_s
Set_Command (const uint8_t *args
		,const size_t argsLen)
{
  #if Set_Command_DBG >= 7
  SCDEFn->Log3Fn(Set_ProvidedByCommand.commandNameText
	,Set_ProvidedByCommand.commandNameTextLen
	,7
	,"CommandFn called with args '%.*s'"
	,argsLen
	,args);
  #endif

// --------------------------------------------------------------------------------------------------

  // prepare STAILQ head for multiple RetMsg storage
  struct head_string_s p_ret_msg_entry_sting;

  // Initialize the queue
  STAILQ_INIT(&p_ret_msg_entry_sting);

  // set start of possible Name
  const uint8_t *name = args;

  // set start of possible Set-Arguments
  const uint8_t *setArgs = args;

  // a seek-counter
  int i = 0;

  // seek to next space !'\32'
  while( (i < argsLen) && (*setArgs != ' ') ) {i++;setArgs++;}

  // length of Name
  size_t nameLen = i;

  // seek to start position of Set-Arguments, after '\32'
  while( (i < argsLen) && (*setArgs == ' ') ) {i++;setArgs++;}

   // length of Set-Arguments
  size_t setArgsLen = argsLen - i;

  // veryfy lengths > 0
  if ( (nameLen) == 0 || (setArgsLen == 0) ) {

	// alloc mem for retMsg
	strTextMultiple_t *retMsg =
		 malloc(sizeof(strTextMultiple_t));

	// response with error text
	retMsg->strTextLen = asprintf(&retMsg->strText
		,"Error! Could not execute command SET! Arguments '%.*s' not sufficient! Usage: Set <Def-Name> <Type dependent arguments or ?>"
		,argsLen
		,args);

	// insert retMsg in stail-queue
	STAILQ_INSERT_TAIL(&p_ret_msg_entry_sting, retMsg, entries);

	// return STAILQ head, stores multiple retMsg, if NULL -> no retMsg-entries
	return p_ret_msg_entry_sting;
  }

  // get the p_entry_common_definition by Name
  entry_common_definition_t *p_entry_common_definition = NULL;

  strTextMultiple_t *retMsg = NULL;

  STAILQ_FOREACH(p_entry_common_definition, &SCDERoot->HeadCommon_Definitions, entries) {

	if ( (p_entry_common_definition->nameLen == nameLen)
		&& (!strncasecmp((const char*) p_entry_common_definition->name, (const char*) name, nameLen)) ) {

        char **p_p_argv;
        int argc;

        // split 'setArgs' the default way
        p_p_argv = SCDEFn->ArgParse_SplitArgsToAllocatedMemFn(&argc,
            setArgs, setArgsLen);
                
        // set fn installed + argc >= 1 ?
		if ( (p_entry_common_definition->module->provided->Set) && ( argc >= 1 ) ) {

            // call modules set fn
			retMsg = p_entry_common_definition->module->provided->Set(p_entry_common_definition, p_p_argv, argc);

			// got an error msg?
			if (retMsg) {

				// insert retMsg in stail-queue
				STAILQ_INSERT_TAIL(&p_ret_msg_entry_sting, retMsg, entries);

			}

		}

		else	{

			// response with error -> Type doesnt support set Fn
			// alloc mem for retMsg
			strTextMultiple_t *retMsg =
				 malloc(sizeof(strTextMultiple_t));

			// response with error text
			retMsg->strTextLen = asprintf(&retMsg->strText
				,"Error! Could not execute command SET at Define '%.*s' with arguments '%.*s'! No args, or Type '%.*s' does not support it!"
				,p_entry_common_definition->nameLen
				,p_entry_common_definition->name
				,setArgsLen
				,setArgs
			   	,p_entry_common_definition->module->provided->typeNameLen
				,p_entry_common_definition->module->provided->typeName);

			// insert retMsg in stail-queue
		    STAILQ_INSERT_TAIL(&p_ret_msg_entry_sting, retMsg, entries);
		}

        // ArgParseFn has allocated mem. Free it.
        SCDEFn->ArgParse_FreeSplittedArgsInAllocatedMemFn(p_p_argv);

	    // return STAILQ head, stores multiple retMsg, if NULL -> no retMsg-entries
	    return p_ret_msg_entry_sting;
	}
  }

  // alloc mem for retMsg
  //strTextMultiple_t *retMsg =
  retMsg =
	 malloc(sizeof(strTextMultiple_t));

  // response with error text
  retMsg->strTextLen = asprintf(&retMsg->strText
	,"Error! Could not execute command SET! Definition '%.*s' not found!"
 	,nameLen
	,name);
	
  // insert retMsg in stail-queue
  STAILQ_INSERT_TAIL(&p_ret_msg_entry_sting, retMsg, entries);

  // return STAILQ head, stores multiple retMsg, if NULL -> no retMsg-entries
  return p_ret_msg_entry_sting;

  }



