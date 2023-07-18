#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#define _GNU_SOURCE         /* See feature_test_macros(7) */

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

#include "SCDE_s.h"

#include "SCDE.h"

















//#include <time.h>



//#include <string.h>

//#include <logging.h>




//LOG_TAG("SCDE");

/*
 * DName: SCDERoot
 * Desc: Stores the Root Data for Smart Connected Devices Engine operation
 * Data: 
 */
SCDERoot_t SCDERoot;



/*
 * DName: SCDEFn (SCDE Functions)
 * Desc: Stores Function callbacks provided & made accessible for modules and commands
 *       for operation and helpers
 * Data: 
 */
SCDEFn_t SCDEFn = {
   Add_Query_Encoded
  ,AnalyzeCommand
  ,AnalyzeCommandChain
  ,CallGetFnByDefName
  ,CommandReloadModule
  ,Create_Reading_Fn
  ,Devspec2Array
  ,Do_Trigger_Fn
  ,FmtTime
  ,Get_All_Readings_Fn
  ,Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn
  ,Get_Def_And_Attr_Fn
  ,Get_Ptr_To_Definition_By_Name_Fn
  ,Get_Definitions_That_Match_DefSpec_String
  ,Get_Formated_Date_Time_Fn
  ,GetLoadedModulePtrByName
  ,GetTiSt
  ,GetUniqueTiSt
  ,GoodDeviceName
  ,GoodReadingName
  ,HexDumpOut
  ,Log 
  ,Log3
  ,Log4
  ,MakeDeviceName
  ,MakeReadingName
  ,Readings_Begin_Update_Fn
  ,Readings_Bulk_Update_Fn
  ,Readings_End_Update_Fn
  ,Str_To_Float
  ,Str_To_Uint16
  ,Str_To_Uint32
  
  // buid with : Argument Parser helpers ?
  ,ArgParse_SplitArgsToAllocatedMem
  ,ArgParse_SplitURLEncodedArgsToAllocatedMem
  ,ArgParse_FreeSplittedArgsInAllocatedMem
  ,ArgParse_Init
  ,ArgParse_Parse
  ,ArgParse_Usage
  ,ArgParse_PrepareLeadinErrorMsg

  ,TimeNow
  ,WriteStatefile

// not final
  ,ParseKVInputArgs
};



