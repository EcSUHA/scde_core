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



/*
 * --------------------------------------------------------------------------------------------------
 *  FName: readingsEndUpdate
 *  Desc: Call readingsEndUpdate when you are done updating readings. This optionally calls DoTrigger
 *        to propagate the changes.
 *  Para: const uint8_t *commandTxt ->  ptr to the Command-Text
 *       const size_t commandTxtLen -> Command-Text length
 *       commandFn_t commandFn -> the command Fn
 *       const uint8_t *commandHelp -> ptr the Command-Help text
 *       const size_t commandHelpLen -> Command-Help text length
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
/*


Benötigt:

strText_t	GetAttrValTextByDefTextAttrText(strText_t defName, strText_t attrName);
attribute_t	GetAttrPtrByDefTextAttrText(strText_t defName, strText_t attrName);


attribute_t	SetAttrByDefTextAttrTextValText(strText_t defName, strText_t attrName, strText_t attrValue);
*/


void //ICACHE_FLASH_ATTR
InitA()
{

printf("Enter0");
// Zeile 570 , Initialisierung ...

  // for building the summary of ret-msg from cfg + state file including (starts empty)
  strText_t cfgRet= {NULL, 0};

// -------------------------------------------------------------------------------------------------

  // get attribute configfile value (global->configfile)
  String_t attrCfgFNDefName = {(uint8_t*) "global", 6};
//  String_t attrCfgFNAttrName = {(uint8_t*) "configfile", 10};
  char attrCfgFNAttrName[] = "configfile";
  char *attrCfgFNValueName  =
	Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn(&attrCfgFNDefName, &attrCfgFNAttrName);

  if (!attrCfgFNValueName) printf("got no value for configfile");


// -------------------------------------------------------------------------------------------------
	printf("Enter2");
	
	printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
printf("Enter0");
	
	
  // create the args to execute the 'include configfile' command
  String_t incCFIncludeCommandArgs;

  incCFIncludeCommandArgs.len =
	asprintf((char**) &incCFIncludeCommandArgs.p_char
		,"include %s"
		,attrCfgFNValueName);
		
	printf("Enter3");
	
  // call command include to process the initial config-file
  struct Head_String_s head_ret_msg_from_cf_inc =
		AnalyzeCommandChain(incCFIncludeCommandArgs);


// temp conversion start
  struct headRetMsgMultiple_s incCFHeadRetMsgMultipleFromFn;
  STAILQ_INIT(&incCFHeadRetMsgMultipleFromFn);
  incCFHeadRetMsgMultipleFromFn.stqh_first =  head_ret_msg_from_cf_inc.stqh_first;
  incCFHeadRetMsgMultipleFromFn.stqh_last =  head_ret_msg_from_cf_inc.stqh_last;
// temp conversion end



 /* // free value from Fn GetAttrValTextByDefTextAttrText
  if (attrCfgFNValueName) {
	  
		if (attrCfgFNValueName->strText) free(attrCfgFNValueName->strText);

		free(attrCfgFNValueName);

	}*/

  free(incCFIncludeCommandArgs.p_char);

// -------------------------------------------------------------------------------------------------

  // in case of strTextMultiple_t queue element add leading "configfile:" text to cfgRet.strText
  if (!STAILQ_EMPTY(&incCFHeadRetMsgMultipleFromFn)) {
	  
		// fill cfgRet.strText with leading "configfile:" text
		cfgRet.strTextLen = asprintf((char**) &cfgRet.strText
			,"configfile: ");

	}

  // loop the queue entries of retMsgMultiple and add them to cfgRet.strText
  while (!STAILQ_EMPTY(&incCFHeadRetMsgMultipleFromFn)) {

		// get an retMsg element from queue
		strTextMultiple_t *retMsg =
			STAILQ_FIRST(&incCFHeadRetMsgMultipleFromFn);

		// reallocate cfgRet.strText memory to new size
		cfgRet.strText = (char *) realloc(cfgRet.strText
			,cfgRet.strTextLen + retMsg->strTextLen);

		// add current retMsg->strText to cfgRet.strText
		memcpy(cfgRet.strText + cfgRet.strTextLen
			,retMsg->strText
			,retMsg->strTextLen);

		// save new retMsg->strTextLen
		cfgRet.strTextLen += retMsg->strTextLen;
	
		// done, remove this entry from queue
		STAILQ_REMOVE_HEAD(&incCFHeadRetMsgMultipleFromFn, entries);
//	STAILQ_REMOVE(&headRetMsgMultipleFromFn, retMsg, strTextMultiple_s, entries);

		// free the allocated retMsg->strText
		free(retMsg->strText);

		// and the allocated strTextMultiple_t queue element
		free(retMsg);

	}

// -------------------------------------------------------------------------------------------------

  // debug info ..
  if (cfgRet.strText) {

	printf("cfgRet after reading configfile filled: %.*s"
			,cfgRet.strTextLen
			,(char*) cfgRet.strText);

  }
  else {

	printf("cfgRet NOT filled!");
  }

// -------------------------------------------------------------------------------------------------

	// get attribute 'statefile' value (from global->statefile)
  String_t attrStateFNDefName = {(uint8_t*) "global", 6};
  char attrStateFNAttrName[] = "statefile";
  char *attrStateFNValueName =
		Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn(&attrStateFNDefName, &attrStateFNAttrName);

// -------------------------------------------------------------------------------------------------

  // for the args to execute the 'include statefile' command
  String_t *incSFIncludeCommandArgs = NULL;

  // attr "statefile" found ?
  if (attrStateFNValueName) {

		// +value found ? 
//		if (attrStateFNValueName->len) {

			// build strText_t for cmd args
			incSFIncludeCommandArgs =
				malloc(sizeof(String_t));

			// attribute "statefile" complete, use it to build args
			incSFIncludeCommandArgs->len =
				asprintf((char**) &incSFIncludeCommandArgs->p_char
					,"include %s"
                    ,attrStateFNValueName);

			//free(attrStateFNValueName->strText);
//		}

 // free(attrStateFNValueName);
	}

  // seems that attribute "statefile" is not set, use default to build args
  if (!incSFIncludeCommandArgs) {

		// build strText_t for cmd args
		incSFIncludeCommandArgs =
			malloc(sizeof(String_t));

		// and fill with cmd-args
		incSFIncludeCommandArgs->len =
			asprintf((char**) &incSFIncludeCommandArgs->p_char
			,"include state.cfg");
  }

  // call command include to process the initial state-file
  struct Head_String_s head_ret_msg_from_sf_inc =
		AnalyzeCommandChain(*incSFIncludeCommandArgs);


// temp conversion start
  struct headRetMsgMultiple_s incSFHeadRetMsgMultipleFromFn;
  STAILQ_INIT(&incSFHeadRetMsgMultipleFromFn);
  incSFHeadRetMsgMultipleFromFn.stqh_first =  head_ret_msg_from_sf_inc.stqh_first;
  incSFHeadRetMsgMultipleFromFn.stqh_last =  head_ret_msg_from_sf_inc.stqh_last;
// temp conversion end


  // free our strText_t that fits the created args
  if (incSFIncludeCommandArgs->p_char) 
		free(incSFIncludeCommandArgs->p_char);
  if (incSFIncludeCommandArgs)
		free(incSFIncludeCommandArgs);

// -------------------------------------------------------------------------------------------------

  // in case of strTextMultiple_t queue element add leading "statefile:" text to cfgRet.strText
  if (!STAILQ_EMPTY(&incSFHeadRetMsgMultipleFromFn)) {
	  
		// fill cfgRet.strText with leading "statefile:" text
		cfgRet.strTextLen = asprintf((char**) &cfgRet.strText
			,"statefile: ");

	}

  // loop the queue entries of retMsgMultiple and add them to cfgRet.strText
  while (!STAILQ_EMPTY(&incSFHeadRetMsgMultipleFromFn)) {

		// get an retMsg element from queue
		strTextMultiple_t *retMsg =
			STAILQ_FIRST(&incSFHeadRetMsgMultipleFromFn);

		// reallocate cfgRet.strText memory to new size
		cfgRet.strText = (char *) realloc(cfgRet.strText
			,cfgRet.strTextLen + retMsg->strTextLen);

		// add current retMsg->strText to cfgRet.strText
		memcpy(cfgRet.strText + cfgRet.strTextLen
			,retMsg->strText
			,retMsg->strTextLen);

		// save new retMsg->strTextLen
		cfgRet.strTextLen += retMsg->strTextLen;

		// done, remove this entry from queue
		STAILQ_REMOVE_HEAD(&incSFHeadRetMsgMultipleFromFn, entries);
//	STAILQ_REMOVE(&headRetMsgMultipleFromFn, retMsg, strTextMultiple_s, entries);

		// free the allocated retMsg->strText
		free(retMsg->strText);

		// and the allocated strTextMultiple_t queue element
		free(retMsg);

	}

// -------------------------------------------------------------------------------------------------

  // debug info ..
  if (cfgRet.strText) {

	printf("cfgRet after reading state filled: %.*s"
			,cfgRet.strTextLen
			,(char*) cfgRet.strText);

  }
  else {

	printf("cfgRet NOT filled!");
  }

// -------------------------------------------------------------------------------------------------

  const char cfgErrMsg[] = "Messages collected while initializing FHEM:";


  // if cfgRet text -> set attribute motd to cfgRet text
  if (cfgRet.strText) {

  // build our motd attribute value and assign it
  strText_t motdAttrVal;

  motdAttrVal.strTextLen = asprintf((char**) &motdAttrVal.strText
	,"%s\n%.*s\r\n"
	,cfgErrMsg
	,(int) cfgRet.strTextLen
	,cfgRet.strText);


  strText_t defName = {(char *) "global", 6};
  strText_t attrName = {(char *) "motd", 4};

//  SetAttrValByDefTextAttrTextValText({"global", 6}, {"motd", 10}, motdAttrVal);
//  SetAttrValByDefTextAttrTextValText(&defName, &attrName, motdAttrVal);

	printf("motddebug: %.*s"
			,motdAttrVal.strTextLen
			,(char*) motdAttrVal.strText);

  free(motdAttrVal.strText);

//  Log 1, $cfgRet;

  free(cfgRet.strText);

  }

  // if no cfgRet text -> clr attribute motd, but only if holding old cfgRet text (detected by leadin)
  // else clear motd attribute
  else {

//	elsif($attr{global}{motd} && $attr{global}{motd} =~ m/^$cfgErrMsg/) {
//	$attr{global}{motd} = "";

  }



  // init is done, set init done flag
  SCDERoot.global_control_register_a |= F_INIT_DONE;


  printf("entering6...");
 // vTaskDelay(1000 / portTICK_PERIOD_MS);







/* bereich ist für telnet setup gewesen
// -------------------------------------------------------------------------------------------------

  // try to get the port (for telnet) from value of attribute (global->port)
  strText_t defNameGlobal = {(char *) "global", 6};
  strText_t attrNamePort = {(char *) "port", 4};
  strText_t *valueNamePort =
//	GetAttrValTextByDefTextAttrText( {(uint8_t*) "global", 6}, {(uint8_t*) "port", 4});
	Get_attrVal_by_defName_and_attrName(&defNameGlobal, &attrNamePort);

  // if we have an value for attribute port -> build an telnet definition
  if ( (valueNamePort) && (valueNamePort->strText) ) {

	// create the args to execute the 'define telnet telnet port' command
	strText_t defTelnetCommandArgs;

	defTelnetCommandArgs.strTextLen = asprintf((char**) &defTelnetCommandArgs.strText
		,"define Telnet Telnet %.*s"
		,(int) valueNamePort->strTextLen
		,(char*) valueNamePort->strText);

	// call command include to process the initial config-file
//war	struct headRetMsgMultiple_s headRetMsgMultipleFromFn =
//		AnalyzeCommandChain(defTelnetCommandArgs.strText, defTelnetCommandArgs.strTextLen);

	struct headRetMsgMultiple_s headRetMsgMultipleFromFn =
		AnalyzeCommandChain((uint8_t*)defTelnetCommandArgs.strText,(const size_t) defTelnetCommandArgs.strTextLen);


//	AnalyzeCommandChain((uint8_t*)includeCommandArgs.strText, (const size_t) 

	free(defTelnetCommandArgs.strText);

	// got an retMsg from Fn ? -> log it, delete port attribute
	if (STAILQ_EMPTY(&headRetMsgMultipleFromFn)) {

		// get the entries from retMsgMultiple till empty, if any
		while (!STAILQ_EMPTY(&headRetMsgMultipleFromFn)) {

			// for the retMsg elements
			strTextMultiple_t *retMsg =
				STAILQ_FIRST(&headRetMsgMultipleFromFn);

			LOGD("have to add log RetMsg: %.*s\n"
				,retMsg->strTextLen
				,retMsg->strText);

			// done, remove this entry
			STAILQ_REMOVE_HEAD(&headRetMsgMultipleFromFn, entries);
//			STAILQ_REMOVE(&headRetMsgMultipleFromFn, retMsg, strTextMultiple_s, entries);

			// free the msg-string
			free(retMsg->strText);

			// and the strTextMultiple_t
			free(retMsg);
		}

	LOGD("errors - delete port attribute here...");
	//vTaskDelay(1000 / portTICK_PERIOD_MS);
//	// error occured - delete the port attribute
//	delete($attr{global}{port});

	}

  }

  // warning if we could not find a valid telnet port
  else {

	LOGD("No port attr val for telnet assigned to global..");

  }

  // free value from Fn GetAttrValTextByDefTextAttrText
  if (valueNamePort) {

	if (valueNamePort->strText) free(valueNamePort->strText);

	free(valueNamePort);

  }

// -------------------------------------------------------------------------------------------------

  LOGD("entering7...");
 // vTaskDelay(1000 / portTICK_PERIOD_MS);

*/



}


/*
513	my $cfgErrMsg = "Messages collected while initializing FHEM:";
514	my $cfgRet="";

520	  my $ret = CommandInclude(undef, $attr{global}{configfile});
521	  $cfgRet .= "configfile: $ret\n" if($ret);
522	
523	  my $stateFile = $attr{global}{statefile};
524	  if($stateFile) {
525	    my @t = localtime;
526	    $stateFile = ResolveDateWildcards($stateFile, @t);
527	    if(-r $stateFile) {
528	      $ret = CommandInclude(undef, $stateFile);
529	      $cfgRet .= "$stateFile: $ret\n" if($ret);
530	    }
531	  }
533	
534	if($cfgRet) {
535	  $attr{global}{motd} = "$cfgErrMsg\n$cfgRet";
536	  Log 1, $cfgRet;
537	
538	} elsif($attr{global}{motd} && $attr{global}{motd} =~ m/^$cfgErrMsg/) {
539	  $attr{global}{motd} = "";
540	
541	}
542	
543	my $pfn = $attr{global}{pidfilename};
544	if($pfn) {
545	  die "$pfn: $!\n" if(!open(PID, ">$pfn"));
546	  print PID $$ . "\n";
547	  close(PID);
548	}
549	
550	my $gp = $attr{global}{port};
551	if($gp) {
552	  Log 3, "Converting 'attr global port $gp' to 'define telnetPort telnet $gp'";
553	  my $ret = CommandDefine(undef, "telnetPort telnet $gp");
554	  Log 1, "$ret" if($ret);
555	  delete($attr{global}{port});
556	}
557	
558	my $sc_text = "SecurityCheck:";
559	$attr{global}{motd} = "$sc_text\n\n"
560	        if(!$attr{global}{motd} || $attr{global}{motd} =~ m/^$sc_text/);
561	
562	$init_done = 1;
563	$lastDefChange = 1;
564	
565	foreach my $d (keys %defs) {
566	  if($defs{$d}{IODevMissing}) {
567	    Log 3, "No I/O device found for $defs{$d}{NAME}";
568	    delete $defs{$d}{IODevMissing};
569	  }
570	}
571	
572	DoTrigger("global", "INITIALIZED", 1);
573	$fhem_started = time;
574	
575	$attr{global}{motd} .= "Running with root privileges."
576	        if($^O !~ m/Win/ && $<==0 && $attr{global}{motd} =~ m/^$sc_text/);
577	$attr{global}{motd} .=
578	        "\nRestart FHEM for a new check if the problem is fixed,\n".
579	        "or set the global attribute motd to none to supress this message.\n"
580	        if($attr{global}{motd} =~ m/^$sc_text\n\n./);
581	my $motd = $attr{global}{motd};
582	if($motd eq "$sc_text\n\n") {
583	  delete($attr{global}{motd});
584	} else {
585	  if($motd ne "none") {
586	    $motd =~ s/\n/ /g;
587	    Log 2, $motd;
588	  }
589	}
590	
591	my $osuser = "os:$^O user:".(getlogin || getpwuid($<) || "unknown");
592	Log 0, "Featurelevel: $featurelevel";
593	Log 0, "Server started with ".int(keys %defs).
594	        " defined entities ($attr{global}{version} perl:$] $osuser pid:$$)";
595		
*/








/*
 *--------------------------------------------------------------------------------------------------
 *FName: Log
 * Desc: This is the main logging function
 * Info: Level 0=System; 16=debug
 * Para: char *Device -> Device Name string that should be associated to the log
 *       int LogLevel -> Level of information
 *       char *Text -> Text string that should be logged
 * Rets: -/-
 *--------------------------------------------------------------------------------------------------
 */
void
Log4 (char *text)
{

  printf ("%s", text);
}



/*
 *--------------------------------------------------------------------------------------------------
 *FName: CommandLoadCommand //ActivateCommand?
 * Desc: Loads a command (Bultin commands after init, ...)
 * Para: const uint8_t *commandTxt ->  ptr to the Command-Text
 *       const size_t commandTxtLen -> Command-Text length
 *       commandFn_t commandFn -> the command Fn
 *       const uint8_t *commandHelp -> ptr the Command-Help text
 *       const size_t commandHelpLen -> Command-Help text length
 * Rets: -/-
 *--------------------------------------------------------------------------------------------------
 */
int
CommandLoadCommand(const uint8_t *commandTxt
		,const size_t commandTxtLen
		,CommandFn_t CommandFn
		,const uint8_t *commandHelp
		,const size_t commandHelpLen)
  {
/*
  Command_t* NewCommand;

  // alloc mem for command specific definition structure (Command_t)
  NewCommand = malloc(sizeof(Command_t));

  // malloc for Command-Text
  NewCommand->commandTxt = malloc(commandTxtLen);

  // copy Command-Text
  memcpy(NewCommand->commandTxt, commandTxt, commandTxtLen);

  // copy Command-Text length
  NewCommand->commandTxtLen = commandTxtLen;

  // store ptr to the command function
  NewCommand->commandFn = commandFn;

  // malloc for Command-Help-Text
  NewCommand->commandHelp = malloc(commandHelpLen);

  // copy Command-Help-Text
  memcpy(NewCommand->commandHelp, commandHelp, commandHelpLen);

  // copy Command-Help-Text length
  NewCommand->commandHelpLen = commandHelpLen;

  // store command to SCDE-Root
  STAILQ_INSERT_HEAD(&SCDERoot.HeadCommands, NewCommand, entries);

//  Log3("main?", 1, "Loaded Command %s\n", commandTxt);

  Log3((const uint8_t*) "root"
	,4
	,1
	,"Loaded command. Command <%.*s> now available for use."
	,commandTxtLen
	,commandTxt);


*/
  return 0;
}













/*
 * Find the first occurrence of find in s, where the search is limited to the
 * first slen characters of s.
 */
char*
strnstr(s, find, slen)
	const char *s;
	const char *find;
	size_t slen;
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if ((sc = *s++) == '\0' || slen-- < 1)
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}



/**
 *--------------------------------------------------------------------------------------------------
 *FName: ParseKVInputArgs Parse Key=Value@ input Arguments
 * Desc: Hepler routine that parses Key=Value(@) input arguments into an allocated array.
 *
 *       . searches and extracts an SPECIAL string value matching the given key. 
 *       Source data is an key=value pair string, seperated by an "&" from an query string
 *       WARNING: FOR FIRST and LAST CHAR ONLY ALPHA OR NUMMERICALAL IS ALLOWED !	
 * Note:  kvArgsText // query keys dürfen keine encoded zeichen enthalten!
 * Para: int num -> number of implemented KEYs for this query-type
 *       const implementedKeys *XX_IK -> list of implemented KEYs for this query-type
 *       const char *QueryStr -> ptr to zero-teriminated KEY=VALUE string from query, seperated by '&'
 * Rets: struct SCDE_XX_parsedKVInput *parsedKVInput -> allocad struct, filled with data
 *       DONT FORGET TO FREE MEMORY !
 *--------------------------------------------------------------------------------------------------
 */
//TO DO:parse eines doppelten keys erkennen-> NULL zurück
//      parse unbekannter keys erkennen -> NULL zurück
//      FEHLER -> NULL zurück
parsedKVInputArgs_t*
ParseKVInputArgs(int numImplementedKeys
                ,const kvParseImplementedKeys_t *XX_IK
                ,const uint8_t *kvArgsText
                ,const size_t kvArgsTextLen)
{

 // LÄNGENANGABE NICHT OK !!!!!!!!!!!!!!!! 

  // alloc memory for ESP32_S0_parsedKVInput_t -> length is variable !
  int memlen =  8 + 8 + 8 + 4 + (numImplementedKeys * (2+2+4));

  #if ESP32_S0_DBG >= 5
  printf("|PKVInput-malloc:%d, input:\"%.*s\">"
	,memlen
	,kvArgsTextLen
	,kvArgsText);
  #endif

  parsedKVInputArgs_t *parsedKVInputArgs =
	(struct parsedKVInputArgs_s *) malloc(memlen);

  // zero the bulk update structure (bulkUpdateReadings_t)
 // memset(parsedKVInputArgs, 0, memlen);

  // clear keys-found bitfield -> user can read what keys are found
  parsedKVInputArgs->keysFoundBF = 0;

  // reset affected Readings -> option for user to set affected readings, start cleared
  parsedKVInputArgs->affectedReadingsBF = 0;

  // implemented keys loop
  int i;

  for ( i = 0 ; i < numImplementedKeys ; i++ ) {

	// get ptr to current key
	const char *key = XX_IK[i].implementedKey;

	#if ESP32_S0_DBG >= 5
	printf("|chk for KEY:%d-\"%s\", "
		,i
		,key);
	#endif

	const uint8_t *p = kvArgsText;			// kv-args start ptr
	const uint8_t *ep = kvArgsText + kvArgsTextLen;	// kv-args end ptr
	const uint8_t *e;				// end ptr 
	int keylen = strlen(key);			// lenght of implemented key

//	while ( (p != NULL) && (*p != '\n') && (*p != '\r') && (*p != 0) )

	while ( (p != NULL) && (p < ep) ) {

		# if ESP32_S0_DBG >= 5
		printf("cmp:\"%.*s\""
			,keylen
			,p);
		# endif

		// matches value? and is "=" after value?
		if ( (strncasecmp((char*) p, key, keylen) == 0 ) && (p[keylen] == '=') ) {

			// move p to start of the VALUE (from this KEY)
			p += keylen + 1;

			// move e to end of the VALUE (from this KEY)
			e = (uint8_t *) strnstr((char*) p, "&", ep - p);

			// or is it last Key-Value pair?
			if (e == NULL) e = ep;

 			# if ESP32_S0_DBG >= 5
			printf(", next cmp:\"%.*s\""
				,(e-p)
				,p);
			# endif

			// store positive result -> do not forget to url-encode !
			parsedKVInputArgs->keysFoundBF |= (uint64_t) 1<<i;
			parsedKVInputArgs->keyData_t[i].off = (p-kvArgsText); // or better store ptr?
			parsedKVInputArgs->keyData_t[i].len = (e-p);
			parsedKVInputArgs->keyData_t[i].affectedReadings = 
				XX_IK[i].affectedReadings;

			// KEY found
			# if ESP32_S0_DBG >= 5
			printf(", found!>");
			# endif

			break;

			}

		// jump after next "&"
		p = (uint8_t*) strnstr((char*) p, "&", ep - p);

		if (p != NULL) p += 1;

		}

	// a KEY is not found -> not allowed
	# if ESP32_S0_DBG >= 5
	printf(">");
	# endif

//	// free allocated memory for query result key-field
//	free(parsedKVInputArgs);
//	return NULL;
  }

  // Query parsed complete
  # if SCDEH_DBG >= 5
  HexDumpOut ("|parsedKVInputArgs-HEX",
	parsedKVInputArgs,
	memlen);
  # endif

  return parsedKVInputArgs;
}






