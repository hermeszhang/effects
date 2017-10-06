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

add_library("gnu-guitar-effects-bench"
  "bench.h"
  "bench.c")

function (ADD_GNU_GUITAR_EFFECTS_BENCH BENCH_NAME)
  add_executable(${BENCH_NAME} ${ARGN} $<TARGET_OBJECTS:gnu-guitar-effects>)
  target_link_libraries(${BENCH_NAME} "gnu-guitar-effects-bench")
  if (UNIX)
    target_link_libraries(${BENCH_NAME} "m")
  endif (UNIX)
  add_test(${BENCH_NAME} ${BENCH_NAME})
endfunction (ADD_GNU_GUITAR_EFFECTS_BENCH BENCH_NAME)

add_gnu_guitar_effects_bench("echo-bench" "echo-bench.c")
add_gnu_guitar_effects_bench("reverb-bench" "reverb-bench.c")
add_gnu_guitar_effects_bench("tremolo-bench" "tremolo-bench.c")
add_gnu_guitar_effects_bench("tubeamp-bench" "tubeamp-bench.c")

enable_testing()

