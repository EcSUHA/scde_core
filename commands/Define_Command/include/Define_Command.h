// Command Define for SCDE

#ifndef DEFINE_COMMAND_H
#define DEFINE_COMMAND_H


/*
 *  SCDE Typedefs and Structures
 */
#include "SCDE_s.h"


// the initialize Fn
int Define_InitializeCommand_Fn(SCDERoot_t* SCDERootptr);

// the main command Fn
struct Head_String_s Define_Command_Fn(const String_t args);


#endif /*DEFINE_COMMAND_H*/
