// SCDE-Command Attr

#ifndef ATTR_COMMAND_H
#define ATTR_COMMAND_H



#include "SCDE_s.h"



int Attr_Initialize_Command_Fn(SCDERoot_t* SCDERootptr);

struct Head_String_s Attr_Command_Fn(const String_t args);


#endif /*ATTR_COMMAND_H*/
