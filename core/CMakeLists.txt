cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(SCDE_CORE)

add_library(${PROJECT_NAME}	
					"SCDE.c"
					"Activate_Command_Fn.c"
					"Activate_Module_Fn.c"
					"Add_Event_Fn.c"
					"Add_Query_Encoded_Fn.c"
					"Analyze_Command_Fn.c"
					"Analyze_Command_Chain_Fn.c"
					"Arg_Parse_Fn.c"
 					"CallGetFnByDefName.c"
 					"CommandReloadModule.c"
 					"Create_Notify_List_Fn.c"
 					"Create_Reading_Fn.c"
  					"Defspec_2_Array_Fn.c"
  					"Delete_Notify_List_Fn.c"
	 				"Do_Global_Def_Fn.c"
	 				"Do_Trigger_Fn.c"
	 				"Eval_State_Format.c"
					"Fmt_Time_Fn.c"
					"Get_All_Readings_Fn.c"
					"Get_Attr_Val_By_Def_Name_And_Attr_Name_Fn.c"
					"Get_Def_And_Attr_Fn.c"
					"Get_Definitions_That_Match_DefSpec_String.c"
					"Get_Formated_Date_Time_Fn.c"
					"Get_Ptr_To_Definition_By_Name_Fn.c"
					"Get_Ptr_To_Loaded_Module_By_Name_Fn.c"
					"GetTiSt.c"
					"GetUniqueTiSt.c"
					"Good_Device_Name_Fn.c"
					"Good_Reading_Name_Fn.c"
					"HexDumpOut.c"
					"InitSCDERoot.c"
					"Log.c"
					"Log3.c"
					"Make_Device_Name_Fn.c"
					"Make_Reading_Name_Fn.c"
					"Readings_Begin_Update_Fn.c"
					"Readings_Bulk_Update_Fn.c"
					"Readings_End_Update_Fn.c"
					"Remove_From_Notify_List_Fn.c"
					"Set_Readings_Value_Fn.c"
					"Str_To_Float_Fn.c"
					"Str_To_Uint16_Fn.c"
					"Str_To_Uint32_Fn.c"
					"Time_Now_Fn.c"
					"Write_State_File_Fn.c"		
					
					"unsorted.c"							
)

target_include_directories(${PROJECT_NAME} 
								     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
									 PUBLIC	"../../main/include"
									 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
