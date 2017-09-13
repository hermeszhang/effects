cmake_minimum_required(VERSION 2.6)

project("gnu-guitar-effects")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -fPIC -Wl,-z,defs")
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
  endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -fPIC")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  # MSVC warning 4244 warns about implicit conversions. While this
  # should actually be enabled, the project contains too many harmless
  # implicit conversions at the time of writing this.
  #
  # The 4305 warning is caused by 'double' to 'float' truncation. Since
  # the audio sample type is 'float' and most control values are 'double',
  # this is bound to happen and can be ignored.
  set (CMAKE_C_FLAGS "/W3 /WX /D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4305")
endif ()

include_directories("include")

add_subdirectory("src")
add_subdirectory("src/ladspa")

enable_testing()

set (CPACK_PACKAGE_NAME "GNU Guitar Effects")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A collection of audio effects for the GNU Guitar project.")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_VERSION_MAJOR ${GNU_GUITAR_EFFECTS_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${GNU_GUITAR_EFFECTS_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${GNU_GUITAR_EFFECTS_VERSION_PATCH})
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")
include(CPack)

