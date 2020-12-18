// Command Delete for SCDE

#ifndef DELETE_COMMAND_H
#define DELETE_COMMAND_H


/*
 *  SCDE Typedefs and Structures
 */
#include "SCDE_s.h"



// the initialize Fn
int Delete_InitializeCommand_Fn(SCDERoot_t* SCDERootptr);

// the main command Fn
struct Head_String_s Delete_Command_Fn(const String_t args);



#endif /*DELETE_COMMAND_H*/
