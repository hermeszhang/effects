cmake_minimum_required(VERSION 2.6)

set (INCDIR "${PROJECT_SOURCE_DIR}/include/gnu-guitar/effects")
set (SRCDIR "${PROJECT_SOURCE_DIR}/src")

add_library("gnu-guitar-effects" OBJECT
  "${INCDIR}/data-block.h"
  "${INCDIR}/backbuf.h"
  "${SRCDIR}/backbuf.c"
  "${INCDIR}/biquad.h"
  "${SRCDIR}/biquad.c"
  "${INCDIR}/tremolo.h"
  "${SRCDIR}/tremolo.c"
  "${INCDIR}/echo.h"
  "${SRCDIR}/echo.c"
  "${INCDIR}/effect.h"
  "${SRCDIR}/effect.c"
  "${INCDIR}/printf.h"
  "${SRCDIR}/printf.c"
  "${INCDIR}/reverb.h"
  "${SRCDIR}/reverb.c"
  "${INCDIR}/tubeamp.h"
  "${SRCDIR}/tubeamp.c")

add_library("gnu-guitar-effects-test"
  "test.h"
  "test.c")

function (ADD_GNU_GUITAR_EFFECTS_TEST TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN} $<TARGET_OBJECTS:gnu-guitar-effects>)
  target_link_libraries(${TEST_NAME} "gnu-guitar-effects-test")
  if (UNIX)
    target_link_libraries(${TEST_NAME} "m")
  endif (UNIX)
  add_test(${TEST_NAME} ${TEST_NAME})
endfunction (ADD_GNU_GUITAR_EFFECTS_TEST TEST_NAME)

add_gnu_guitar_effects_test("echo-test" "echo-test.c")
add_gnu_guitar_effects_test("reverb-test" "reverb-test.c")
add_gnu_guitar_effects_test("tremolo-test" "tremolo-test.c")
add_gnu_guitar_effects_test("tubeamp-test" "tubeamp-test.c")

enable_testing()

