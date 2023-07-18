/* #################################################################################################
 *
 *  Function: GLOBAL Module for SCDE (Smart Connected Device Engine)
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

#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>


#include "logging.h"

// the Smart-Connected-Device-Engine structures & types ..
#include <SCDE_s.h>

// provides WebIf, need the structures & types ...
//#include "WebIf_Module.h"

// this Modules structures & types ...
#include "GLOBAL_Module.h"
#include "GLOBAL_readings.h"


// -------------------------------------------------------------------------------------------------

LOG_TAG("GLOBAL Module");

// developers debug information 0 = off 5 max?
#define SCDED_DBG 5
#define SCDEH_DBG 5








ProvidedByModule_t GLOBAL_ProvidedByModule;







/**
 * -------------------------------------------------------------------------------------------------
 *  DName: GLOBAL_Module
 *  Desc: Data 'Provided By Module' for the GLOBAL module (functions + infos this module provides
 *        to SCDE)
 *  Data: 
 * -------------------------------------------------------------------------------------------------
 */
ProvidedByModule_t GLOBAL_ProvidedByModule = { // A-Z order
  "GLOBAL"				// Type-Name of module -> on Linux libfilename.so !
  ,6					// size of Type-Name

  ,&GLOBAL_reading_types    // embedded reading types
  
  ,NULL					// Add
  ,NULL					// Attribute
  ,GLOBAL_Define		// Define
  ,NULL					// Delete
  ,NULL					// DirectRead
  ,NULL					// DirectWrite
  ,NULL					// Except
  ,NULL					// Get
  ,NULL					// IdleCb
  ,GLOBAL_Initialize	// Initialize
  ,NULL					// Notify
  ,NULL					// Parse
  ,NULL					// Read
  ,NULL					// Ready
  ,NULL					// Rename
  ,NULL					// Set
  ,NULL					// Shutdown
  ,NULL					// State
  ,NULL					// Sub
  ,NULL					// Undefine
  ,NULL					// Write
  ,NULL					// FnProvided
  ,sizeof(Entry_GLOBAL_Definition_t)		// Modul specific Size (Entry_Common_Definition_t + X)
};



/**
 * -------------------------------------------------------------------------------------------------
 *  FName: GLOBAL_Initialize
 *  Desc: Initializion of SCDE Function Callbacks of an new loaded module
 *  Info: Stores Module-Information (Function Callbacks) to SCDE-Root
 *  Para: SCDERoot_t* SCDERootptr -> ptr to SCDE Data Root
 *  Rets: ? unused
 *--------------------------------------------------------------------------------------------------
 */
int 
GLOBAL_Initialize(SCDERoot_t* SCDERootptr)
{
  // make data root locally available
  SCDERoot_at_GLOBAL_M = SCDERootptr;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  SCDEFn_at_GLOBAL_M = SCDERootptr->SCDEFn;

  SCDEFn_at_GLOBAL_M->Log3Fn(GLOBAL_ProvidedByModule.typeName
		  ,GLOBAL_ProvidedByModule.typeNameLen
		  ,3
		  ,"InitializeFn called. Type '%.*s' now useable."
		  ,GLOBAL_ProvidedByModule.typeNameLen
		  ,GLOBAL_ProvidedByModule.typeName);

  return 0;
}



/**
 * -------------------------------------------------------------------------------------------------
 *  FName: GLOBAL_Define
 *  Desc: Finalizes the defines a new "device" of 'GLOBAL' type. Contains devicespecific init code.
 *  Info: 
 *  Para: Entry_Common_Definition_t *p_entry_common_definition -> prefilled ESP32Control Definition
 *        char *Definition -> the last part of the CommandDefine arg* 
 *  Rets: strTextMultiple_t* -> response text NULL=no text
 * -------------------------------------------------------------------------------------------------
 */
entry_string_t *
GLOBAL_Define(Entry_Common_Definition_t *p_entry_common_definition)
{
  // the ret msg
  Entry_String_t *p_entry_ret_msg = SCDE_OK;

  // make common ptr to modul specific ptr
  Entry_GLOBAL_Definition_t* p_entry_GLOBAL_definition =
		  (Entry_GLOBAL_Definition_t*) p_entry_common_definition;

// -------------------------------------------------------------------------------------------------

  #if GLOBAL_Module_DBG >= 5
  SCDEFn_at_GLOBAL_M->Log3Fn(p_entry_common_definition->name
	,p_entry_common_definition->nameLen
	,5
	,"Define Fn ('%.*s' Module) is called to continue creation of definition '%.*s' with args '%.*s'."
	,p_entry_GLOBAL_definition->common.module->provided->typeNameLen
	,p_entry_GLOBAL_definition->common.module->provided->typeName
	,p_entry_GLOBAL_definition->common.nameLen
	,p_entry_GLOBAL_definition->common.name
	,p_entry_GLOBAL_definition->common.definition.len
	,p_entry_GLOBAL_definition->common.definition.p_char);
  #endif
  
// -------------------------------------------------------------------------------------------------

  // store FD to Definition. Will than be processed in GLOBAL loop ... -> THIS MODULE USES NO FD
  p_entry_GLOBAL_definition->common.fd = -1;

// -------------------------------------------------------------------------------------------

  // init WebIf_Provided offset
//  ESP32Control_Definition->common.link =
//	&ESP32Control_Definition->WebIf_Provided;
/*
  // check for loaded Module 'WebIf' -> get provided Fn
  p_entry_GLOBAL_definition->WebIf_Provided.WebIf_FnProvided =
	NULL;//(WebIf_FnProvided_t *) SCDEFn_at_GLOBAL_M->GetFnProvidedByModule("WebIf");

 // Providing data for WebIf? Initialise data provided for WebIf
  if (p_entry_GLOBAL_definition->WebIf_Provided.WebIf_FnProvided) {

	p_entry_GLOBAL_definition->WebIf_Provided.ActiveResourcesDataA =
		(WebIf_ActiveResourcesDataA_t *) &ESP32Control_ActiveResourcesDataA_forWebIf;

	ESP32Control_Definition->WebIf_Provided.ActiveResourcesDataB =
		(WebIf_ActiveResourcesDataB_t *) &ESP32Control_ActiveResourcesDataB_forWebIf;

	}

  else	{

	SCDEFn_at_GLOBAL_M->Log3Fn(p_entry_common_definition->name
		,p_entry_common_definition->nameLen
		,1
		,"Could not enable WebIf support for '%.*s'. Type '%.*s' detects Type 'WebIf' is NOT loaded!"
		,p_entry_GLOBAL_definition->common.nameLen
		,p_entry_GLOBAL_definition->common.name
		,p_entry_GLOBAL_definition->common.module->ProvidedByModule->typeNameLen
		,p_entry_GLOBAL_definition->common.module->ProvidedByModule->typeName);
	}
*/

  // there should be no return messages - we expect no return messages
//  SCDEFn_at_GLOBAL_M->AnalyzeCommandFn((const uint8_t *) "attr global verbose 3", 21);
//  SCDEFn_at_GLOBAL_M->AnalyzeCommandFn((const uint8_t *) "attr global logfile -", 21);


  // for response msg from AnalyzeCommand
  struct head_string_s head_string;

  head_string = SCDEFn_at_GLOBAL_M->analyze_command_fn((const uint8_t *) "attr global configfile maker.cfg", 32);

 // retMsgMultiple stailq filled ?
  if (!STAILQ_EMPTY(&head_string)) {

	// for the retMsg elements
	entry_string_t *retMsg;

	// get the entries till empty
	while (!STAILQ_EMPTY(&head_string)) {

		retMsg = STAILQ_FIRST(&head_string);

		SCDEFn_at_GLOBAL_M->Log3Fn(p_entry_common_definition->name
			,p_entry_common_definition->nameLen
			,1
			,"Temp error msg ! result is:%.*s\n"
			,retMsg->string.len
			,retMsg->string.p_char);

		// done, remove this entry
		STAILQ_REMOVE(&head_string, retMsg, strTextMultiple_s, entries);

		// free the msg-string
		free(retMsg->string.p_char);

		// and the strTextMultiple_t
		free(retMsg);
	}
  }




//  SCDEFn_at_GLOBAL_M->AnalyzeCommandFn((const uint8_t *) "attr global statefile state.cfg", 31);
//  SCDEFn_at_GLOBAL_M->AnalyzeCommandFn((const uint8_t *) "attr global port 1234", 21);
   
   
   
   
  // -------------------------------------------------------------------------------------------------
/*
  // create required & maintained reading state
  entry_reading2_t *p_state_entry_reading =
      malloc(sizeof(entry_reading2_t));

  // the name 'state'
  p_state_entry_reading->reading.name.len = 
      asprintf((char **) &p_state_entry_reading->reading.name.p_char, "state");
 
  // update the raw data -> its text
  p_state_entry_reading->reading.raw_data.len = 
      asprintf((char **) &p_state_entry_reading->reading.raw_data.p_char
  	  ,"alive");
 
  // assign the reading type
  p_state_entry_reading->reading.p_entry_reading_type = 
      &GLOBAL_reading_types[0];
      
  // assign timestamp
  p_state_entry_reading->reading.timestamp = 0;
  
  // done, store in definition for fast acccess
  p_entry_GLOBAL_definition->common.p_state_reading = p_state_entry_reading;
  
  // and add reading to definitions readings queue
  STAILQ_INSERT_TAIL(&p_entry_GLOBAL_definition->common.head_readings2,
      p_state_entry_reading, entries);
   */   
// -------------------------------------------------------------------------------------------------
 
  // create maintained reading state (GLOBAL:STATE type), and store ptr in definition for fast access
  p_entry_GLOBAL_definition->common.p_state_reading =
      SCDEFn_at_GLOBAL_M->Create_Reading((Entry_Common_Definition_t *)p_entry_GLOBAL_definition,
          NULL, // default is "state"
          NULL, // keep default unit
          "alive2", // or NULL
          "GLOBAL", "state");//, 0);
   
// -------------------------------------------------------------------------------------------
//err:

  return p_entry_ret_msg;
}



