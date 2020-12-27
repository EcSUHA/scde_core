// SCDE-Command Deleteattr

#ifndef DELETEATTR_COMMAND_H
#define DELETEATTR_COMMAND_H



#include "SCDE_s.h"



int Deleteattr_Initialize_Command_Fn(SCDERoot_t* SCDERootptr);

struct Head_String_s Deleteattr_Command_Fn(const String_t args);


#endif /*DELETEATTR_COMMAND_H*/
