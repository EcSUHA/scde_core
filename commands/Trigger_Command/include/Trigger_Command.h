// SCDE-Command Trigger

#ifndef TRIGGER_COMMAND_H
#define TRIGGER_COMMAND_H



#include "SCDE_s.h"



int Trigger_Initialize_Command_Fn(SCDERoot_t* SCDERootptr);

struct Head_String_s Trigger_Command_Fn(const String_t args);


#endif /*TRIGGER_COMMAND_H*/
