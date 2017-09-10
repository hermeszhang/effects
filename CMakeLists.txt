cmake_minimum_required(VERSION 2.6)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -fPIC -Wl,-z,defs")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -fPIC")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  # MSVC warning 4244 warns about implicit conversions. While this
  # should actually be enabled, the project contains too many harmless
  # implicit conversions at the time of writing this.
  set (CMAKE_C_FLAGS "/W3 /WX /D_CRT_SECURE_NO_WARNINGS /wd4244")
endif ()

include_directories("include")

add_subdirectory("src")
add_subdirectory("src/ladspa")

