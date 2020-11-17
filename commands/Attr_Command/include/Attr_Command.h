// SCDE-Command Attr

#ifndef ATTR_COMMAND_H
#define ATTR_COMMAND_H



#include "SCDE_s.h"



int Attr_InitializeCommandFn(SCDERoot_t* SCDERootptr);

struct headRetMsgMultiple_s Attr_CommandFn (const uint8_t* p_args ,const size_t args_len);
struct Head_String_s Attr_Command2Fn(const String_t args);


#endif /*ATTR_COMMAND_H*/
