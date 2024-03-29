#ifndef _SCDE_H_
#define _SCDE_H_

// -------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef CORE_SCDE_DBG  
#define CORE_SCDE_DBG  5	// 5 is default
#endif 

// set default build verbose - if no external override
#ifndef Helpers_SCDE_DBG  
#define Helpers_SCDE_DBG  5	// 5 is default
#endif 

// -------------------------------------------------------------------------------------------------


// stores the Root Data of the Smart Connected Devices Engine
SCDERoot_t SCDERoot;

// stores SCDEFn (Functions / callbacks) for operation (provided / made accessible for modules)
SCDEFn_t SCDEFn;


// inits the Root Data of the Smart Connected Devices Engine
void InitSCDERoot(void);

// ?
void InitA();


// -------------------------------------------------------------------------------------------------



/*
 * Functions provided by SCDE (Fn callbacks) 
 * AND made accessible for calling by modules and commands (for SCDE operation and helpers )
 */


// adds an event as entry_event_t to the changed-readings List
void Add_Event_Fn(Entry_Common_Definition_t *p_entry_common_definition, reading2_t *reading);

// adds zero terminated query at given request-head buffer write position, with extras
void Add_Query_Encoded(char *p_query_buf, char *p_req_head_buf);

// analyzes + processes one command row
//struct headRetMsgMultiple_s AnalyzeCommand(const uint8_t *args, const size_t argsLen);
struct Head_String_s AnalyzeCommand(const String_t args);

// analyzes + processes an configuration file
//struct headRetMsgMultiple_s AnalyzeCommandChain(const uint8_t *args, const size_t argsLen);
struct Head_String_s AnalyzeCommandChain(const String_t args);

//
int CallGetFnByDefName(const uint8_t *nameText, const size_t nameTextLen, Common_Definition_t *sourceCommon_Definition, void *X);

// Initially loads or executes a reload of an Module of given type-name
Module_t* CommandReloadModule(const String_t type_name);

// creates the notify list (should be empty when calling!)
void Create_Notify_List_Fn();

// creates an new reading in calling definition
reading2_t * Create_Reading_Fn(entry_common_definition_t *p_entry_common_definition, char *p_name, char *p_initial_value_as_text, char *p_initial_custom_unit_text, char *p_r_provided_by_module, char *template_name);

// returns all definitions that match 'def-spec'
struct Head_String_s Devspec2Array(const String_t devspecString);
//struct Head_String_s Definition_Specifications_2_List(const String_t def_spec);

// deletes the notify list (if any)
void Delete_Notify_List_Fn();

//
Entry_String_t* Do_Trigger_Fn(Entry_Common_Definition_t *p_entry_common_definition, char *p_changed_state);

// evaluates the state-reading embedded in each definition
void Eval_State_Format_Fn(Entry_Common_Definition_t *p_entry_common_definition);

// returns formated text of Time from tist
strText_t FmtTime(time_t tiSt);

// returns text of attr val
char * Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn(const string_t *p_def_name, const char *p_attr_name);

// returns all readings of the definition as setreading args-text
struct head_string_s Get_All_Readings_Fn(Entry_Common_Definition_t *p_entry_common_definition);

// returns an SLTQ-head, containig the definition line and the attribute lines as entrys
struct head_string_s Get_Def_And_Attr_Fn(Entry_Definition_t* p_entry_definition);

//
Common_Definition_t * Get_Ptr_To_Definition_By_Name_Fn(const String_t definition_name);

// Returns a STAILQ head that stores entries of all 'dev_spec' matching definitions
struct Head_Definition_Ptr_s Get_Definitions_That_Match_DefSpec_String(const String_t dev_spec);

// returns formated text of Date-Time from tist
string_t Get_Formated_Date_Time_Fn(time_t timestamp);

//
Module_t* GetLoadedModulePtrByName(const uint8_t *typeName, const size_t typeNameLen);

// returns current SCDE Time Stamp
time_t GetTiSt(void);

// returns an UNIQUE SCDE Time Stamp (in most cases the current Time Stamp)
time_t GetUniqueTiSt(void);

// checks the given Device Name for device name rules
bool GoodDeviceName(const String_t nameString);

// checks the given Reading Name for device name rules
bool GoodReadingName(const String_t nameString);

// prints data as Hex-Dump to debug terminal
void HexDumpOut (char *desc, void *addr, int len);

// the log Fn for the core elements
void Log(const char *internal_fn, const uint8_t log_level, const char *format, ...);

// the log Fn for modular elements (Modules & Commands)
void Log3(const uint8_t *p_name_char, const size_t name_len, const uint8_t log_level, const char *format, ...);

//
void Log4(char *text);

// corrects the given Device Name
void MakeDeviceName(const String_t nameString);

// corrects the given Reading Name
void MakeReadingName(const String_t nameString);

// call this before updating readings
time_t Readings_Begin_Update_Fn(Entry_Common_Definition_t *p_entry_common_definition);

// call this to add an Reading to the running update of Readings
int Readings_Bulk_Update_Fn(Entry_Common_Definition_t *p_entry_common_definition, reading2_t *reading, const bool changed, time_t timestamp);
    
// call this to after bulk-update to process readings
int Readings_End_Update_Fn(Common_Definition_t *Common_Definition, bool doTrigger);

// removes definitions from the notify list. Faster then generating new list.
void Remove_From_Notify_List_Fn(Entry_Definition_t *p_entry_common_definition);

// Sets an reading with value and update time in given definition
void Set_Readings_Value_Fn(Entry_Common_Definition_t *p_entry_common_definition, const String_t reading_name, const String_t reading_value, const time_t timestamp);

// reads number from sting to float_t at res
bool Str_To_Float(const char *str, float *res);

// reads number from sting to uint16_t at res
bool Str_To_Uint16(const char *str, uint16_t *res);

// reads number from sting to uint32_t at res
bool Str_To_Uint32(const char *str, uint32_t *res);

// The argument parser
  // Argument Parser - Split Arguments To Allocated Mememory Fn
  char ** ArgParse_SplitArgsToAllocatedMem(int *argc, uint8_t *argsText,  size_t argsTextLen);

  // Argument Parser - Split URL Encoded Arguments To Allocated Memory Fn
  char ** ArgParse_SplitURLEncodedArgsToAllocatedMem(int *argc ,char *argsText);

  // Argument Parser - Free Splitted Arguments In Allocated Memory Fn
  void ArgParse_FreeSplittedArgsInAllocatedMem(char **argv);

  // Argument Parser - Init Fn
  Entry_String_t* ArgParse_Init(struct argparse *self, struct argparse_option *options,
                  const char *const *usages, int flags, const char *description, const char *epilog);

  // Argument Parser - Parse Fn
  Entry_String_t* ArgParse_Parse(struct argparse *self, int argc, const char **argv);

  // Argument Parser - Usage Fn
  void ArgParse_Usage(struct argparse *self);

  // Argument Parser - Prepare Leadin Error Message Fn
  void ArgParse_PrepareLeadinErrorMsg(struct argparse *self, const struct argparse_option *opt,
               const char *reason, int flags);


// returns current system time. (IT IS NOT AN TIME STAMP)
time_t TimeNow();

//
struct headRetMsgMultiple_s WriteStatefile();



// -------------------------------------------------------------------------------------------------



/*
 * Functions provided by SCDE (Fn callbacks) 
 * AND NOT made accessible for calling by modules and commands (internal Fn)
 */

//
Module_t* CommandActivateModule (ProvidedByModule_t* ProvidedByNEWModule);

//
Command_t* CommandActivateCommand (ProvidedByCommand_t* providedByNEWCommand);

//
int CommandLoadCommand(const uint8_t *commandTxt, const size_t commandTxtLen, CommandFn_t CommandFn, const uint8_t *commandHelp, const size_t commandHelpLen);


// internal Fn, but made available to Modules and Commands

//deleted Common_Definition_t* AddDefineForModuleWithName(Module_t* ModulePtr, const char* Name);

//Common_Definition_t*      AddDefineForModuleWithName(Module_t* ModulePtr, char* Name);




//char* CommandDefine (const uint8_t *args, const size_t argsLen);
//char* CommandSet (const uint8_t *args, const size_t argsLen);
strTextMultiple_t*          CommandUndefine (const uint8_t *args, const size_t argsLen);
// Attribute Management
char*                       GetAttribute(char* Definition, char* AttributeName);
bool                        AssignAttribute(char* Definition, char* AttributeName, char* Attribute);
bool                        DeleteAttribute(char* Definition, char* AttributeName);

// Initializes the global device
void doGlobalDef(const uint8_t *cfgFileName, const size_t cfgFileNameLen);






// prints data as Hex-Dump to debug terminal
// doppeltvoid HexDumpOut (char *desc, void *addr, int len);

parsedKVInputArgs_t* ParseKVInputArgs(int numImplementedKeys, const kvParseImplementedKeys_t *XX_IK, const uint8_t *kvArgsText, const size_t kvArgsTextLen);




#endif /*_SCDE_H_*/
