cmake_minimum_required(VERSION 2.6)

add_library("gnu-guitar-effects" SHARED
  "echo.h"
  "echo.c"
  "effect.h"
  "effect.c"
  "ladspa.h"
  "ladspa.c"
  "printf.h")

set_target_properties("gnu-guitar-effects" PROPERTIES PREFIX "")

install(TARGETS "gnu-guitar-effects"
  RUNTIME DESTINATION "bin/ladspa"
  ARCHIVE DESTINATION "lib/ladspa"
  LIBRARY DESTINATION "lib/ladspa")

