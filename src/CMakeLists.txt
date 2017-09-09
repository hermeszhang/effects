cmake_minimum_required(VERSION 2.6)

add_library("gnuitar" SHARED
  "echo.h"
  "echo.c"
  "effect.h"
  "effect.c"
  "ladspa.h"
  "ladspa.c"
  "printf.h")

set_target_properties("gnuitar" PROPERTIES PREFIX "")

