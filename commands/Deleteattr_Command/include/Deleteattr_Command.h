﻿// SCDE-Command Deleteattr

#ifndef DELETEATTR_COMMAND_H
#define DELETEATTR_COMMAND_H



#include "SCDE_s.h"



int Deleteattr_InitializeCommandFn(SCDERoot_t* SCDERootptr);

//struct headRetMsgMultiple_s Deleteattr_CommandFn (const uint8_t *args, const size_t argsLen);

struct headRetMsgMultiple_s Deleteattr_CommandFn (const uint8_t* args ,const size_t argsLen);
struct Head_String_s Deleteattr_Command2Fn(const String_t args);


#endif /*DELETEATTR_COMMAND_H*/
