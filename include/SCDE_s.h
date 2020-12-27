//                                                                                        +++ 100>|

#ifndef _SCDE_S_H_
#define _SCDE_S_H_

#include <sys/queue.h>


// include the forward declarations of SCDE-Core components
#include "argparse_s_fwd.h"

#define SCDE_OK  NULL

// -------------------------------------------------------------------------------------------------

/*
 * Project TYPO corrections (temporary)
 */ 
 
// 1. groß / kleinschreibung Typen
#define string_s     String_s
#define string_t     String_t
#define entry_string_s     Entry_String_s
#define entry_string_t     Entry_String_t
#define head_string_s     Head_String_s
#define reading_s     Reading_s
#define reading_t     Reading_t
#define entry_reading_s     Entry_Reading_s
#define entry_reading_t     Entry_Reading_t

// 2. renamed during work

//        alt                      neu
#define Entry_Definition_s     Entry_Common_Definition_s
#define Common_Definition_s    Entry_Common_Definition_s

#define Common_Definition_t    Entry_Common_Definition_t
#define Entry_Definition_t     Entry_Common_Definition_t

#define HeadCommon_Definitions head_definition

//        alt            neu
#define Command_s    Entry_Command_s
#define Command_t    Entry_Command_t
#define headCommands head_command

//        alt        neu
#define Module_s    Entry_Module_s
#define Module_t    Entry_Module_t
#define HeadModules head_module

//        alt        neu
#define ProvidedByModule_s Provided_By_Module_s
#define ProvidedByModule_t Provided_By_Module_t


//#define ESP32_SwITCH_Definition_s  Entry_ESP32_SwITCH_Definition_s
//#define ESP32_SwITCH_Definition_t  Entry_ESP32_SwITCH_Definition_t


// -------------------------------------------------------------------------------------------------

/*
 * SCDE forward declarations
 */ 

// typedef for string_t, holds an text string with given length
typedef struct string_s string_t;

// typedef for entry_tring_t, holds an string (to use as entry in an singly linked tail queue)
typedef struct entry_string_s entry_string_t;

// SCDE Root (type) holds the Smart Connected Devices Engine - root data
typedef struct SCDERoot_s SCDERoot_t;

// Module (type) stores information of loaded modules, required for module use and operation
typedef struct Entry_Module_s Entry_Module_t;

// typedef for Entry_Reading_t - an SLTQ entry that stores one reading
typedef struct Entry_Reading_s Entry_Reading_t;

// Provided by Module (type) stores (the common) function callbacks for SCDE module operation
typedef struct Provided_By_Module_s Provided_By_Module_t;

// Entry Command (type) stores commands made available for operation.
typedef struct Entry_Command_s Entry_Command_t;

// Entry_Common_Definition_t stores values (the common part) for operation of an definition
typedef struct Entry_Common_Definition_s Entry_Common_Definition_t;

// ?
typedef struct Common_StageXCHG_s Common_StageXCHG_t;


// typedef for entry_definition_to_be_notified_t - stores an definition that should get a notify
typedef struct entry_definition_to_be_notified_s entry_definition_to_be_notified_t;

// typedef for notify_t - stores the content of one notify
typedef struct notify_s notify_t;

// typedef for entry_notify_t - an STAILQ entry that stores one notify
typedef struct entry_notify_s entry_notify_t;

// typedef for Changed_t - stores general infos to process the changes + includes the pending notifies LIST head
typedef struct changed_s changed_t;

// typedef for Reading_t - stores the content of one reading
typedef struct Reading_s Reading_t;

// typedef for Entry_Reading_t - an STAILQ entry that stores one reading
typedef struct Entry_Reading_s Entry_Reading_t;

// -------------------------------------------------------------------------------------------------



// the global stuff from the SCDE-Core Fn - here???
#include "argparse_s.h"





// NEW TYPE DEFINITIONS

// GENERAL

// size_t for this system
//typedef size_t uint32_t;


// ptr zur Fehlermeldung in allokierter strTextMultiple_t WENN NOCH KEIN FEHLER WAR
#define SCDE_OK  NULL



// ↓↓↓↓↓↓↓↓↓↓↓↓ typedefs and structs for string storage and processing  ↓↓↓↓↓↓↓↓↓↓↓↓



/*
 * string_s (struct)
 * Holds an string (ptr to characters-array (not zero terminated!), typically in allocated memory,
 * including the length of this characters-array)
 */
struct string_s {
  uint8_t* p_char;  // ptr to allocated memory, filled with character-array, not zero terminated!
  size_t len;		// length of the character-array
};


// -------------------------------------------------------------------------------------------------



/*
 * entry_string_s (struct) 
 * Holds an string (to use as entry in an singly linked tail queue)
 */
struct entry_string_s {
  STAILQ_ENTRY(entry_string_s) entries;	// link to next element of the singly linked tail queue
  String_t string;				        // the string of this entry
};

// Constructor for the singly linked tail queue (head), which can hold multiple linked string entries
STAILQ_HEAD(head_string_s, entry_string_s);


// -------------------------------------------------------------------------------------------------


// FIRST TRY ..

// OBSOLETE: use String_t
typedef struct strText_s strText_t;

/* OBSOLETE: use xString_s
 * strText_s (struct)
 * - holds normally a text string with length (not zero terminated)
 * - may be raw data too !
 * - *strText = NULL -> empty, else filled !
 */
struct strText_s {
  char *strText;				// ptr to allocated mem filled with text
  size_t strTextLen;				// length of the text
};

// -------------------------------------------------------------------------------------------------

// OBSOLETE: use  Entry_String_s            Entry_String_t
typedef struct strTextMultiple_s strTextMultiple_t;

/* OBSOLETE: use xMultipleString_s
 * strTextMultiple_s (struct)
 * - holds multiple text strings with length (not zero terminated) (linked queue)
 */
// OBSOLETE: use Entry_String_s
struct strTextMultiple_s {
  STAILQ_ENTRY(strTextMultiple_s) entries;	// Link to next strTextMultiple
  char *strText; 				// ptr to allocated mem filled with text
  size_t strTextLen;				// length of the text
};

// OBSOLETE: use Head_String_s, Entry_String_s
STAILQ_HEAD(headRetMsgMultiple_s, strTextMultiple_s);

// -------------------------------------------------------------------------------------------------







/*
 *  ...
 */

// Commands A-Z - will be removed loaded!!
//typedef char* (* CommandDefineFn_t) (const uint8_t *args, const size_t argsLen);
//typedef char* (* CommandSetFn_t) (const uint8_t *args, const size_t argsLen);
//typedef strTextMultiple_t* (* CommandUndefineFn_t) (const uint8_t *args, const size_t argsLen);



// -------------------------------------------------------------------------------------------------



/*
 *  Helper routine that parses Key=Value(@) input arguments into an allocated array
 */
// holding the input (IK)
typedef struct kvParseImplementedKeys_s kvParseImplementedKeys_t;

// Data structure that holds the implemented keys and affected readings.
// Used as input for Key=Value input parsing.
struct kvParseImplementedKeys_s {

  // Bit-Field of the affected readings
  const uint32_t affectedReadings;

  // text of the implemented key
  const char *implementedKey;

  // text of the input options for the key
  const char *inputOptions;
};

 // holds the parsed result in allocated mem
typedef struct parsedKVInputArgs_s parsedKVInputArgs_t;

// Result structure for http_parser_parse_url().
// Callers should index into field_data[] with UF_* values if field_set
// has the relevant (1 << UF_*) bit set. As a courtesy to clients (and
// because we probably have padding left over), we convert any port to
// a uint32_t.
struct parsedKVInputArgs_s {
  uint64_t requiredKVBF;	// Bit-Field set by user -> keys required for a successful parse
  uint64_t forbiddenKVBF;	// Bit-Field set by user -> keys forbidden for a successful parse
  uint64_t keysFoundBF;		// Bit-Field set by parseKV -> stores the keys found (1 << XX_IK_*)
  uint32_t affectedReadingsBF;	// Bit-Field set by parseKV -> stores the affected readings

  struct keyData_s {
    uint16_t off;		// Offset into buffer in which value-text starts
    uint16_t len;		// Length of value-text in buffer
    uint32_t affectedReadings;
  } keyData_t[];		// XX_IK_MAX
};

// Hepler routine that parses Key=Value(@) input arguments into an allocated array
typedef parsedKVInputArgs_t* (* ParseKVInputArgsFn_t) (int numImplementedKeys, const kvParseImplementedKeys_t *XX_IK, const uint8_t *kvArgsText, const size_t kvArgsTextLen);



//--------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------



/*
 * typedefs of Function Callbacks
 * This are provided & made accessible for modules and commands - for operation and helpers
 */
// typedef for AnalyzeCommandFn - analyzes + processes one command row
typedef struct headRetMsgMultiple_s (*AnalyzeCommandFn_t) (const uint8_t *args, const size_t argsLen);

// typedef for AnalyzeCommandChainFn - analyzes + processes an configuration file
typedef struct headRetMsgMultiple_s (*AnalyzeCommandChainFn_t) (const uint8_t *args, const size_t argsLen);

// typedef for Call GetFn by Def-Name - for 2 stage desings, requests data
typedef int (* CallGetFnByDefNameFn_t) (const uint8_t *nameText, const size_t nameTextLen, Entry_Common_Definition_t *sourceCommon_Definition, void *X);

// typedef for CommandReloadModule - Initially loads or executes a reload of an Module of given type-name
typedef Module_t* (*CommandReloadModuleFn_t)(const String_t type_name);

// typedef for Devspec2ArrayFn - returns all definitions that match the given devicespecification (devspec)
//typedef struct xHeadMultipleStringSLTQ_s (*Devspec2ArrayFn_t) (const xString_t devspecString);
typedef struct Head_String_s (*Devspec2ArrayFn_t) (const String_t devspecString);

//
typedef Entry_String_t* (*Do_Trigger_Fn_t) (Entry_Common_Definition_t *p_entry_common_definition, Entry_Reading_t *p_changed_state);

// typedef - returns formated text of Date-Time from tist
typedef string_t (*Get_Formated_Date_Time_Fn_t) (time_t time);

// typedef for FmtTimeFn - returns formated text of Time from tist
typedef strText_t (*FmtTimeFn_t) (time_t time);

// typedef for Call Get All Readings Fn by Def-Name
typedef struct headRetMsgMultiple_s (*GetAllReadingsFn_t) (Entry_Common_Definition_t *Common_Definition);

// typedef for Call Get Definition and Attributes setup lines Fn by Def-Name
typedef struct headRetMsgMultiple_s (*GetDefAndAttrFn_t) (Entry_Common_Definition_t *Common_Definition);

//
//typedef Entry_Common_Definition_t * (*Get_Ptr_To_Definition_By_Name_Fn_t) (const size_t definitionNameLen, const uint8_t *definitionName);
typedef Entry_Common_Definition_t * (*Get_Ptr_To_Definition_By_Name_Fn_t) (const String_t definition_name);

// Returns a STAILQ head that stores entries of all 'dev_spec' matching definitions
typedef struct Head_Definition_Ptr_s (*Get_Definitions_That_Match_DefSpec_String_Fn_t) (const String_t dev_spec);

//
typedef Module_t* (*GetLoadedModulePtrByNameFn_t)(const uint8_t *typeName, const size_t typeNameLen);

// returns current SCDE Time Stamp
typedef time_t (*GetTiStFn_t)(void);

// returns an UNIQUE SCDE Time Stamp (in most cases the current Time Stamp)
typedef time_t (*GetUniqueTiStFn_t)(void);

// typedef for GoodDeviceNameFn - checks the given Device Name for device name rules
typedef bool (*GoodDeviceNameFn_t) (const String_t nameString);

// typedef for GoodReadingNameFn - checks the given Reading Name for reading name rules
typedef bool (*GoodReadingNameFn_t) (const String_t nameString);

// prints data as Hex-Dump to debug terminal
typedef void (* HexDumpOutFn_t) (char *desc, void *addr, int len);

// ! REMOVE ! should be core only log Fn
typedef void (*LogFn_t) (const char *internal_fn, const uint8_t log_level, const char *format, ...);

// log Fn for modular elements (Modules & Commands)
typedef void (*Log3Fn_t) (const uint8_t *p_name_char, const size_t name_len, const uint8_t log_level, const char *format, ...);
//new?: typedef void (*Log3Fn_t) (const String_t internal_fn, const uint8_t log_level, const char *format, ...);

//
typedef void (*Log4Fn_t) (char *text);

// typedef for MakeDeviceNameFn - corrects the given Device Name (overwrites illegal chars with '_')
typedef void (*MakeDeviceNameFn_t) (const String_t nameString);

// typedef for MakeReadingNameFn - corrects the given Reading Name (overwrites illegal chars with '_')
typedef void (*MakeReadingNameFn_t) (const String_t nameString);

// typedef for Readings_Begin_Update_Fn - call this before updating readings
typedef time_t (*Readings_Begin_Update_Fn_t) (Entry_Common_Definition_t *p_entry_common_definition);

// typedef for Readings_Bulk_Update_Fn - call this for every reading (bulk-update)
typedef int (*Readings_Bulk_Update_Fn_t) (Entry_Common_Definition_t *p_entry_common_definition, const String_t reading_name, const String_t reading_value, const bool changed, time_t timestamp);

// typedef for Readings_End_Update_Fn - call this to after bulk-update to process readings
typedef int (*Readings_End_Update_Fn_t) (Entry_Common_Definition_t *Common_Definition, bool doTrigger);

// typedef for TimeNowFn - returns current time stamp
typedef time_t (*TimeNowFn_t) ();

// typedef for WriteStatefileFn - 
typedef struct headRetMsgMultiple_s (*WriteStatefileFn_t) ();

// ----------------

// added Fn (Perl -> C)
typedef String_t* (*Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn_t) (const String_t* p_def_name, const String_t* p_attr_name);


// Argument Parser - (helpers)
/*
  // typedef for Argument Parser - Split Arguments To Allocated Mememory Fn
  typedef char ** (*ArgParse_SplitArgsToAllocatedMemFn_t) (int *argc, uint8_t *argsText,  size_t argsTextLen);

  // typedef for Argument Parser - Split URL Encoded Arguments To Allocated Memory Fn
  typedef char ** (*ArgParse_SplitURLEncodedArgsToAllocatedMemFn_t) (int *argc, char *argsText);

  // typedef for Argument Parser - Free Splitted Arguments In Allocated Memory Fn
  typedef void (*ArgParse_FreeSplittedArgsInAllocatedMemFn_t) (char **argv);

  // typedef for Argument Parser - Init Fn
  typedef Entry_String_t* (*ArgParse_InitFn_t) (struct argparse *self, struct argparse_option *options,
	const char *const *usages, int flags, const char *description, const char *epilog);

  // typedef for Argument Parser - Parse Fn
  typedef Entry_String_t* (*ArgParse_ParseFn_t) (struct argparse *self, int argc, const char **argv);

  // typedef for Argument Parser - Usage Fn
  typedef void (*ArgParse_UsageFn_t) (struct argparse *self);

  // typedef for Argument Parser - Prepare Leadin Error Message Fn
  typedef void (*ArgParse_PrepareLeadinErrorMsgFn_t) (struct argparse *self, const struct argparse_option *opt,
		const char *reason, int flags);
*/

/*
 * SCDEFn (SCDE Functions) typedef
 * Stores Function callbacks provided & made accessible for modules and commands - for operation and helpers
 */
typedef struct SCDEFn_s {
  AnalyzeCommandFn_t AnalyzeCommandFn;                      // analyzes + processes one command row
  AnalyzeCommandChainFn_t AnalyzeCommandChainFn;            // analyzes + processes an configuration file
  CallGetFnByDefNameFn_t CallGetFnByDefNameFn;              // original CallFn
  CommandReloadModuleFn_t CommandReloadModuleFn;            //
  Devspec2ArrayFn_t Devspec2ArrayFn;			            // returns all definitions that match devspec
  Do_Trigger_Fn_t Do_Trigger_Fn;                            //
  FmtTimeFn_t FmtTimeFn;                                    // returns formated text of Time from tist
  GetAllReadingsFn_t GetAllReadingsFn;                      // returns all readings of an definition
  GetDefAndAttrFn_t GetDefAndAttrFn;                        //
  Get_Ptr_To_Definition_By_Name_Fn_t get_ptr_to_definition_by_name_fn;  //
  Get_Definitions_That_Match_DefSpec_String_Fn_t Get_Definitions_That_Match_DefSpec_String_Fn;
  Get_Formated_Date_Time_Fn_t get_formated_date_time_fn;    // returns formated text of Date-Time from tist
  GetLoadedModulePtrByNameFn_t GetLoadedModulePtrByNameFn;  //
  GetTiStFn_t GetTiStFn;                                    // returns current SCDE Time Stamp
  GetUniqueTiStFn_t GetUniqueTiStFn;			            // returns an UNIQUE SCDE Time Stamp
  GoodDeviceNameFn_t GoodDeviceNameFn;			            // checks the given Device Name for device name rules
  GoodReadingNameFn_t GoodReadingNameFn;		            // checks the given Reading Name for device name rules
  HexDumpOutFn_t HexDumpOutFn;                              // prints data as Hex-Dump to debug terminal
  LogFn_t LogFn;                                            // Log -> This is the main logging function
  Log3Fn_t Log3Fn;                                          // Log -> This is the main logging function
  Log4Fn_t Log4Fn;                                          // Log -> This is the main logging function
  MakeDeviceNameFn_t MakeDeviceNameFn;			            // corrects the given Device Name
  MakeReadingNameFn_t MakeReadingNameFn;		            // corrects the given Reading Name   
  Readings_Begin_Update_Fn_t readings_begin_update_fn;      // call this before / to begin updating readings
  Readings_Bulk_Update_Fn_t readings_bulk_update_fn;        // call this to add an reading to the running update
  Readings_End_Update_Fn_t readings_end_update_fn;          // call this to after updating, to process readings & notifies

  // buid with : Argument Parser helpers ?
  ArgParse_SplitArgsToAllocatedMemFn_t ArgParse_SplitArgsToAllocatedMemFn;
  ArgParse_SplitURLEncodedArgsToAllocatedMemFn_t ArgParse_SplitURLEncodedArgsToAllocatedMemFn;
  ArgParse_FreeSplittedArgsInAllocatedMemFn_t ArgParse_FreeSplittedArgsInAllocatedMemFn;
  ArgParse_InitFn_t ArgParse_InitFn;
  ArgParse_ParseFn_t ArgParse_ParseFn;
  ArgParse_UsageFn_t ArgParse_UsageFn;
  ArgParse_PrepareLeadinErrorMsgFn_t ArgParse_PrepareLeadinErrorMsgFn;

  TimeNowFn_t TimeNowFn;                                   // returns current system time (no TiSt)
  WriteStatefileFn_t WriteStatefileFn;                     // 
// added Fn (Perl -> C)
  Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn_t Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn;
// not final
  ParseKVInputArgsFn_t ParseKVInputArgsFn;                 // parses Key=Value(@) input arguments into array
} SCDEFn_t;



//--------------------------------------------------------------------------------------------------



/*
 * typedefs for common Module Functions (callbacks) (provided for SCDE - to execute common module Fn),
 * Note: Should be only called if not NULL. Assignment of some are mandatory.
 */
 


/*
 * InitializeFn  - returns module information (module_s) required for operation
 * DeleteFn      - clean up (delete logfile), called by delete after UndefFn	// wird beim delete aufgerufen um das Device/Gerät endgültig zu löschen
 * ExceptFn      - called if the global select reports an except field
 * GetFn         - get some data from this device
 * NotifyFn      - call this if some device changed its properties
 * ParseFn       - Interpret a raw message
 * ReadFn        - Reading from a Device (see FHZ/WS300)
 * ReadyFn       - check for available data, if no FD
 * SetFn         - set/activate this device
 * ShutdownFn    - called before shutdown
 * StateFn       - set local info for this device, do not activate anything
 * UndefineFn    - clean up (delete timer, close fd), called by delete and rereadcfg	// Callback wird beim Undefine eines Devices/Gerätes aufgerufen
 */


/*
 *--------------------------------------------------------------------------------------------------
 *FName: Master_Initialize
 * Desc: Initializion of Home Control Function Callbacks in loaded module
 * Info: Initializion of available Module Function Callbacks for Home Control
 * Para: 
 * Rets: 
 *--------------------------------------------------------------------------------------------------
 */
/*

 *--------------------------------------------------------------------------------------------------
 *FName: Master_Define
 * Desc: Called from command define when a "device" of this type is defined.
 *       Pot modul specific code for define here
 * Info: 
 * Para: Common_Definition *Def -> the new definition for this device, some inits done
 *	 char *Definition -> args?
 * Rets: char* -> error-text-string in allocated mem, or NULL=OK
 *--------------------------------------------------------------------------------------------------
 */



//--------------------------------------------------------------------------------------------------



// typedef for Add_Fn - experimental - provided my module
typedef strTextMultiple_t* (*Add_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition, uint8_t *kvArgs, size_t kvArgsLen);

// typedef for Attribute_Fn - called in case of attribute changes for this definition, to check them
typedef Entry_String_t* (*Attribute_Fn_t)(Entry_Common_Definition_t* p_entry_common_definition, const String_t attr_command, const String_t attr_name, const String_t attr_value);

// typedef for Define_Fn - called to create a new definition of this type
typedef Entry_String_t* (*Define_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

//
typedef int (*Delete_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);		


//
typedef int (*Direct_Read_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

// typedef for Direct_Write_Fn - for 2 stage designs - called to give write job to 1st stage - provided my module
/*typedef strTextMultiple_t* (* Direct_Write_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition_Stage1, Entry_Common_Definition_t *p_entry_common_definition_Stage2, Common_StageXCHG_t *Common_StageXCHG);*/
typedef int (*Direct_Write_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

//
typedef int (*Except_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

//
typedef int (*FingerprintFn_t)(Entry_Common_Definition_t *p_entry_common_definition);

// typedef for Get_Fn - for 2 stage designs - called to get data from this type - provided my module
typedef int (*Get_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition, Entry_Common_Definition_t *sourcep_entry_common_definition, void *X);
//typedef int (*GetFn_t)(Common_StageXCHG_t *Common_StageXCHG);

//
typedef int (*Idle_Cb_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

//
typedef int (*Initialize_Fn_t)(SCDERoot_t *SCDERoot);

//
typedef Entry_String_t* (*Notify_Fn_t)(Entry_Common_Definition_t *p_notifying_entry_common_definition, Entry_Common_Definition_t *p_own_entry_common_definition);

// typedef for Parse_Fn - for 2 stage designs - called to give job data to 2nd stage - provided my module
typedef strTextMultiple_t* (*Parse_Fn_t)(Common_StageXCHG_t *Common_StageXCHG);

//
typedef int (*Read_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

//
typedef int (*Ready_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

// typedef for Rename_Fn - called to inform the definition about its renameing - provided my module
typedef strTextMultiple_t* (*Rename_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition, uint8_t *newName, size_t newNameLen, uint8_t *oldName, size_t oldNameLen);

// typedef for Set_Fn - called to send data to the definition (opposite of Get) - provided my module
typedef strTextMultiple_t* (*Set_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition, uint8_t *setArgs, size_t setArgsLen);

// typedef for Shutdown_Fn - called to do activities before SCDE shuts down - provided my module
typedef strTextMultiple_t* (*Shutdown_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);

// typedef for StateFn - called to set an state for this definition e.g. called from setstate cmd for recovery from save
//typedef strTextMultiple_t* (*StateFn_t)(Entry_Common_Definition_t *p_entry_common_definition, time_t readingTiSt,
// uint8_t *readingName, size_t readingNameLen, uint8_t *readingValue, size_t readingValueLen,
// uint8_t *readingMime, size_t readingMimeLen);

// typedef for State_Fn - called to set an state for this definition.
// Normally called from setstate cmd when recovering states from save from save
typedef Entry_String_t* (*State_Fn_t) (Entry_Common_Definition_t *p_entry_common_definition, const time_t stateTiSt, const String_t stateNameString, const String_t stateValueString, const String_t stateMimeString);

// typedef for Sub_Fn - experimental - provided my module
typedef strTextMultiple_t* (*Sub_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition, uint8_t *kArgs, size_t kArgsLen);

// typedef for Undefine_Fn - called when an definition is deleted, chance to cleanup - provided my module
typedef Entry_String_t* (*Undefine_Fn_t)(Entry_Common_Definition_t *p_entry_common_definition);//, String_t opt_args);		

// typedef for Write_Fn - called to write data to the definition
typedef Entry_String_t* (*Write_Fn_t) (Entry_Common_Definition_t *p_entry_common_definition, String_t data);


/* 
 * Provided by Module (struct)
 * - stores (the common) function callbacks for SCDE module operation
 * - information is sent to SCDE by module, when loaded
 * - done by Initialize_Fn after the module is loaded
 */
struct Provided_By_Module_s {

//  uint8_t name_char[32];		    // Module Name = Type Name, max 32 characters!
//  size_t name_len;
    uint8_t typeName[32];
    size_t typeNameLen;

  Add_Fn_t add_fn;		            //
  Attribute_Fn_t attribute_fn;	    // called in case of attribute changes, to check them
  Define_Fn_t define_fn;		    // called to create a new definition of this type
  Delete_Fn_t delete_fn;		    // deleteDefinitionFn, to cleanup (delete log), called by delete after UndefFn
  Direct_Read_Fn_t direct_read_fn;	// readDirectFn      , called from select loop to read
  Direct_Write_Fn_t direct_write_fn;// writeDirectFn     , called from select loop to write
  Except_Fn_t except_fn;		    //                   , called if the global select reports an except field
  Get_Fn_t get_fn;		            // get some data from this device
  Idle_Cb_Fn_t idle_cb_fn;		    //                   , give module an Idle-Callback to process something
  Initialize_Fn_t initialize_fn;	// initializeModuleFn, set up module for operation (after load)
  Notify_Fn_t notify_fn;		    //                   , call this if some device changed its properties
  Parse_Fn_t parse_fn;		        //                   , Interpret a new message
  Read_Fn_t read_fn;		        //                   , Reading / receiving from a Device
  Ready_Fn_t ready_fn;		        //                   , check for available data, if no FD
  Rename_Fn_t rename_fn;		    // renameDefinitionFn, called to inform the definition about its renameing
  Set_Fn_t set_fn;		            // setDefinitionFn?  , set/activate this device
  Shutdown_Fn_t shutdown_fn;        //                   ,called before shutdown
  State_Fn_t state_fn;		        //                   ,set local info for this device, do not activate anything
  Sub_Fn_t sub_fn;		            //                   ,called when Attribute-Keys owned by this Module are deleted
  Undefine_Fn_t undefine_fn;	    //                   ,clean up (delete timer, close fd), called by delete and rereadcfg
  Write_Fn_t write_fn;		        //
  void* custom_fn;		            // ... provided by this Module (non-standard Fn). For other Modules.
  int sizeof_custom_definition;     //sizeOf..// Size of modul specific definition structure (Entry_Common_Definition_t + X)
};



//--------------------------------------------------------------------------------------------------



/*
 * Entry_Module (struct)
 * - stores information of loaded modules, required for module use and operation
 * - data is associated when module is loaded
 * - done in InitializeFn (after module load)
 */
struct Entry_Module_s {
  STAILQ_ENTRY (Entry_Module_s) entries;	// Link to next loaded Module
  Provided_By_Module_t *provided;			// Ptr to Provided by Module Info
  void* lib_handle;							// Handle to this loaded Module

 // place  Provided  Fn's here direct ?
};



// -------------------------------------------------------------------------------------------------




/* 
 * Entry Common Definition (struct)
 * - stores values for operation of an definition (device), common part,  valid only for the defined
 * - instance of an loaded module. Values are initialized by the SCDE and finalized by the
 *   loaded module itself (defineFn).
 */
struct Entry_Common_Definition_s {
  STAILQ_ENTRY(Entry_Definition_s) entries;	    // link to next definition entry

  LIST_HEAD (Head_Attr_Value_s,                 // constructs the head of an LIST, which holds 
      Entry_Attr_Value_s) head_attr_value;      // multiple linked attribute values

 //? uint32_t* link;						    // link to next struct

  union 
  {
  struct // new
    {
    String_t nname;                              // this definitions 'name'
    };
  struct // old
    {
    uint8_t* name;
    size_t nameLen; 
    };
  };

  union 
  {
  struct // new
    {
    Entry_Module_t* p_entry_module;             // ptr to the module_t (assigned by name when module is loaded)
    };
  struct // old
    {
    Entry_Module_t* module; 
    };
  };

  string_t state_reading_value;                 // the STATE reading is obligatory for every definition !
  time_t state_reading_timestamp;               // SCDE has also a timestamp for STATE

  string_t definition;                          // the arguments that created this definition 
 
  uint32_t nr;							        // An unique sequential number assigned to definition
								                // to rebuild / save in the order of definition-creation

  int fd;							            // File Descriptor, used by selectlist / readyfnlist 
								                // (-1 = not assigned)

  int Common_CtrlRegA; //common_control_register_a		// Common Control Reg A (enum Common_CtrlRegA from WEBIF.h)

  int defCtrlRegA; //common_definition_control_register_a	// Definition Control Reg A (enum Common_DefCtrlRegA)

//  xSemaphoreHandle def_mux; //definition_mux 

  changed_t *p_changed;                         // an helper to process the changed readings in regard
                                                // to the notify creation, in use if NOT NULL

  STAILQ_HEAD (head_readings_s,                 // Constructs the head of an STAILQ,
      entry_reading_s) head_readings;           // which holds multiple linked readings
  
  // Pointer to ActiveResourcesDataA, set at init time.
  void* ActiveResourcesDataA; //active_resource_data_a
  // Pointer to ActiveResourcesDataB, set at init time.
  void* ActiveResourcesDataB; //active_resource_data_b

  /* todo
# Special values in %defs:
# IODev   - attached to io device
# CHANGED - Currently changed attributes of this device. Used by NotifyFn
# VOLATILE- Set if the definition should be saved to the "statefile"
# NOTIFYDEV - if set, the notifyFn will only be called for this device
*/
};

// Information Flags, stored in Common_Definition_s - for Definition Control
enum Common_DefCtrlRegA { //common_definition_control_register_a
//hinzu CDCRA_Flag                    // indicates:
    F_TEMPORARY		         = 1 << 0 // Definition is temporary e.g. WEBIf connection
  , F_VOLATILE		         = 1 << 1 // Definition is volatile -> saved to statefile (do not?)
  , F_IN_TRIGGER	         = 1 << 2 // Definition is in the trigger ->  do not call the triggers from the inner loop.
  , F_SKIP_STATE_FORMAT_EVAL = 1 << 3 // Definitions state-format eval should be skipped
  , CDCRA_F_weitere	         = 1 << 4 //   
};

// Information Flags, stored in Common_Definition_s - for Connection Control
enum Common_CtrlRegA {	//common_control_register_a
//hinzu CCRA_Flag
    F_WANTS_WRITE		= 1 << 0	// indicates: define (FD) wants to write data (call DirectWriteFn), if ready
  , F_WANTS_IDLE_TASK	= 1 << 1	// indicates: define (FD) wants idle-task to be called (call IdleTaskFn)
  , CCRA_F_weitere		= 1 << 2	//   
};



// -------------------------------------------------------------------------------------------------



// oder Common_DirectWriteJob_t
// Common_StageXCHG_t stores values for operation of 2. stage design Modules
typedef struct Common_StageXCHG_s Common_StageXCHG_t;



/* 
 * Common_StageXCHG_s (struct)
 * - stores values for operation of 2. stage design Modules (stage exchange)
 * - the definitions for callbacks
 */
struct Common_StageXCHG_s {

  Entry_Common_Definition_t *definitionStage1;	// ptr to the definition that is 1 st stage
  Entry_Common_Definition_t *definitionStage2;	// ptr to the definition that is 2 nd stage

  int stageCtrlRegA;		// Stage processing Control Register A (enum Common_stageCtrlRegA)
};

// Information Flags, stored in Common_Definition_s - for Connection Control
enum stageCtrlRegA {
    SCRA_F_JOB_DONE			= 1 << 0	// indicates: the job is done
  , SCRA_F_ERROR			= 1 << 1	// indicates: ?
  , SCRA_F_weitere			= 1 << 2	// indicates: ?
};



// -------------------------------------------------------------------------------------------------
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ typedefs and structs for Command operation ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓



// typedef for InitializeCommandFn - called to initialize an command (once called after loading it)
typedef int (* InitializeCommandFn_t)(SCDERoot_t* SCDERoot);

// typedef for CommandFn - when SCDE finds matching command name, this Fn will be called - to exec.
typedef struct headRetMsgMultiple_s (* CommandFn_t) (const uint8_t *args, const size_t argsLen);

// Provided by Command (type) - stores function callbacks and information needed for cmd operation
typedef struct ProvidedByCommand_s ProvidedByCommand_t;


//--------------------------------------------------------------------------------------------------


/* 
 * Provided by Command (struct)
 * - stores function callbacks and information for a modular command
 * - information is associated when the command is loaded
 * - (done in InitializeFn after command load)
 */
struct ProvidedByCommand_s {
  uint8_t commandNameText[32];					//command_name_char		// name of command
  size_t commandNameTextLen; 					//command_name_len

  InitializeCommandFn_t InitializeCommandFn;	// returns module information (module_s)
  CommandFn_t CommandFn;						// the command Fn
  const String_t helpString; 					//help_string	// help text
  const String_t helpDetailString; //help_detail_string	// detailed help text
};

//--------------------------------------------------------------------------------------------------

/* 
 * Entry_Command
 * - stores commands made available for operation.
 * - some are buildin and initialized after start, others are loaded by user
 */
struct Entry_Command_s {
  STAILQ_ENTRY (Entry_Command_s) entries;		// link to next loaded command
  ProvidedByCommand_t* provided; // p_provided	// ptr to provided-by-command Info
  void* lib_handle;								// Handle to this loaded Module

};



// -------------------------------------------------------------------------------------------------


//NAME DOOF! - dient dem speichern von def-spec matching definitions -> dorthin verschieben ?
// forward declaration
typedef struct Entry_Definition_Ptr_s Entry_Definition_Ptr_t;

/*
 * Entry_Definition_Ptr_s (struct) 
 * - is an singly linked tail queue entry and stores the ptr to one Entry_(Common)_Definition_t
 * it is used when storing multiple definitions (in an singly tail linked queue - SLTQ)
 */
struct Entry_Definition_Ptr_s { // is NOT 'common'
  STAILQ_ENTRY(Entry_Definition_Ptr_s) entries;	            // link to next element of the singly linked tail queue
  Entry_Common_Definition_t* p_entry_common_definition;	    // the definition of this element
};

/*
 * Constructor for the singly linked tail queue (head), which can hold multiple linked definitions
 * SLTQ can be used for an FIFO queue by adding entries at the tail and fetching entries from the head
 * SLTQ is inefficient when removing arbitrary elements
 */
STAILQ_HEAD(Head_Definition_Ptr_s, Entry_Definitions_Ptr_s);



// -------------------------------------------------------------------------------------------------
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ typedefs and structs for Attribute operation ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓



// Entry_Attr_Name_t holds the name of an assigned attribute (attr_name). (linked list from queue.h)
typedef struct Entry_Attr_Name_s Entry_Attr_Name_t;

/*
 * Entry_Attr_Name_s (struct)
 * - holds the name of an assigned attribute (attr_name). (linked list from queue.h)
 */
struct Entry_Attr_Name_s {
  LIST_ENTRY(Entry_Attr_Name_s) entries;	// links to next list entries
  String_t attr_name;					    // an assigned Attribute Name
};


// -------------------------------------------------------------------------------------------------


// Entry_Attr_Value_t holds assigned attribute-values. (linked list from queue.h)
typedef struct Entry_Attr_Value_s Entry_Attr_Value_t;

/*
 * Entry_Attr_Value_s (struct)
 * - holds assigned attribute-values. (linked list from queue.h)
 */
struct Entry_Attr_Value_s {
  LIST_ENTRY(Entry_Attr_Value_s) entries;	// links to next list entries
  Entry_Attr_Name_t* p_entry_attr_name;		// ptr to the attribute-name-entry the attribute is assigned to
  String_t attr_value;				// the assigned value MAY BE NULL IF NO VALUE!
};



// -------------------------------------------------------------------------------------------------
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ typedefs and structs for generating notifies ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

/*
 * entry_definition_to_be_notified_s (struct)
 * - holds one definition that should be notified. (linked list from queue.h)
 */
struct entry_definition_to_be_notified_s {
  LIST_ENTRY(entry_definition_to_be_notified_s) entries;	// links to next LIST entries
  Entry_Common_Definition_t *p_entry_common_definition;		// definition that should be notified
};

// -------------------------------------------------------------------------------------------------

/*
 * notify_s
 * - stores the content of an notify
 * - the name of the notify, more optional details as value and an timestamp
 */
typedef struct notify_s {
  time_t timestamp;		// assigned timestamp 
  String_t name;		// notify-name text
  String_t value;		// notify details as text
} notify_t;

// -------------------------------------------------------------------------------------------------

/*
 * entry_notify_s (struct) 
 * - used as an STAILQ entry, to stores one 'notify'
 * - used to store multiple notifies in an STAILQ
 */
struct entry_notify_s {
  STAILQ_ENTRY(entry_notify_s) entries;	  // link to next entries
  notify_t notify;                        // the notify
};

// -------------------------------------------------------------------------------------------------

/* 
 * changed_s (struct)
 * - stores general information to process the changes -> notify creation
 * - stores the pending notifies LIST head
 */
struct changed_s {
  // Constructs the head of an STAILQ, which holds multiple linked notifies
  STAILQ_HEAD (head_notifies_s, entry_notify_s) head_notifies;

  time_t update_timestamp;               	// timestamp of bulk update
};



// -------------------------------------------------------------------------------------------------
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓ typedefs and structs for generating readings ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓



/*
 * reading_s (struct) 
 * - stores the content of an reading
 * - the name of the reading, more optional details as value and an timestamp
 */
typedef struct reading_s {
  time_t time;	//timestamp		// assigned timestamp 
  string_t name;				// reading-name text
  string_t value;				// reading-value as text
} reading_t;

// -------------------------------------------------------------------------------------------------

/*
 * entry_reading_s (struct) 
 * - used as an STAILQ entry, to stores one 'reading'
 * - used to store multiple readings in an STAILQ
 */
struct entry_reading_s {
  STAILQ_ENTRY(entry_reading_s) entries;	// link to next entry in SLTQ
  reading_t reading;                        // the reading
};



// -------------------------------------------------------------------------------------------------













// -------------------------------------------------------------------------------------------------
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓        typedefs and structs of SCDE ROOT       ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓



/* 
 * SCDERoot (struct)
 * - Smart Connected Devices Engine - root data
 */
struct SCDERoot_s {
  // SCDEFn (Functions / callbacks) for faster operation
  SCDEFn_t* SCDEFn; //p_scde_fn


//use vars qw($auth_refresh);
//use vars qw($cmdFromAnalyze);   # used by the warnings-sub
//use vars qw($cvsid);            # used in 98_version.pm

  // current_config_file
  strText_t current_config_file;

  // Is used to store Definitions in the right order. 1st=1
  uint32_t device_count;

  // for version management
  uint32_t feature_level;

  // -> 'enum global_control_register_a'
  uint32_t global_control_register_a;

// to check if requested TiSt is unique (higher than last time)
  time_t last_timestamp;

//use vars qw($fhem_started);     # used for uptime calculation
//use vars qw($init_done);        #
//use vars qw($internal_data);    # FileLog/DbLog -> SVG data transport
//use vars qw($lastDefChange);    # number of last def/attr change
//use vars qw($nextat);           # Time when next timer will be triggered.
//use vars qw($readytimeout);     # Polling interval. UNIX: device search only
//use vars qw($reread_active);
//use vars qw($selectTimestamp);  # used to check last select exit timestamp
//use vars qw($winService);       # the Windows Service object

  // Constructs the head of an LIST, which holds multiple linked attribute names
  LIST_HEAD (Head_Attr_Name_s, Entry_Attr_Name_s) head_attr_name;

  // Link to available commands
  STAILQ_HEAD (stailhead4, Entry_Command_s) head_command;

//use vars qw(%data);             # Hash for user data
//use vars qw(%defaultattr);      # Default attributes, used by FHEM2FHEM

// Link to Definitions (a.k.a. devices ...)
  STAILQ_HEAD (stailhead2, Entry_Definition_s) head_definition;

//use vars qw(%inform);           # Used by telnet_ActivateInform
//use vars qw(%intAt);            # Internal at timer hash, global for benchmark
//use vars qw(%logInform);        # Used by FHEMWEB/Event-Monitor

  // STAILQ with loaded Modules
  STAILQ_HEAD (stailhead1, Entry_Module_s) head_module;

  // Constructs the head of an LIST, which holds multiple linked definitions that should be notified
  LIST_HEAD(head_definitions_to_be_notified_s,
      entry_definition_to_be_notified_s) head_definitions_to_be_notified;

//use vars qw(%oldvalue);         # Old values, see commandref.html
//use vars qw(%readyfnlist);      # devices which want a "readyfn"
//use vars qw(%selectlist);       # devices which want a "select"
//use vars qw(%value);            # Current values, see commandref.html
//use vars qw(@authenticate);     # List of authentication devices
//use vars qw(@authorize);        # List of authorization devices
//use vars qw(@structChangeHist); # Contains the last 10 structural changes
};


// global control register a - holds global flags - e.g. for connection control
enum global_control_register_a {
//NEU: GCRA_F lag ändern
  F_RECEIVED_QUIT		            = 1 << 0, // indicates: quit the include processing
  F_INIT_DONE			            = 1 << 1, // indicates: the SCDE has fiinished the init process
  F_READINGS_UPDATE_DELAY_TRIGGER	= 1 << 2, // indicates: delayed processing of readings ?
  GCRA_F_weitere		= 1 << 3	//   
};

// the global stuff from the SCDE-Core Fn - here???
//#include "argparse_s.h"




#endif /*_SCDE_S_H_*/
