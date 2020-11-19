# CMakeLists.txt for this Project (needs to be customized)
# Platform: LINUX



# Set values of hypothetical variables that control the build of `foo`
set(FOO_BUILD_STATIC OFF)
set(FOO_BUILD_TESTS OFF)    



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/include)


# add the SCDE core components
 
  # the Core
  add_subdirectory(core)
  
  # the embedded Commands
  add_subdirectory(commands/Attr_Command)
  add_subdirectory(commands/Define_Command)
  add_subdirectory(commands/Delete_Command)
  add_subdirectory(commands/Deleteattr_Command)
  add_subdirectory(commands/Get_Command)
  add_subdirectory(commands/Help_Command)
  add_subdirectory(commands/Include_Command)
  add_subdirectory(commands/IOWrite_Command)
  add_subdirectory(commands/List_Command)
  add_subdirectory(commands/Rename_Command)
  add_subdirectory(commands/Rereadcfg_Command)
  add_subdirectory(commands/Save_Command)
  add_subdirectory(commands/Set_Command)
  add_subdirectory(commands/Setstate_Command)
  add_subdirectory(commands/Shutdown_Command)

  # the embedded Modules
  add_subdirectory(modules/Global_Module)    



