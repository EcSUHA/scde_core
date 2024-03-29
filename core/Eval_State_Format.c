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

4675	sub
4676	evalStateFormat($)
4677	{
4678	  my ($hash) = @_;
4679	
4680	  my $name = $hash->{NAME};
4681	
4682	  ###########################
4683	  # Set STATE
4684	  my $st = $hash->{READINGS}{state};
4685	  if($hash->{skipStateFormat} && defined($st)) {
4686	    $hash->{STATE} = ReplaceEventMap($name, $st->{VAL}, 1);
4687	    return;
4688	  }
4689	
4690	  my $sr = AttrVal($name, "stateFormat", undef);
4691	  if(!$sr) {
4692	    $st = $st->{VAL} if(defined($st));
4693	
4694	  } elsif($sr =~ m/^{(.*)}$/s) {
4695	    $cmdFromAnalyze = $1;
4696	    $st = eval $1;
4697	    if($@) {
4698	      $st = "Error evaluating $name stateFormat: $@";
4699	      Log 1, $st;
4700	    }
4701	    $cmdFromAnalyze = undef;
4702	
4703	  } else {
4704	    # Substitute reading names with their values, leave the rest untouched.
4705	    $st = $sr;
4706	    my $r = $hash->{READINGS};
4707	    $st =~ s/\$name/$name/g;
4708	    (undef, $st) = ReplaceSetMagic($hash, 1, $st);
4709	    $st =~ s/\b([A-Za-z\d_\.-]+)\b/($r->{$1} ? $r->{$1}{VAL} : $1)/ge
4710	      if($st eq $sr);
4711	
4712	  }
4713	  $hash->{STATE} = ReplaceEventMap($name, $st, 1) if(defined($st));
4714	}
*/



/* sucht das state reading in den definitions-readings und packt es formatiert in den/das STATE!
 * --------------------------------------------------------------------------------------------------
 *  FName: Eval_State_Format_Fn
 *  Desc: Evaluates the state-reading embedded in each definition
 *  Para: Entry_Common_Definition_t *p_entry_common_definition -> definition for which the state reading should be evaluated
 *  Rets: -/-
 * --------------------------------------------------------------------------------------------------
 */
void
Eval_State_Format_Fn(Entry_Common_Definition_t *p_entry_common_definition)
{
  // uses entry_reading2_t *p_state in definition, for direct access (should be maintained by modules!)

/*
  // find state readings entry
  Entry_Reading_t *p_entry_state_reading;
  STAILQ_FOREACH(p_entry_state_reading, &p_entry_common_definition->head_readings, entries) {
  
      // break if this entry is the state reading
	  if ( (p_entry_state_reading->reading.name.len ==  5) // "state" = 5
			&& (!strncmp((const char*) p_entry_state_reading->reading.name.p_char,
				(const char*) "state", 5)) ) break;
  }
 */
  
// -------------------------------------------------------------------------------------------------

  // found state readings entry + flag SKIP_STATE_FORMAT_EVAL
  if ( (p_entry_common_definition->p_state_reading) &&
      (p_entry_common_definition->Common_CtrlRegA & (F_SKIP_STATE_FORMAT_EVAL)) ) {
      
//    return;    
  }
  
// -------------------------------------------------------------------------------------------------

// temp replace state raw ... 
  if (p_entry_common_definition->p_state_reading) {

      // upd. existing reading, 1. free existing value
      if (p_entry_common_definition->state.p_char) 
	      free(p_entry_common_definition->state.p_char);

      string_t value_as_text = 
 //         p_entry_common_definition->p_state_reading->reading.p_entry_reading_type->p_get_raw_reading_as_text_fn(&p_entry_common_definition->p_state_reading->reading);				
          p_entry_common_definition->p_state_reading->p_reading_type->p_get_raw_reading_as_text_fn(p_entry_common_definition->p_state_reading);				

      // upd. existing reading, 2. fill reading with new value
      p_entry_common_definition->state.len =
	      asprintf((char **) &p_entry_common_definition->state.p_char
	        ,"%.*s"
	        ,value_as_text.len
            ,value_as_text.p_char);
	        
	  free(value_as_text.p_char);       
  }
 
// -------------------------------------------------------------------------------------------------
 
// temp debug out ... 
  if (p_entry_common_definition->state.p_char) {
  
	  printf("evaluated STATE = %.*s\n"
		  ,p_entry_common_definition->state.len
		  ,p_entry_common_definition->state.p_char);	
  }
  
  else printf("STATE READING NOT MAINTAINED! CAN NOT EVAL\n");

 
// -------------------------------------------------------------------------------------------------

  return;
}



