#ifndef GLOBAL_READINGS_H
#define GLOBAL_READINGS_H



// -------------------------------------------------------------------------------------------------



// GLOBAL - embedded reading types
enum GLOBAL_READING_TYPES {	        // #XX desc for debugging
  GLOBAL_STATE = 0			        // #00 start
};



// -------------------------------------------------------------------------------------------------



//
string_t GLOBAL_Get_Reading_State_As_Text(reading2_t *p_state_reading);



// -------------------------------------------------------------------------------------------------



//
void GLOBAL_Set_Reading_State_From_Text(reading2_t *p_state_reading, char* p_value_as_text);



// -------------------------------------------------------------------------------------------------



// GLOBAL - embedded reading types
reading_type_t GLOBAL_reading_types[];

#endif // GLOBAL_READINGS_H
