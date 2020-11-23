#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

//#if defined(LINUX_PLATFORM)
#include <dlfcn.h>
//#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "SCDE_s.h"

#include "SCDE.h"














//

/* dynamische Bibliothek laden */
/*
static void *
my_load_dyn (const char *lib)
{
  static void *handle;

  handle = dlopen(LIBEQUAL, RTLD_NOW);

  if (handle == NULL)
	{
	printf ("Fehler bei dlopen(): %s\n", dlerror ());
	exit (EXIT_FAILURE);
	}

  return handle;
}
*/





/* Funktion aus der dynamischen Bibliothek laden */
/*
static void *
my_load_func (void *handle, const char *func)
{
  void *funcptr = dlsym (handle, func);

  if (funcptr == NULL)
	{
	printf ("Fehler bei dlsym(): %s\n", dlerror ());
	exit (EXIT_FAILURE);
	}

  return funcptr;
}
*/


/* Speicher wieder freigeben */
/*
static void
my_close_func (void *handle)
{
  if (dlclose (handle))
	printf ("Fehler bei dlclose(): %s\n", dlerror ());
}
*/

//http://stackoverflow.com/questions/17081131/how-to-call-a-function-in-the-main-program-from-a-dynamically-loaded-shared-libr
























/* Category: SCDE Core Fn
 * --------------------------------------------------------------------------------------------------
 *  FName: CommandReloadModule
 *  Desc: Initially loads or executes a reload of an Module of given type-name
 *  Info:  
 *  Para: const String_t type_name -> Modules type-name that should be re?-loaded 
 *  Rets: Module_t* -> Pointer to loaded Module / NULL on error, not loaded
 *--------------------------------------------------------------------------------------------------
 */
Module_t*
CommandReloadModule(const String_t type_name)
{

#if defined(ESP_PLATFORM)

  #if CORE_SCDE_DBG >= 1
  Log("CommandReloadModule",1,"Error!, platform has no support for loading Type-Name '%.*s'. It should be linked into image by the maker."
 	,type_name.len
	,type_name.p_char);
  #endif
    
  return NULL;
  
//#endif
   
//#if defined(LINUX_PLATFORM)// LINUX_PLATFORM
#else

  // build lib.so filename
  char *file_name;
  asprintf(&file_name
		,"/home/maikschulze/LINUX/LINUX_Device/build/release/main/lib/modules/Telnet_Module/lib%.*s_Module.so"
		,type_name.len
		,type_name.p_char);
		
  #if CORE_SCDE_DBG >= 7
  Log("CommandReloadModule",7,"(Re-)Loading Module with Type-Name '%.*s'."
  	,type_name.len
	,type_name.p_char);
  #endif
		
  // handle for the lib
  void *lib_handle;

  // open the module (lib) 
  lib_handle = dlopen(file_name, RTLD_NOW);
  
  // free the constructed file-name
  free(file_name);
		
  if (lib_handle == NULL) {
  
    #if CORE_SCDE_DBG >= 1
    Log("CommandReloadModule",1,"Error!, loading Type-Name '%.*s' failed, %s."
 		,type_name.len
		,type_name.p_char
		,dlerror() );
    #endif

	// return with result: not found
	return NULL;
  }
   
// ---
  
  // build ProvidedByModule symbol-filename
  char *symbol_name;
  asprintf(&symbol_name
		,"%.*s_ProvidedByModule"
		,type_name.len
		,type_name.p_char);
  
  // Get ptr to ProvidedByModule_t. We need the function callbacks for module operation
  ProvidedByModule_t* provided_by_loaded_module;
	
  provided_by_loaded_module = dlsym(lib_handle, symbol_name);

  // free the constructed symbol-name
  free(symbol_name);

  // failed to get ?
  if (!provided_by_loaded_module ) {
  
    #if CORE_SCDE_DBG >= 1
    Log("CommandReloadModule",1,"Error!, accessing Type-Name '%.*s' failed, %s."
 		,type_name.len
		,type_name.p_char
		,dlerror() );
    #endif

    // unload lib to clean up
    if (dlclose(lib_handle)) {
   
      #if CORE_SCDE_DBG >= 1
      Log("CommandReloadModule",1,"Error!, closing Type-Name '%.*s' failed, %s."
 		,type_name.len
		,type_name.p_char
		,dlerror() );
      #endif
    }
     
  	// return with result: not found
	return NULL;
  }

// ---

  // call loaded modules initialize function
  provided_by_loaded_module->InitializeFn(&SCDERoot);

  // store the Module (provided Fn's, and the lib handle for possible clean up)
  Module_t* new_module;
  
  new_module = (Module_t*) malloc(sizeof(Module_t));
  new_module->provided = provided_by_loaded_module;
  new_module->lib_handle = lib_handle;
  STAILQ_INSERT_HEAD(&SCDERoot.HeadModules, new_module, entries);


  #if CORE_SCDE_DBG >= 5
  Log("CommandReloadModule",5,"Loaded Module, Type-Name '%.*s', is now useable."
  	,new_module->provided->typeNameLen
	,new_module->provided->typeName);
  #endif

  return new_module;
#endif // LINUX_PLATFORM
}

