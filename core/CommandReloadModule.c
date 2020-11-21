#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "SCDE_s.h"

#include "SCDE.h"











#include <dlfcn.h>


#define LIBEQUAL "MusterModul.so"

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
*
/

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
  Log("CommandReloadModule",1,"Error!, platform has no support for loading Type-Name '%.*s'."
 	,type_name.len
	,type_name.p_char);
  #endif
    
  return NewModule;
  
#else// LINUX_PLATFORM

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
	
  if (lib_handle == NULL) {
  
    #if CORE_SCDE_DBG >= 1
    Log("CommandReloadModule",1,"Error!, loading Type-Name '%.*s' failed, %s."
 		,type_name.len
		,type_name.p_char
		,dlerror() );
    #endif
		
	// free the constructed file-name
	free(file_name);

	// return with result: not found
	return NULL;
  }
	
  // free the constructed file-name
  free(file_name);
  
// ---
  
  // Get ptr to ProvidedByModule_t. We need the function callbacks for module operation
  ProvidedByModule_t* ProvidedByNEWModule;
	
  ProvidedByNEWModule = dlsym(lib_handle, "Telnet_ProvidedByModule");

  // failed to get ?
  if (!ProvidedByNEWModule ) {
  
	  Log("HCTRL",16,"ProvidedByModule not found!/n");
  
      // unload lib here
  
	return NULL;
  }




/*
  if (LoadedModule == NULL)

		{

		printf ("Fehler bei dlsym() kiadmodule: %s\n", dlerror ());
		exit (EXIT_FAILURE);

		}
*/

 


  // call the Initialize Function
  ProvidedByNEWModule->InitializeFn(&SCDERoot);

  // store the Module provided Fn and handle 
  Module_t* NewModule;
  NewModule = (Module_t*) malloc(sizeof(Module_t));
  NewModule->provided = ProvidedByNEWModule;
  NewModule->lib_handle = lib_handle;
  STAILQ_INSERT_HEAD(&SCDERoot.HeadModules, NewModule, entries);

  Log("HCTRL",16,"Module initializzed/n");


/*
  Log3((const uint8_t*) Name
	,sizeof(Name)
	,1
	,"CommandAttr called. Creating for definition <%s> a new AttrName <%s> with AttrValue <%s>"
	,Name
	,AttrName
	,AttrValue);
*/

/*
 // debug: list currently stored modules
  Module_t *Module;
  STAILQ_FOREACH(Module, &SCDERoot.HeadModules, entries) {
	printf("LM Name:\"%.*s\"\n"
		,Module->provided->typeNameLen
		,Module->provided->typeName);
  }
*/

  return NewModule;
#endif // LINUX_PLATFORM
}

