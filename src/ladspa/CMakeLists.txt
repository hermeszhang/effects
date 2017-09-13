cmake_minimum_required(VERSION 2.6)

add_library("gnu-guitar-ladspa" SHARED
  $<TARGET_OBJECTS:gnu-guitar-effects>
  "ladspa-descriptor.c"
  "echo.c"
  "reverb.c"
  "tubeamp.c")

set_target_properties("gnu-guitar-ladspa" PROPERTIES PREFIX "")

if (UNIX)
  target_link_libraries("gnu-guitar-ladspa" "m")
endif (UNIX)

if (MSVC)
  set_target_properties("gnu-guitar-ladspa" PROPERTIES
                        LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/gnu-guitar-ladspa.def\"")
endif (MSVC)
install(TARGETS "gnu-guitar-ladspa"
  RUNTIME DESTINATION "bin/ladspa"
  ARCHIVE DESTINATION "lib/ladspa"
  LIBRARY DESTINATION "lib/ladspa")

