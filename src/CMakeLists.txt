cmake_minimum_required(VERSION 2.6)

set (INCDIR "${PROJECT_SOURCE_DIR}/include/gnu-guitar/effects")
set (SRCDIR "${PROJECT_SOURCE_DIR}/src")

add_library("gnu-guitar-effects" SHARED
  "${INCDIR}/data-block.h"
  "${INCDIR}/backbuf.h"
  "${SRCDIR}/backbuf.c"
  "${INCDIR}/echo.h"
  "${SRCDIR}/echo.c"
  "${INCDIR}/effect.h"
  "${SRCDIR}/effect.c"
  "ladspa.h"
  "ladspa.c"
  "${INCDIR}/printf.h"
  "${SRCDIR}/printf.c"
  "${INCDIR}/utils.h"
  "${SRCDIR}/utils.c")

if (UNIX)
  target_link_libraries("gnu-guitar-effects" "m")
endif (UNIX)

target_link_libraries("gnu-guitar-effects" ${GLIB2_LIBRARIES})

set_target_properties("gnu-guitar-effects" PROPERTIES PREFIX "")

install(TARGETS "gnu-guitar-effects"
  RUNTIME DESTINATION "bin/ladspa"
  ARCHIVE DESTINATION "lib/ladspa"
  LIBRARY DESTINATION "lib/ladspa")

