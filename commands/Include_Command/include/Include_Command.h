// SCDE-Command Include

#ifndef INCLUDE_COMMAND_H
#define INCLUDE_COMMAND_H



#include "SCDE_s.h"



int Include_InitializeCommandFn(SCDERoot_t* SCDERootptr);

struct headRetMsgMultiple_s Include_CommandFn (const uint8_t* p_args ,const size_t args_len);
struct Head_String_s Include_Command2Fn(const String_t args);


#endif /*INCLUDE_COMMAND_H*/
