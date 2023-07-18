// SCDE-Command Attr

#ifndef ATTR_COMMAND_H
#define ATTR_COMMAND_H


/*
 *  SCDE Typedefs and Structures
 */
#include "SCDE_s.h"


// the initialize Fn
int Attr_Initialize_Command_Fn(SCDERoot_t * SCDERootptr);

// the main command Fn
struct Head_String_s Attr_Command_Fn(const String_t args);


#endif /*ATTR_COMMAND_H*/
