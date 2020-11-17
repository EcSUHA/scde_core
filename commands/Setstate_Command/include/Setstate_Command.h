﻿// SCDE-Command Setstate

#ifndef SETSTATE_COMMAND_H
#define SETSTATE_COMMAND_H



#include "SCDE_s.h"



int Setstate_InitializeCommandFn(SCDERoot_t* SCDERootptr);

struct headRetMsgMultiple_s Setstate_CommandFn (const uint8_t* args ,const size_t argsLen);
struct Head_String_s Setstate_Command2Fn(const String_t args);


#endif /*SETSTATE_COMMAND_H*/
