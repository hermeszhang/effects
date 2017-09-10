cmake_minimum_required(VERSION 2.6)

if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -fPIC -Wl,-z,defs")
endif (CMAKE_COMPILER_IS_GNUCC)

include_directories("include")

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Glib2 REQUIRED)

include_directories(${GLIB2_INCLUDE_DIRS})

add_subdirectory("src")
add_subdirectory("src/ladspa")

