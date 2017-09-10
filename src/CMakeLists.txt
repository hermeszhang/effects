cmake_minimum_required(VERSION 2.6)

set (INCDIR "${PROJECT_SOURCE_DIR}/include/gnu-guitar/effects")
set (SRCDIR "${PROJECT_SOURCE_DIR}/src")

add_library("gnu-guitar-effects" OBJECT
  "${INCDIR}/data-block.h"
  "${INCDIR}/backbuf.h"
  "${SRCDIR}/backbuf.c"
  "${INCDIR}/echo.h"
  "${SRCDIR}/echo.c"
  "${INCDIR}/effect.h"
  "${SRCDIR}/effect.c"
  "${INCDIR}/printf.h"
  "${SRCDIR}/printf.c"
  "${INCDIR}/utils.h"
  "${SRCDIR}/utils.c")

