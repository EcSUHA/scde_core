/* #################################################################################################
 *
 *  Function: GLOBAL Module for SCDE (Smart Connected Device Engine)
 *
 *  DeVICE -> DeVICE Feature
 *
 *  It is used to control an IoT Device (ESP32 - SoC and OS) that runs the SCDE 
 *  (Smart Connected Device Engine)
 *
 *  ESP 8266EX & ESP32 SOC Activities ...
 *  Copyright by EcSUHA
 *
 *  Created by Maik Schulze, Sandfuhren 4, 38448 Wolfsburg, Germany for EcSUHA.de 
 *
 *  MSchulze780@GMAIL.COM
 *  Copyright by EcSUHA
 *
 *  EcSUHA - ECONOMIC SURVEILLANCE AND HOME AUTOMATION - WWW.EcSUHA.DE
 * #################################################################################################
 */

#include <ProjectConfig.h>
#include <esp8266.h>
#include <Platform.h>



// the Smart-Connected-Device-Engine structures & types ...
#include <SCDE_s.h>

// provides WebIf, need the structures & types ...
//#include "WebIf_Module.h"

// this Modules structures & types ...
#include "GLOBAL_Module.h"
#include "GLOBAL_readings.h"

// -------------------------------------------------------------------------------------------------



string_t
GLOBAL_Get_Reading_State_As_Text(reading2_t *p_state_reading)
{
  string_t state_text;
    
  state_text.len = asprintf((char **) &state_text.p_char
		  ,"%.*s"
          ,p_state_reading->raw_data.len
          ,p_state_reading->raw_data.p_char);
  
  return state_text;
}



// -------------------------------------------------------------------------------------------------



void
GLOBAL_Set_Reading_State_From_Text(reading2_t *p_state_reading, char* p_value_as_text)
{
  // do we have text?
  if ( (p_value_as_text) && (strlen(p_value_as_text)) ) {
 
      // yes 
      if (p_state_reading->raw_data.p_char) free(p_state_reading->raw_data.p_char);
        
      p_state_reading->raw_data.len = asprintf((char **) &p_state_reading->raw_data.p_char
		  ,"%s"
          ,p_value_as_text);
  } 
  
  /// no
  else {
  
  p_state_reading->raw_data.p_char = NULL;
  p_state_reading->raw_data.len = 0;
  }
  
  printf ("stored to cccc reading:'%.*s'",
          p_state_reading->raw_data.len,
          p_state_reading->raw_data.p_char);
  
  
  
  return;
}



// -------------------------------------------------------------------------------------------------



// GLOBAL - embedded reading types
reading_type_t GLOBAL_reading_types[] = { 
 { // reading state -> state is text only reading
  "state",
  "txt", 
  &GLOBAL_ProvidedByModule,
  GLOBAL_Get_Reading_State_As_Text,
  GLOBAL_Set_Reading_State_From_Text
 },
 


 {NULL} // marks end of reading types
};



