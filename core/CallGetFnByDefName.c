#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

#include "SCDE_s.h"

#include "SCDE.h"



/* --------------------------------------------------------------------------------------------------
 *  FName: CallGetFnByDefName
 *  Desc: xxx Creates a new Define with name "Name", and Module "TypeName" and calls Modules DefFn with
 *        args "Args"
 *  Info: 'Name' is custom definition name [azAZ09._] char[31]
 *        'TypeName' is module name
 *        'Definition+X' is passed to modules DefineFn, and stored in Definition->Definition
 *  Para: const uint8_t *args  -> prt to space seperated command text string "Command ..."
 *        const size_t argsLen -> length of args
 *  Rets: struct headRetMsgMultiple_s -> head from STAILQ, stores multiple RetMsg (errors), NULL=OK
 * --------------------------------------------------------------------------------------------------
 */
int
CallGetFnByDefName(const uint8_t *nameText
	,const size_t nameTextLen
	,Common_Definition_t *sourceCommon_Definition
	, void *X)
{
  int retInt = 0;

  // get the Common_Definition by Name
  Common_Definition_t *Common_Definition;

  STAILQ_FOREACH(Common_Definition, &SCDERoot.HeadCommon_Definitions, entries) {

      if ( (Common_Definition->nname.len == nameTextLen)
          && (!strncasecmp((const char*) Common_Definition->nname.p_char,
		(const char*) nameText, nameTextLen)) ) {

              if (Common_Definition->module->provided->get_fn) {

                  retInt = Common_Definition->module->provided->get_fn(Common_Definition,
			sourceCommon_Definition, X);
              }

              else {


              }
      }
  }

  return retInt;
}
