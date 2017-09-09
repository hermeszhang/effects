cmake_minimum_required(VERSION 2.6)

add_library("gnuitar" SHARED
  "printf.h"
  "echo.h"
  "echo.c"
  "effect.h"
  "effect.c")

set_target_properties("gnuitar" PROPERTIES OUTPUT_NAME "gnuitar")

