// SCDE-Command Set

#ifndef SET_COMMAND_H
#define SET_COMMAND_H


/*
 *  SCDE Typedefs and Structures
 */
#include "SCDE_s.h"



// the initialize Fn
int Set_Initialize_Command(SCDERoot_t * SCDERootptr);

// the main command Fn
struct head_string_s Set_Command(const uint8_t *args, const size_t argsLen);



#endif /*SET_COMMAND_H*/
