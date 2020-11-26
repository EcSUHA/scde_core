#include "ProjectConfig.h"

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#endif

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

#include "SCDE_s.h"

#include "SCDE.h"



/* --------------------------------------------------------------------------------------------------
 *  FName: AnalyzeCommand
 *  Desc: Seeks command and commandArgs from given args and calls the commandFn from an matching 
 *        command-name (if loaded / available)
 *  Info: executes reloadcommand if command is not found
 *  Para: const String_t args  -> space seperated "command args" text
 *  Rets: struct Head_String_s -> head from STAILQ, stores multiple RetMsg (errors), NULL=OK/None
 * --------------------------------------------------------------------------------------------------
 */
struct Head_String_s
AnalyzeCommand(const String_t args)
{

  #if CORE_SCDE_DBG >= 7
  Log("AnalyzeCommand",7,"Fn analyzing cmd '%.*s'."
 	,args.len
	,args.p_char);
  #endif

// --------------------------------------------------------------------------------------------------

  // prepare STAILQ head to store multiple 'ret_msg' elements
  struct Head_String_s head_ret_msg;

  // Initialize the queue
  STAILQ_INIT(&head_ret_msg);

// --------------------------------------------------------------------------------------------------

  // expected argument #1
  String_t cmd_name;

  // set * to start of possible command-name text (seek-start-pos)
  cmd_name.p_char = args.p_char;

  // the total seek-counter
  int i = 0;
  
  // an element seek-counter
  int j = 0;
  
  // seek * to start of command-name text ('\32' after space)
  while( ( i < args.len ) && ( *cmd_name.p_char == ' ' ) ) { i++ ; cmd_name.p_char++ ; }

  // 1 @ finnished

  // expected argument #2
  String_t cmd_args;

  // set * to start of possible command-args text (seek-start-pos)
  cmd_args.p_char = cmd_name.p_char;

  // seek to next space '\32'
  while( ( i < args.len ) && ( *cmd_args.p_char != ' ' ) ) { i++, j++ ; cmd_args.p_char++ ; }

  // length of command-name text determined
  cmd_name.len = j;

  // seek * to start of command-args text ('\32' after space)
  while( ( i < args.len ) && ( *cmd_args.p_char == ' ' ) ) { i++ ; cmd_args.p_char++ ; }

 // 2 @ finnished - no further processing of rest !
  /*
  // no further arguments expected - searching the end of text
  String_t end_of_text;
	
  // set start * of possible 'end of text' seek-start-pos
  end_of_text.p_char = cmd_args.p_char;
	
  // clear element seek-counter
  j = 0;

  // seek to next space '\32'
  while( ( i < args.len ) && ( *end_of_text.p_char != ' ' ) ) { i++ , j++ ; end_of_text.p_char++ ; }
*/
  // length of command-args text determined
  cmd_args.len = args.len - i ;

  // @ 'p_end_of_text' ! No further processing ...

// -------------------------------------------------------------------------------------------------

  // verify lengths > 0, ! cmd_args = 0 IS ALLOWED!
  if ( cmd_name.len == 0 ) {

	// alloc mem for retMsg
	Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	// response with error text
	p_entry_ret_msg->string.len = asprintf((char**) &p_entry_ret_msg->string.p_char
		,"Could not interpret input '%.*s' ! Usage: <command> <command dependent arguments>"
		,(int) args.len
		,args.p_char);

	// insert ret_msg as entry in stail-queue
	STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	// return head of singly linked tail queue, which holds 'ret_msg' elements
    return 	head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

  // get the Command by Name
  Entry_Command_t *entry_command;

  // search for the command
  STAILQ_FOREACH(entry_command, &SCDERoot.headCommands, entries) {

	if ( (entry_command->provided->commandNameTextLen == cmd_name.len)
		&& (!strncasecmp((const char*) entry_command->provided->commandNameText, (const char*) cmd_name.p_char, cmd_name.len)) ) {

		// call the CommandFn, if retMsg != NULL -> error ret Msg
//		struct Head_String_s head_ret_msg_from_fn =
		struct headRetMsgMultiple_s headRetMsgMultipleFromFn =
			entry_command->provided->CommandFn(cmd_args.p_char, cmd_args.len);

// temp conversion start
  struct Head_String_s head_ret_msg_from_fn;
  STAILQ_INIT(&head_ret_msg_from_fn);
  head_ret_msg_from_fn.stqh_first =  headRetMsgMultipleFromFn.stqh_first;
  head_ret_msg_from_fn.stqh_last =  headRetMsgMultipleFromFn.stqh_last;
// temp conversion end

		// add the retMsg entrys
		STAILQ_CONCAT(&head_ret_msg, &head_ret_msg_from_fn);  
		
  		// return STAILQ head, it stores multiple RetMsg, if NULL -> no RetMsg-entries
		return head_ret_msg;
	}
  }

// -------------------------------------------------------------------------------------------------

 // alloc mem for retMsg
  Entry_String_t* p_entry_ret_msg =
	malloc(sizeof(Entry_String_t));

  // response with error text
  p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char
	,"Unknown command <%.*s> with arguments <%.*s>!"
	,(int) cmd_name.len
	,cmd_name.p_char
	,(int) cmd_args.len
	,cmd_args.p_char);

  // insert ret_msg as entry in stail-queue
  STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  // return head of singly linked tail queue, which holds 'ret_msg' elements
  return head_ret_msg;
}



