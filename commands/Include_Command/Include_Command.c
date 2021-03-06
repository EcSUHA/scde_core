﻿/* #################################################################################################
 *
 *  Function: Include Command for SCDE (Smart Connected Device Engine)
 *
 *  ESP 8266EX & ESP32 SOC Activities ...
 *  Copyright by EcSUHA
 *
 *  Created by Maik Schulze, Sandfuhren 4, 38448 Wolfsburg, Germany for EcSUHA.de 
 *
 *  MSchulze780@GMAIL.COM
 *  EcSUHA - ECONOMIC SURVEILLANCE AND HOME AUTOMATION - WWW.EcSUHA.DE
 * #################################################################################################
 */



#include <ProjectConfig.h>

#if defined(ESP_PLATFORM)
#include <esp8266.h>
#include <Platform.h>
#endif // ESP_PLATFORM

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SCDE_s.h>

#include "Include_Command.h"


// -------------------------------------------------------------------------------------------------

// set default build verbose - if no external override
#ifndef Include_Command_DBG  
#define Include_Command_DBG  5	// 5 is default
#endif

// -------------------------------------------------------------------------------------------------

// make data root locally available
static SCDERoot_t* SCDERoot;

// make locally available from data-root: the SCDEFn (Functions / callbacks) for operation
static SCDEFn_t* SCDEFn;

// -------------------------------------------------------------------------------------------------



/*
 * -------------------------------------------------------------------------------------------------
 *  DName: Include_Command
 *  Desc: Data 'Provided By Command' for the Include Command (functions + infos this command provides
 *        to SCDE)
 *  Data: 
 * -------------------------------------------------------------------------------------------------
 */

// Command Help
const uint8_t Include_helpText[] =
  "Usage: Include <filename>, to read the commands from <filenname>";
// CommandHelp (detailed)
const uint8_t Include_helpDetailText[] =
  "Usagebwrebwerb: Include <filename>, to read the commands from <filenname>";


ProvidedByCommand_t Include_ProvidedByCommand = {
  "Include",			// command-name text -> libfilename.so !
  7,				// command-name text length
  Include_InitializeCommandFn,	// Initialize Fn
  Include_CommandFn,		// the Fn code
  { &Include_helpText, sizeof(Include_helpText) },
  { &Include_helpDetailText, sizeof(Include_helpDetailText) }
};



/* --------------------------------------------------------------------------------------------------
 *  FName: Include_InitializeCommandFn
 *  Desc: Initializion of SCDE Function Callback of an new loaded command
 *  Info: Stores Command-Information (Function Callbacks) to SCDE-Root
 *  Para: SCDERoot_t* SCDERootptr -> ptr to SCDE Data Root
 *  Rets: ? unused
 *--------------------------------------------------------------------------------------------------
 */
int 
Include_InitializeCommandFn(SCDERoot_t* SCDERootptr)
{
  // make data root locally available
  SCDERoot = SCDERootptr;

  // make locally available from data-root: SCDEFn (Functions / callbacks) for faster operation
  SCDEFn = SCDERootptr->SCDEFn;

// --------------------------------------------------------------------------------------------------

  #if Include_Command_DBG >= 3
  SCDEFn->Log3Fn(Include_ProvidedByCommand.commandNameText
	,Include_ProvidedByCommand.commandNameTextLen
	,3
	,"InitializeFn called. Now useable.");
  #endif

// --------------------------------------------------------------------------------------------------

  return 0;
}




// conversion to V2
struct headRetMsgMultiple_s
Include_CommandFn (const uint8_t* p_args
		,const size_t args_len)
{
  // temporary conversion to make ready -> const String_t args
  String_t args;
  args.p_char = p_args;
  args.len = args_len;


// temporary conversion to make ready ->  head_ret_msg
  struct Head_String_s head_ret_msg
  	 = Include_Command2Fn(args);


  struct headRetMsgMultiple_s x;
  STAILQ_INIT(&x);
  x.stqh_first =  head_ret_msg.stqh_first;
  x.stqh_last =  head_ret_msg.stqh_last;
  return x; 
}







/* -------------------------------------------------------------------------------------------------
 *  FName: Include_CommandFn
 *  Desc: Opens the fileName, loads and splits the content it in multiple lines by '\r\n',
 *        recreates multi-line text by '//' and executes each line.
 *  Info: 'fileName' is the file-name that should be loaded
 *  Para: const uint8_t *argsText  -> prt to include command args text "fileName"
 *        const size_t argsTextLen -> attr command args text length
 *  Rets: struct headRetMsgMultiple_s -> STAILQ head of multiple retMsg, if NULL -> NO entry
 * -------------------------------------------------------------------------------------------------
 */
struct Head_String_s
Include_Command2Fn (const String_t args)
{
  #if Include_Command_DBG >= 7
  SCDEFn->Log3Fn(Include_ProvidedByCommand.commandNameText
	,Include_ProvidedByCommand.commandNameTextLen
	,7
	,"CommandFn called with args '%.*s'"
	,args.len
	,args.p_char);
  #endif

// -------------------------------------------------------------------------------------------------

  // prepare STAILQ head to store multiple 'ret_msg' elements
  struct Head_String_s head_ret_msg;

  // Initialize the queue
  STAILQ_INIT(&head_ret_msg);

// -------------------------------------------------------------------------------------------------

  // expected argument #1
  String_t file_name;

  // set * to start of possible def-spec text (seek-start-pos)
  file_name.p_char = args.p_char;

  // the total seek-counter
  int i = 0;
  
  // an element seek-counter
  int j = 0;

  // seek * to start of  def-spec text ('\32' after space)
  while( ( i < args.len ) && ( *file_name.p_char == ' ' ) ) { i++ ; file_name.p_char++ ; }

  // 1 @ finnished

  // no further arguments expected - searching the end of text
  String_t end_of_text;
	
  // set start * of possible 'end of text' seek-start-pos
  end_of_text.p_char = file_name.p_char;
	
  // clear element seek-counter
  j = 0;

  // seek to next space '\32'
  while( ( i < args.len ) && ( *end_of_text.p_char != ' ' ) ) { i++ , j++ ; end_of_text.p_char++ ; }

  // length of attr-Val text determined
  file_name.len = j;

  // @ 'p_end_of_text' ! No further processing ...

// -------------------------------------------------------------------------------------------------

  // verify lengths > 0, ATTR_VAL_LEN = 0 ALLOWED!
  if ( file_name.len == 0 ) {

	// alloc mem for retMsg
	Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	// response with error text
	p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		"Error! Could not interpret '%.*s'! Usage: Include <fileName>",
		(int) args.len,
		args.p_char);

	// insert ret_msg as entry in stail-queue
	STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);

  	// return head of singly linked tail queue, which holds 'ret_msg' elements
    return 	head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------







  // Open the configfile or statefile for reading
  char *fileName;
  asprintf(&fileName

#if defined(ESP_PLATFORM)
		,"/spiffs/%.*s"
#else		
				,"/home/maikschulze/LINUX/LINUX_Device/spiffs/%.*s"
#endif		
		,file_name.len
		,file_name.p_char);






  // file handle
  FILE *fh;
  
  // open file
  fh = fopen(fileName, "r");

  // error msg handling, in case of probs
  if (!fh) {

	// alloc mem for retMsg
	Entry_String_t* p_entry_ret_msg =
		 malloc(sizeof(Entry_String_t));

	// response with error text
	p_entry_ret_msg->string.len = asprintf(&p_entry_ret_msg->string.p_char,
		"Error! Can't open file %.*s !",
//		(int) file_name.len,	//real
//		file_name.p_char);
		strlen(fileName),	//platform
		fileName);

	// insert ret_msg as entry in stail-queue
	STAILQ_INSERT_TAIL(&head_ret_msg, p_entry_ret_msg, entries);
	
free(fileName);

  	// return head of singly linked tail queue, which holds 'ret_msg' elements
    return 	head_ret_msg;
  }

// -------------------------------------------------------------------------------------------------

free(fileName);



  SCDEFn->Log3Fn(Include_ProvidedByCommand.commandNameText
	,Include_ProvidedByCommand.commandNameTextLen
	,1
	,"Including %.*s."
	,(int) file_name.len
	,file_name.p_char);

/* FHEM
1363	  my @t = localtime(gettimeofday());
1364	  my $gcfg = ResolveDateWildcards(AttrVal("global", "configfile", ""), @t);
1365	  my $stf  = ResolveDateWildcards(AttrVal("global", "statefile",  ""), @t); //cfg + state file bereinigt bereitstellen

1366	  if(!$init_done && $arg ne $stf && $arg ne $gcfg) {		// init done nicht abgeschlossen? + cfg oder statefile?
1367	    my $nr =  $devcount++;									// ???
1368	    $comments{$nr}{TEXT} = "include $arg";					//
1369	    $comments{$nr}{CFGFN} = $currcfgfile if($currcfgfile ne $gcfg);	// ???
1370	  }
*/
 
  // backup old configuration file, for reconstruction
  strText_t old_config_file = SCDERoot->current_config_file;

  // set temporary to filename from include arg
  SCDERoot->current_config_file.strText = (char *) file_name.p_char;			// besser machen!!!!
  SCDERoot->current_config_file.strTextLen = file_name.len;
 // SCDERoot->current_config_file = file_name;

  // holds the complete rebuilt line/multiline cmd row when reconstructed
  String_t complete_cmd_row;
  
  // holds the processed line number
  int line_no = 0;

  // clear, reset the global quit-flag
  SCDERoot->global_control_register_a &= ~(F_RECEIVED_QUIT);

  // our one line cache
  char line [256+1];				//Makro!

  // we need to start empty
  complete_cmd_row.p_char = NULL;
    
  // get file content (cmd text) row by row (splitted /n newline)
  while (fgets(line, sizeof(line), fh)) {

	line_no++;

  	// ?
  	String_t partial_cmd_row;

// ---

	// input: a line of cmd-text (zero terminated string)
  	// goals: 1. in case of content, add it to complete cmd-row
  	//        2. remove '\r' and/or '\n' from partial_cmd_rows
  	//        3. check for /^(.*)\\ *$/ -> indicates partial cmd-row,
  	//               more to come for complete_cmd_row 
  	
  	char *p_src, *p_dst, *p_seek;

  	p_seek = p_src = p_dst = line;

	// no cmd-text - keep complete cmd-row empty
  	if (*p_src != '\0') do {
	
		// move processed character
		*p_dst = *p_src;
		
		// check this and upcoming characters for /^(.*)\\ *$/
		//  -> indicates partial cmd row to process
		p_seek = p_src;
		
		if (*p_seek++ == '\\') {

			if (*p_seek++ == '\\') {
		
				while ( (*p_seek == ' ') 
					 || (*p_seek == '\r')
					 || (*p_seek == '\n') ) p_seek++;
			
				if (*p_seek == '\0') {
			
					// indicate -> a partial cmd row should be processed
					p_seek = NULL;	
				}
		 	}
		}
	 
		// -> partial cmd row was processed? or '\0' in src is next ?
		if (p_seek == NULL || *++p_src == '\0') {
	
			// zero terminate to use strlen (for *p_src != '\0')
			*p_dst = '\0';
  
  			// get length in this loop
  			partial_cmd_row.len = (size_t) strlen(line);
  			partial_cmd_row.p_char = (uint8_t *) line;
			
  			// further processing only if we have text
  			if (partial_cmd_row.len > 0) {

  				// if starting with new command row (not continuing one) ...
  				if (complete_cmd_row.p_char == NULL) {

  					// memory allocation for new command row
  					complete_cmd_row.p_char = (uint8_t *) malloc(partial_cmd_row.len);

  					// copy corrent row to allocated memory
					memcpy(complete_cmd_row.p_char
						,partial_cmd_row.p_char
						,partial_cmd_row.len);

					// init length
					complete_cmd_row.len = partial_cmd_row.len;
  				}

  				// continuing a command row with an part ...
 				 else {

					// reallocate memory to new size
					complete_cmd_row.p_char = (uint8_t *) realloc(complete_cmd_row.p_char
						,complete_cmd_row.len + partial_cmd_row.len);

					// add command-part to allocated memory
					memcpy(complete_cmd_row.p_char + complete_cmd_row.len
						,partial_cmd_row.p_char
						,partial_cmd_row.len);

					// save new length
					complete_cmd_row.len += partial_cmd_row.len;
  				}
  			}
 		
 			// cmd row is complete
			break;
		} 
	 
		// continue filtering '\r' || '\n'
		if (*p_dst != ('\r' || '\n') ) p_dst++;
	
  	} while (1);
  
  	// continue when an initial part of an multi-row-cmd was parsed - indicated by p_seek == NULL
    if (p_seek == NULL) continue;
  
	// if we have an parseable cmd-row now, parse it ...
	if (complete_cmd_row.p_char) {
    
		// call the AnalyzeCommandChainFn, if retMsg != NULL -> got ret Msgs entries		//warum multiple???
		struct headRetMsgMultiple_s headRetMsgMultipleFromFn =
			SCDEFn->AnalyzeCommandChainFn((const uint8_t *) complete_cmd_row.p_char,
				complete_cmd_row.len);

		// retMsgMultiple stailq filled from Fn ? -> get the entries till empty
		while (!STAILQ_EMPTY(&headRetMsgMultipleFromFn)) {

			// for the retMsg elements
			strTextMultiple_t *retMsg =
				STAILQ_FIRST(&headRetMsgMultipleFromFn);

			SCDEFn->Log3Fn(Include_ProvidedByCommand.commandNameText
				,Include_ProvidedByCommand.commandNameTextLen
				,5
				,"%.*s line %d returned >%.*s<"
				,file_name.len
				,file_name.p_char
				,line_no
				,retMsg->strTextLen
				,retMsg->strText);	

			// first remove this entry
			STAILQ_REMOVE(&headRetMsgMultipleFromFn, retMsg, strTextMultiple_s, entries);

			// then insert retMsg in stail-queue
//			STAILQ_INSERT_TAIL(&headRetMsgMultiple, retMsg, entries);
			STAILQ_INSERT_TAIL(&head_ret_msg, retMsg, entries);
		}

   		// we have an complete cmd-row here
  		free(complete_cmd_row.p_char);
		complete_cmd_row.p_char = NULL;
	}
  
	// break cmd read loop, if the global quit-flag is set
	if (SCDERoot->global_control_register_a & F_RECEIVED_QUIT) break;
  
  // continue next row
  }

  // there is a chance that we have an incomplete cmd-row here - free the mem
  if (complete_cmd_row.p_char) free(complete_cmd_row.p_char);

  // rebuilt current cfg file
  SCDERoot->current_config_file = old_config_file;

  // close file
  fclose(fh);

  // return head of singly linked tail queue, which holds 'ret_msg' elements
  return head_ret_msg;
}



