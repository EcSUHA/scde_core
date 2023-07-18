// SCDE-Module GLOBAL

#ifndef GLOBAL_MODULE_H
#define GLOBAL_MODULE_H

// -------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef GLOBAL_Module_DBG  
#define GLOBAL_Module_DBG  5	// 5 is default
#endif 

// -------------------------------------------------------------------------------------------------

// this Module is made for the Smart-Connected-Device-Engine
#include "SCDE_s.h"

// this Module provides functions for other Modules:
// -- no ---

// this Module uses an 1st stage:
// -- no ---

// -------------------------------------------------------------------------------------------------

// stores the Root Data of the Smart Connected Devices Engine - at/for this Module
SCDERoot_t* SCDERoot_at_GLOBAL_M;

// stores SCDEFn (Functions / callbacks) provided for operation - at/for this Module
SCDEFn_t* SCDEFn_at_GLOBAL_M;

// -------------------------------------------------------------------------------------------------

ProvidedByModule_t GLOBAL_ProvidedByModule;

// -------------------------------------------------------------------------------------------------






/* 
 * GLOBAL_Definition stores values for operation valid only for the defined instance of an
 * loaded module. Values are initialized by HCTRLD an the loaded module itself.
 */
typedef struct Entry_GLOBAL_Definition_s {

  Entry_Common_Definition_t common;		// ... the common part of the definition

//  WebIf_Provided_t WebIf_Provided;	// provided data for WebIf

//  uint32_t demo;			// custom data

  } Entry_GLOBAL_Definition_t;



/*
 *  Functions provided to SCDE by Module - for type operation (A-Z)
 */
entry_string_t * GLOBAL_Define(Entry_Common_Definition_t *p_entry_common_definition);

int GLOBAL_Initialize(SCDERoot_t* SCDERoot);



/*
 *  helpers
 */
//void ESP32Control_HexDumpOut (char *desc, void *addr, int len);



#endif /*GLOBAL_MODULE_H*/
