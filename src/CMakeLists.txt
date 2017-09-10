cmake_minimum_required(VERSION 2.6)

add_library("gnu-guitar-effects" SHARED
  "audio-driver.h"
  "audio-driver.c"
  "backbuf.h"
  "backbuf.c"
  "echo.h"
  "echo.c"
  "effect.h"
  "effect.c"
  "ladspa.h"
  "ladspa.c"
  "printf.h"
  "printf.c"
  "utils.h"
  "utils.c")

if (UNIX)
  target_link_libraries("gnu-guitar-effects" "m")
endif (UNIX)

target_link_libraries("gnu-guitar-effects" ${GLIB2_LIBRARIES})

set_target_properties("gnu-guitar-effects" PROPERTIES PREFIX "")

install(TARGETS "gnu-guitar-effects"
  RUNTIME DESTINATION "bin/ladspa"
  ARCHIVE DESTINATION "lib/ladspa"
  LIBRARY DESTINATION "lib/ladspa")

